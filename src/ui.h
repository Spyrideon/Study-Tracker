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
    static ImGuiWindowFlags getWindowFlags();

    void drawEntryTable(const std::vector<Entry>& entries) const;
    void drawTracker();
};

#endif //STUDYTRACKER_UI_H