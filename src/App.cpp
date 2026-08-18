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
