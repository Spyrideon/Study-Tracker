#ifndef STUDYTRACKER_UI_H
#define STUDYTRACKER_UI_H

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Entry.h"

class App;

class Ui {
public:
    explicit Ui(App &app);

    void render();

private:
    App &app;
    int pendingOptionsPopup = false;

    std::string noteBuff;
    std::string subjectBuff ;

    int editId = -1;
    bool pendingEdit = false;

    std::string editSubjectBuff;
    int editDuration = 0;
    std::string editNoteBuff;
    tm editDate;

    int optionsSelectedId = -1;
    std::string periodNameBuffer;

    void drawEntryTable(const std::vector<Entry>& entries);
    void drawTracker();
    void drawEditPopup();
    void drawMenu();
    void drawOptionsPopup();
    void drawPeriodCombo();

    [[nodiscard]] std::string formatDuration(std::chrono::minutes d) const;
};

#endif