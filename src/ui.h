#ifndef STUDYTRACKER_UI_H
#define STUDYTRACKER_UI_H

#include "imgui.h"
#include "Entry.h"

class App;

class Ui {
public:
    explicit Ui(App &app);

    void render();

private:
    App &app;
    char noteBuff[50];
    char subjectBuff[20];

    int editId;
    bool pendingEdit;

    void drawEntryTable(const std::vector<Entry>& entries);
    void drawTracker();
    void drawEditPopup();
};

#endif //STUDYTRACKER_UI_H