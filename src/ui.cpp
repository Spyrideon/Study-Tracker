#include "ui.h"
#include "App.h"
#include "imgui.h"

namespace {     // private helpers
    void drawEntryTable(const std::vector<Entry>& entries);
    ImGuiWindowFlags getWindowFlags();
}

namespace ui {
    void render(App &app) {
        bool open = true;
        ImGui::SetNextWindowSize(ImVec2(1000, 500));
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::Begin("Table", &open, getWindowFlags());
        drawEntryTable(app.getEntries());
        ImGui::End();

        ImGui::SetNextWindowSize(ImVec2(300, 500));
        ImGui::SetNextWindowPos(ImVec2(1000, 0));
        ImGui::Begin("Tracker", &open, getWindowFlags());
        if (ImGui::Button("TestButton")) {
            app.addEntry();
        }
        ImGui::End();
    }
}

namespace {
    void drawEntryTable(const std::vector<Entry>& entries) {

        static ImGuiTableFlags flags = ImGuiTableFlags_RowBg;

        const int columnCount = 4;
        if (ImGui::BeginTable("table1", columnCount, flags)) {
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
    }

    ImGuiWindowFlags getWindowFlags() {
        const ImGuiWindowFlags flags =      ImGuiWindowFlags_NoCollapse |
                                            ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoResize;
        return flags;
    }
}