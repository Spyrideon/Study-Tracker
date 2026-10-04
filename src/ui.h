#ifndef STUDYTRACKER_UI_H
#define STUDYTRACKER_UI_H

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Period.h"
#include "Entry.h"

class App;

class Ui {
public:
    explicit Ui(App &app);

    void render();

private:
    App &app;
    bool pendingOptionsPopup = false;

    std::string noteBuff;
    std::string subjectBuff ;

    int editId = -1;
    bool pendingEdit = false;

    std::string editSubjectBuff;
    int editDuration = 0;
    std::string editNoteBuff;
    tm editDate;

    int optionsSelPeriodId = -1;
    int optionsSelSubjectIdx = -1;
    std::string periodNameBuff;
    std::string periodSubjectBuff;

    int deletePeriodId = -1;

    void drawEntryTable(const std::vector<Entry>& entries);
    void drawTracker();
    void drawEditPopup();
    void drawMenu();
    void drawOptionsPopup();
    void drawPeriodCombo();
    void drawSubjectCombo(const char* id, const std::vector<std::string>& subjects, std::string& selection);
    void drawDeletePeriodPopup();

    [[nodiscard]] std::string formatDuration(std::chrono::minutes d) const;
};

#endif