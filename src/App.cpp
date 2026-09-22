#include "App.h"

#include "persistence.h"
#include <fstream>

App::App() : entryStore(loadEntryStore()), periodStore(loadPeriodStore()) {

}

std::vector<Entry> App::loadEntryStore() const{
    std::ifstream input("test.json");
    std::vector<Entry> vec = persistence::loadEntries(input);
    input.close();
    return vec;
}

void App::saveEntryStore() const {
    std::ofstream output("test.json");
    persistence::saveEntries(entryStore.getAllEntries(), output);
    output.close();
}

[[nodiscard]] std::vector<Period> App::loadPeriodStore() const {
    std::ifstream input("period.json");
    std::vector<Period> vec = persistence::loadPeriods(input);
    input.close();
    return vec;
}
void App::savePeriodStore() const {
    std::ofstream output("period.json");
    persistence::savePeriods(periodStore.getAllPeriods(), output);
    output.close();
}

[[nodiscard]] Settings App::loadSettings() const{
    std::ifstream input("settings.json");
    Settings sett = persistence::loadSettings(input);
    input.close();
    return sett;
}

void App::saveSettings() const {
    std::ofstream output("settings.json");
    persistence::saveSettings(settings, output);
    output.close();
}

const std::vector<Entry>& App::getEntries() const{
    return entryStore.getAllEntries();
}

bool App::startEntry() {
    if (openEntry) return false;
    openEntry.emplace();
    openEntry->start = std::chrono::system_clock::now();
    return true;
}
bool App::endEntry(const std::string &note, const std::string &subject) {
    if (!openEntry) return false;
    const auto now = std::chrono::system_clock::now();
    openEntry->duration =
        std::chrono::duration_cast<std::chrono::minutes>(now - openEntry->start);
    openEntry->note = note;
    openEntry->subject = subject;
    entryStore.addEntry(*openEntry);
    openEntry.reset();
    saveEntryStore();
    return true;
}
bool App::isTimerRunning() const{
    return openEntry.has_value();
}

void App::deleteEntry(const int id) {
    entryStore.deleteEntry(id);
    saveEntryStore();
}
void App::editEntry(const EditEntry& editEntry) {
    entryStore.editEntry(editEntry);
    saveEntryStore();
}