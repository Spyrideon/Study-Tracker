#include "PeriodStore.h"

PeriodStore::PeriodStore(const std::vector<Period> &str) : store(str){
    for (const auto& p : store)
        nextId = std::max(nextId, p.id + 1);
}

const std::vector<Period>& PeriodStore::getAllPeriods() const{
    return store;
}
