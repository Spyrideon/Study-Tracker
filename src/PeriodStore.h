#ifndef STUDYTRACKER_PERIODSTORE_H
#define STUDYTRACKER_PERIODSTORE_H

#include <vector>
#include "Period.h"

class PeriodStore {
public:
    PeriodStore() = default;
    explicit PeriodStore(const std::vector<Period> &str);

    const std::vector<Period>& getAllPeriods() const;

private:
    std::vector<Period> store;
    int nextId = 0;
};


#endif