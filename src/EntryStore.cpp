#include "EntryStore.h"


EntryStore::EntryStore(const std::vector<Entry> &str) : store(str){
    for (const auto& e : store)
        nextId = std::max(nextId, e.id + 1);
}

void EntryStore::addEntry(Entry entry) {
    entry.id = nextId++;
    store.push_back(std::move(entry));
}

bool EntryStore::deleteEntry(const int id){
    for (int i = 0; i < store.size(); i++) {
        if (store[i].id == id) {
            store.erase(store.begin() + i);
            return true;
        }
    }
    return false;
}

const std::vector<Entry>& EntryStore::getAllEntries() const{
    return store;
}

std::vector<Entry> EntryStore::getAllBySubject(const std::string& subject) const {
    std::vector<Entry> list;
    for (auto & item : store) {
        if (item.subject == subject)
            list.push_back(item);
    }
    return list;
}
