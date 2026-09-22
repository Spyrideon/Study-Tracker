#include "PeriodStore.h"

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
void PeriodStore::editPeriod(const Period& editPeriod) {
    for (auto& p : store) {
        if (p.id == editPeriod.id) {
            p.name = editPeriod.name;
            p.subjects = editPeriod.subjects;
            return;
        }
    }

}
void PeriodStore::deletePeriod(const int id) {
    for (int i = 0; i < store.size(); i++) {
        if (store[i].id == id) {
            store.erase(store.begin() + i);
            return;
        }
    }
}
[[nodiscard]] const Period* PeriodStore::getById(const int id) const{
    for (const auto& p : store) {
        if (p.id == id)
            return &p;
    }
    return nullptr;
}