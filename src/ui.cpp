#include "ui.h"
#include "App.h"
#include "EditEntry.h"
#include "ImGuiDatePicker.hpp"
#include "implot.h"
#include <algorithm>

namespace {
    enum EntryColumn : ImGuiID {
        ColSubject,
        ColDuration,
        ColDate,
        ColNote,
        ColAction
    };

    template <typename T>
    int compare3(const T& a, const T& b) {
        if (a < b) return -1;
        if (b < a) return 1;
        return 0;
    }

    int compareIgnoreCase(const std::string& a, const std::string& b) {
        const size_t n = std::min(a.size(), b.size());
        for (size_t i = 0; i < n; ++i) {
            const int ca = std::tolower(static_cast<unsigned char>(a[i]));
            const int cb = std::tolower(static_cast<unsigned char>(b[i]));
            if (ca != cb) return ca < cb ? -1 : 1;
        }
        return compare3(a.size(), b.size());
    }

    void sortEntries(std::vector<const Entry*>& view, const ImGuiTableSortSpecs* specs) {
        std::ranges::sort(view, [specs](const Entry* a, const Entry* b) {
            for (int n = 0; n < specs->SpecsCount; ++n) {
                const ImGuiTableColumnSortSpecs& s = specs->Specs[n];
                int delta = 0;
                switch (s.ColumnUserID) {
                    case ColSubject:  delta = compareIgnoreCase(a->subject, b->subject); break;
                    case ColDuration: delta = compare3(a->duration, b->duration);       break;
                    case ColDate:     delta = compare3(a->start, b->start);             break;
                    default: break;
                }
                if (delta != 0)
                    return s.SortDirection == ImGuiSortDirection_Ascending ? delta < 0 : delta > 0;
            }
            return a->id < b->id;
        });
    }
}


Ui::Ui(App &app) : app(app){}

void Ui::render() {
    drawMenu();
    drawEntryTable(app.getEntries());
    drawTracker();
    drawStatistics();
}

