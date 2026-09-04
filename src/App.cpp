#include "App.h"

#include "persistence.h"
#include <fstream>

App::App() : entryStore(loadStore()) {

}

std::vector<Entry> App::loadStore() const{
    std::ifstream input("test.json");
    std::vector<Entry> vec = persistence::load(input);
    input.close();
    return vec;
}

void App::saveStore() const {
    std::ofstream output("test.json");
    persistence::save(entryStore.getAllEntries(), output);
    output.close();
}

const std::vector<Entry>& App::getEntries() const{
    return entryStore.getAllEntries();
}

void App::addEntry() {
    Entry e;
    e.note = "This is a test";
    e.subject = "Math";
    entryStore.addEntry(e);
    saveStore();
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
    saveStore();
    return true;
}
bool App::isTimerRunning() const{
    return openEntry.has_value();
}

void App::deleteEntry(const int id) {
    entryStore.deleteEntry(id);
    saveStore();
}