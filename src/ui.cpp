#include "ui.h"
#include "App.h"
#include "EditEntry.h"
#include "ImGuiDatePicker.hpp"

Ui::Ui(App &app) : app(app){}

void Ui::render() {
    drawEntryTable(app.getEntries());
    drawTracker();
}

void Ui::drawEntryTable(const std::vector<Entry> &entries){
    ImGui::Begin("Table", nullptr);

    static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Reorderable;

    const int columnCount = 5;
    int toDelete = -1;
    if (ImGui::BeginTable("table1", columnCount, flags)) {
        ImGui::TableSetupScrollFreeze(0,1);
        ImGui::TableSetupColumn("Subject");
        ImGui::TableSetupColumn("Duration");
        ImGui::TableSetupColumn("Date-Started");
        ImGui::TableSetupColumn("Note");
        ImGui::TableSetupColumn("Action");
        ImGui::TableHeadersRow();

        for (int row = 0; row < entries.size(); row++) {
            const Entry& e = entries[row];
            ImGui::PushID(row);
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(e.subject.c_str());

            ImGui::TableNextColumn();
            ImGui::Text(formatDuration(e.duration).c_str());

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
    ImGui::Begin("Tracker", nullptr);

    if (ImGui::Button("Start Timer"))
        app.startEntry();
    if (app.isTimerRunning()) {
        ImGui::SameLine();
        if (ImGui::Button("Stop Timer")) {
            app.endEntry(noteBuff, subjectBuff);
        }
        ImGui::InputText("Subject", &subjectBuff);
        ImGui::InputText("Note", &noteBuff);
    }

    ImGui::End();
}

void Ui::drawEditPopup() {
    ImGui::SetNextWindowSize(ImVec2(360, 0), ImGuiCond_Appearing);

    if (ImGui::BeginPopupModal("Edit Entry", nullptr)) {

        ImGui::PushItemWidth(-FLT_MIN);

        ImGui::TextUnformatted("Subject");
        ImGui::InputText("##subject", &editSubjectBuff);

        ImGui::TextUnformatted("Duration");
        ImGui::InputInt("##duration", &editDuration);

        if (ImGui::DatePicker("##date", editDate)){}
        ImGui::TableNextColumn();

        ImGui::TextUnformatted("Note");
        ImGui::InputText("##note", &editNoteBuff);

        ImGui::PopItemWidth();

        ImGui::Spacing();
        if (ImGui::Button("Save")) {
            const EditEntry editEntry(editId, editSubjectBuff, editNoteBuff);
            app.editEntry(editEntry);
            editId = -1;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")){ImGui::CloseCurrentPopup();}

        ImGui::EndPopup();
    }
}

[[nodiscard]] std::string Ui::formatDuration(const std::chrono::minutes d) const{
    const long long total = d.count();
    if (total < 60)
        return std::format("{}m", total);
    return std::format("{}h{:02d}m", total / 60, static_cast<int>(total % 60));
}