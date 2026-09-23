#include "PeriodStore.h"
#include <algorithm>
#include <vector>

PeriodStore::PeriodStore(const std::vector<Period> &str) : store(str){
    for (const auto& p : store)
        nextId = std::max(nextId, p.id + 1);
}

[[nodiscard]] const std::vector<Period>& PeriodStore::getAllPeriods() const{
    return store;
}

int PeriodStore::addPeriod(Period period) {
    period.id = nextId++;
    const int id = period.id;
    store.push_back(std::move(period));
    return id;
}

bool PeriodStore::rename(const int id, const std::string &name) {
    if (Period* p = find(id)) {
        p->name = name; return true;
    }
    return false;
}

bool PeriodStore::addSubject(const int id, const std::string &subject) {
    Period* p = find(id);
    auto& v = p->subjects;
    const auto before = v.size();
    v.push_back(subject);
    return v.size() != before;
}

bool PeriodStore::removeSubject(const int id, const std::string &subject) {
    Period* p = find(id);
    auto& v = p->subjects;
    const auto before = v.size();
    v.erase(std::remove(v.begin(), v.end(), subject), v.end());
    return v.size() != before;
}

bool PeriodStore::deletePeriod(const int id) {
    for (int i = 0; i < store.size(); i++) {
        if (store[i].id == id) {
            store.erase(store.begin() + i);
            return true;
        }
    }
    return false;
}
[[nodiscard]] const Period* PeriodStore::getById(const int id) const{
    for (const auto& p : store) {
        if (p.id == id)
            return &p;
    }
    return nullptr;
}

Period* PeriodStore::find(int id) {
    for (auto& p : store) if (p.id == id) return &p;
    return nullptr;
}