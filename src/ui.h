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
    std::string noteBuff = "";
    std::string subjectBuff = "";

    int editId = -1;
    bool pendingEdit = false;

    std::string editSubjectBuff = "";
    std::string editNoteBuff = "";

    void drawEntryTable(const std::vector<Entry>& entries);
    void drawTracker();
    void drawEditPopup();
};

#endif //STUDYTRACKER_UI_H