void Ui::drawEntryTable(const std::vector<Entry> &entries){
    ImGui::Begin("Table", nullptr);

    static ImGuiTableFlags flags =  ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable |
                                    ImGuiTableFlags_Reorderable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti;

    const int columnCount = 5;
    int toDelete = -1;
    if (ImGui::BeginTable("table1", columnCount, flags)) {
        ImGui::TableSetupScrollFreeze(0,1);
        ImGui::TableSetupColumn("Subject", ImGuiTableColumnFlags_None, 0.0f, ColSubject);
        ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_PreferSortDescending, 0.0f, ColDuration);
        ImGui::TableSetupColumn("Date-Started", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_PreferSortDescending, 0.0f, ColDate);
        ImGui::TableSetupColumn("Note", ImGuiTableColumnFlags_NoSort, 0.0f, ColNote);
        ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_NoSort, 0.0f, ColAction);
        ImGui::TableHeadersRow();

        sortedView.clear();
        sortedView.reserve(entries.size());
        for (const Entry& e : entries)
            sortedView.push_back(&e);

        if (ImGuiTableSortSpecs* specs = ImGui::TableGetSortSpecs()) {
            if (specs->SpecsCount > 0)
                sortEntries(sortedView, specs);
            specs->SpecsDirty = false;
        }


        for (const Entry* ep : sortedView) {
            const Entry& e = *ep;
            ImGui::PushID(e.id);
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(e.subject.c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(formatDuration(e.duration).c_str());

            ImGui::TableNextColumn();
            const auto local = std::chrono::current_zone()->to_local(e.start);
            ImGui::TextUnformatted(std::format("{:%Y-%m-%d %H:%M}",
                std::chrono::floor<std::chrono::seconds>(local)).c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(e.note.c_str());

            ImGui::TableNextColumn();
            if (ImGui::SmallButton("Edit")) {
                editId = e.id;
                pendingEdit = true;
                editSubjectBuff = e.subject;
                editDuration = e.duration.count();
                editNoteBuff = e.note;
                std::time_t tt = std::chrono::system_clock::to_time_t(e.start);
                editDate = *std::localtime(&tt);
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Delete")){toDelete = e.id;}

            ImGui::PopID();
        }

        ImGui::EndTable();
    }
    ImGui::End();

    if (toDelete != -1) {
        app.deleteEntry(toDelete);
    }
    if (pendingEdit) {
        ImGui::OpenPopup("Edit Entry");
        pendingEdit = false;
    }
    drawEditPopup();
}

void Ui::drawTracker() {
    ImGui::Begin("Tracker");

    const bool running = app.isTimerRunning();

    ImGui::BeginDisabled(running);
    if (ImGui::Button("Start Timer"))
        app.startEntry();
    ImGui::EndDisabled();

    if (running) {
        const auto& subjects = app.getActivePeriod().subjects;

        if (std::ranges::find(subjects, subjectBuff) == subjects.end())
            subjectBuff = subjects.empty() ? "" : subjects.front();

        ImGui::SameLine();
        ImGui::BeginDisabled(subjectBuff.empty());
        if (ImGui::Button("Stop Timer")) {
            app.endEntry(noteBuff, subjectBuff);
            noteBuff.clear();
        }
        ImGui::EndDisabled();

        drawSubjectCombo("##trackerSubject", subjects, subjectBuff);
        ImGui::InputText("Note", &noteBuff);
    }

    ImGui::End();
}

void Ui::drawStatistics() {
    ImGui::Begin("Statistics");

    std::map<std::string, std::chrono::minutes> totals;
    std::chrono::minutes sum{0};
    for (const Entry& e : app.getEntries()) {
        totals[e.subject] += e.duration;
        sum += e.duration;
    }

    if (sum.count() == 0) {
        ImGui::TextDisabled("No study time recorded in this period yet.");
        ImGui::End();
        return;
    }

    std::vector<std::pair<std::string, std::chrono::minutes>> sorted(totals.begin(), totals.end());
    std::ranges::stable_sort(sorted, std::ranges::greater{},
                             [](const auto& p) { return p.second; });

    const int n = static_cast<int>(sorted.size());
    std::vector<const char*> names;
    std::vector<double> positions, percents;
    std::vector<std::string> barLabels;
    names.reserve(n); positions.reserve(n); percents.reserve(n); barLabels.reserve(n);

    double maxPct = 0.0;
    for (int i = 0; i < n; ++i) {
        const auto& [subject, time] = sorted[i];
        const double pct = 100.0 * static_cast<double>(time.count()) / static_cast<double>(sum.count());
        names.push_back(subject.c_str());
        positions.push_back(i);
        percents.push_back(pct);
        barLabels.push_back(std::format("{} ({:.0f}%)", formatDuration(time), pct));
        maxPct = std::max(maxPct, pct);
    }

    ImGui::Text("Total: %s", formatDuration(sum).c_str());

    constexpr ImPlotFlags plotFlags = ImPlotFlags_NoLegend | ImPlotFlags_NoMenus
                                    | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText;

    if (ImPlot::BeginPlot("##timePerSubject", ImVec2(-1, -1), plotFlags)) {
        ImPlot::SetupAxes(nullptr, nullptr,
                          ImPlotAxisFlags_Lock,
                          ImPlotAxisFlags_Lock | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoGridLines);
        ImPlot::SetupAxisFormat(ImAxis_X1, "%.0f%%");
        ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, maxPct * 1.35, ImPlotCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, -0.5, n - 0.5, ImPlotCond_Always);
        ImPlot::SetupAxisTicks(ImAxis_Y1, positions.data(), n, names.data());

        ImPlot::PlotBars("##time", percents.data(), n, 0.6, 0.0, ImPlotSpec(ImPlotProp_Flags, ImPlotBarsFlags_Horizontal));

        for (int i = 0; i < n; ++i) {
            const char* txt = barLabels[i].c_str();
            const float off = ImGui::CalcTextSize(txt).x * 0.5f + 6.0f;
            ImPlot::PlotText(txt, percents[i], positions[i], ImVec2(off, 0));
        }
        ImPlot::EndPlot();
    }

    ImGui::End();
}

