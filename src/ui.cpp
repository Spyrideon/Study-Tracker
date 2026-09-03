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
            ImGui::TextUnformatted(e.subject.c_str());     //subject

            ImGui::TableNextColumn();
            ImGui::Text("%lld min", static_cast<long long>(e.duration.count()));

            ImGui::TableNextColumn();
            const auto local = std::chrono::current_zone()->to_local(e.start);
            ImGui::TextUnformatted(std::format("{:%Y-%m-%d %H:%M}",
                std::chrono::floor<std::chrono::seconds>(local)).c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(e.note.c_str());

            ImGui::TableNextColumn();
            if (ImGui::SmallButton("Delete")){toDelete = e.id;}

            ImGui::PopID();
        }


        ImGui::EndTable();
    }

    if (toDelete != -1) {
        app.deleteEntry(toDelete);
    }

    ImGui::End();
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
        ImGui::InputText("Subject", subjectBuff, 20);
        ImGui::InputText("Note", noteBuff, 50);
    }

    ImGui::End();
}