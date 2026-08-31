#include "ui.h"
#include "App.h"

Ui::Ui(App &app) : app(app), noteBuff(""), subjectBuff(""){}

void Ui::render() {
    drawEntryTable(app.getEntries());
    drawTracker();
}


ImGuiWindowFlags Ui::getWindowFlags() {
    constexpr ImGuiWindowFlags flags =      ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoResize;
    return flags;
}

void Ui::drawEntryTable(const std::vector<Entry> &entries) const{
    ImGui::SetNextWindowSize(ImVec2(1000, 500));
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::Begin("Table", nullptr, getWindowFlags());

    static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;

    const int columnCount = 4;
    if (ImGui::BeginTable("table1", columnCount, flags)) {
        ImGui::TableSetupScrollFreeze(0,1);
        ImGui::TableSetupColumn("Subject");
        ImGui::TableSetupColumn("Duration");
        ImGui::TableSetupColumn("Date-Started");
        ImGui::TableSetupColumn("Note");
        ImGui::TableHeadersRow();

        for (int row = 0; row < entries.size(); row++) {
            const Entry& e = entries[row];
            ImGui::PushID(row);
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(e.subject.c_str());     //subject

            ImGui::TableNextColumn();
            ImGui::Text("%lld min", static_cast<long long>(e.duration.count()));

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(std::format("{:%Y-%m-%d %H:%M}",
                std::chrono::floor<std::chrono::seconds>(e.start)).c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(e.note.c_str());

            ImGui::PopID();
        }


        ImGui::EndTable();
    }
    ImGui::End();
}

void Ui::drawTracker() {
    ImGui::SetNextWindowSize(ImVec2(1000, 220));
    ImGui::SetNextWindowPos(ImVec2(0, 500));
    ImGui::Begin("Tracker", nullptr, getWindowFlags());

    if (ImGui::Button("Start Timer"))
        app.startEntry();
    if (app.isTimerRunning()) {
        ImGui::SameLine();
        if (ImGui::Button("Stop Timer")) {
            app.endEntry(noteBuff, subjectBuff);
        }
        ImGui::InputText("Subject", subjectBuff, 20);
        ImGui::InputText("Note", noteBuff, 50);
    }

    ImGui::End();
}