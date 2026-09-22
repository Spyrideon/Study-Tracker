#include "EntryStore.h"


namespace {
    std::chrono::time_point<std::chrono::system_clock> toChronoTimePoint(tm timePoint);
}

EntryStore::EntryStore(const std::vector<Entry> &str) : store(str){
    for (const auto& e : store)
        nextId = std::max(nextId, e.id + 1);
}

void EntryStore::addEntry(Entry entry) {
    entry.id = nextId++;
    store.insert(store.begin(), std::move(entry));
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
void EntryStore::editEntry(const EditEntry &editEntry) {
    for (auto & e : store) {
        if (e.id == editEntry.id) {
            e.subject = editEntry.subject;
            e.duration = std::chrono::minutes{editEntry.duration};
            e.start = toChronoTimePoint(editEntry.start);
            e.note = editEntry.note;
            return;
        }
    }
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

namespace {
    std::chrono::system_clock::time_point toChronoTimePoint(const tm t) {
        const std::chrono::year_month_day ymd{
            std::chrono::year{t.tm_year + 1900},
            std::chrono::month{static_cast<unsigned>(t.tm_mon + 1)},
            std::chrono::day{static_cast<unsigned>(t.tm_mday)}};
        const std::chrono::local_seconds lt =
            std::chrono::local_days{ymd}
        + std::chrono::hours{t.tm_hour}
        + std::chrono::minutes{t.tm_min};
        return std::chrono::current_zone()->to_sys(lt);               // local -> UTC
    }
}