void Ui::drawEditPopup() {
    ImGui::SetNextWindowSize(ImVec2(360, 0), ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal("Edit Entry", nullptr)) {

        ImGui::PushItemWidth(-FLT_MIN);

        ImGui::TextUnformatted("Subject");
        drawSubjectCombo("##editSubject", app.getActivePeriod().subjects, editSubjectBuff);

        ImGui::TextUnformatted("Duration");
        ImGui::InputInt("##duration", &editDuration);

        if (ImGui::DatePicker("##date", editDate)) {}
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##hour",   &editDate.tm_hour, 0, 23, "%02dh", ImGuiSliderFlags_AlwaysClamp);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##minute", &editDate.tm_min, 0, 59, "%02dm", ImGuiSliderFlags_AlwaysClamp);

        ImGui::TextUnformatted("Note");
        ImGui::InputText("##note", &editNoteBuff);

        ImGui::PopItemWidth();

        ImGui::Spacing();
        if (ImGui::Button("Save")) {
            const EditEntry editEntry(editId, editSubjectBuff, editDuration, editDate, editNoteBuff);
            app.editEntry(editEntry);
            editId = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")){ImGui::CloseCurrentPopup();}

        ImGui::EndPopup();
    }
}

void Ui::drawMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_S, ImGuiInputFlags_Tooltip);
            if (ImGui::MenuItem("Options", "Ctrl+S")) {
                pendingOptionsPopup = true;
                optionsSelPeriodId = app.getActivePeriodId();
            }
            if (ImGui::BeginMenu("Change record", !app.isTimerRunning())) {
                const int activeId = app.getActivePeriodId();

                for (const Period& p : app.getPeriods()) {
                    ImGui::PushID(p.id);
                    if (ImGui::MenuItem(p.name.c_str(), nullptr, p.id == activeId))
                        app.swapActivePeriod(p.id);
                    ImGui::PopID();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (pendingOptionsPopup) {
        ImGui::OpenPopup("OptionsPopup");
        pendingOptionsPopup = false;
    }
    drawOptionsPopup();
}

void Ui::drawOptionsPopup() {
    if (ImGui::BeginPopupModal("OptionsPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        drawPeriodCombo();

        if (const Period* selected = app.getPeriodById(optionsSelPeriodId)) {
            ImGui::InputText("##periodSubject", &periodSubjectBuff);
            ImGui::SameLine();
            if (ImGui::Button("Add subject") && !(periodSubjectBuff.empty())) {
                app.addSubject(selected->id, periodSubjectBuff);
                periodSubjectBuff.clear();
            }
            else {
                ImGui::SameLine();
                ImGui::TextDisabled("Input a subject name.");
            }
        }else {
            ImGui::TextDisabled("Select a period to edit.");
        }

        ImGui::Separator();

        ImGui::InputText("##periodName", &periodNameBuff);
        ImGui::SameLine();
        if (ImGui::Button("New period") && !periodNameBuff.empty()) {
            app.addPeriod(periodNameBuff);
            periodNameBuff.clear();
        }
        else {
            ImGui::SameLine();
            ImGui::TextDisabled("Input a period name");
        }

        if (ImGui::SmallButton("Exit")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void Ui::drawPeriodCombo() {
    const auto& periods = app.getPeriods();

    auto it = std::find_if(periods.begin(),
                                                                periods.end(),
                                                                [&] (const Period& p) {return p.id == optionsSelPeriodId; });

    const Period* current = (it != periods.end()) ? &*it : nullptr;

    const char* periodPreview = current ? current->name.c_str() : "Select a period";

    if (ImGui::BeginCombo("##periodSelect", periodPreview, ImGuiComboFlags_WidthFitPreview)) {
        for (const Period& p : periods) {
            ImGui::PushID(p.id);
            const bool selected = (p.id == optionsSelPeriodId);
            if (ImGui::Selectable(p.name.c_str(), selected)) {
                optionsSelPeriodId = p.id;
                optionsSelSubjectIdx = 0;
                current = &p;
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (current) {
        const int subjCount = static_cast<int>(current->subjects.size());
        if (optionsSelSubjectIdx < 0 || optionsSelSubjectIdx >= subjCount)
            optionsSelSubjectIdx = 0;

        const char* subjectPreview = (subjCount == 0) ? "No subjects yet" : current->subjects[optionsSelSubjectIdx].c_str();
        if (ImGui::BeginCombo("##subjectSelect", subjectPreview, ImGuiComboFlags_WidthFitPreview)) {
            for (int i = 0; i < static_cast<int>(current->subjects.size()); ++i) {
                const std::string& s = current->subjects[i];
                const bool selectedSubj = (i == optionsSelSubjectIdx);

                if (ImGui::Selectable(s.c_str(), selectedSubj))
                    optionsSelSubjectIdx = i;
                if (selectedSubj)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    if (current && !current->subjects.empty()) {
        ImGui::SameLine();
        if (ImGui::Button("Delete selected subject")) {
            const std::string toDelete = current->subjects[optionsSelSubjectIdx];
            app.deleteSubject(current->id, toDelete);
            optionsSelSubjectIdx = -1;
        }
    }

    if (current) {
        const bool isActive = current->id == app.getActivePeriodId();
        const bool lastOne  = periods.size() <= 1;
        const bool locked   = isActive && app.isTimerRunning();

        ImGui::SameLine();
        ImGui::BeginDisabled(lastOne || locked);
        if (ImGui::Button("Delete period")) {
            deletePeriodId = current->id;
            ImGui::OpenPopup("Delete period?");
        }
        ImGui::EndDisabled();
        if ((lastOne || locked) && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            ImGui::SetTooltip(lastOne ? "Can't delete the only period." : "Stop the timer first.");
    }
    drawDeletePeriodPopup();

}

void Ui::drawSubjectCombo(const char* id,
                          const std::vector<std::string>& subjects,
                          std::string& selection) {
    if (subjects.empty() && selection.empty()) {
        ImGui::TextDisabled("No subjects in this period - add some via File > Options.");
        return;
    }

    const bool orphaned = !selection.empty() &&
                          std::ranges::find(subjects, selection) == subjects.end();

    ImGui::SetNextItemWidth(-FLT_MIN);

    if (orphaned)
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
    const bool open = ImGui::BeginCombo(id, selection.c_str());
    if (orphaned)
        ImGui::PopStyleColor();

    if (open) {
        if (orphaned) {
            ImGui::BeginDisabled();
            ImGui::Selectable(selection.c_str(), true);
            ImGui::EndDisabled();
            ImGui::Separator();
        }
        for (const std::string& s : subjects) {
            const bool selected = (s == selection);
            if (ImGui::Selectable(s.c_str(), selected))
                selection = s;
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void Ui::drawDeletePeriodPopup() {
    if (ImGui::BeginPopupModal("Delete period?", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        const Period* p = app.getPeriodById(deletePeriodId);
        ImGui::Text("Delete \"%s\" and all its entries?", p ? p->name.c_str() : "?");
        ImGui::TextDisabled("This cannot be undone.");
        ImGui::Spacing();

        if (ImGui::Button("Delete")) {
            app.deletePeriod(deletePeriodId);
            optionsSelPeriodId   = app.getActivePeriodId();
            optionsSelSubjectIdx = 0;
            deletePeriodId = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            deletePeriodId = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

[[nodiscard]] std::string Ui::formatDuration(const std::chrono::minutes d) const{
    const long long total = d.count();
    if (total < 60)
        return std::format("{}m", total);
    return std::format("{}h{:02d}m", total / 60, static_cast<int>(total % 60));
}