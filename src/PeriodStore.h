#ifndef STUDYTRACKER_PERIODSTORE_H
#define STUDYTRACKER_PERIODSTORE_H

#include <vector>
#include "Period.h"

class PeriodStore {
public:
    PeriodStore() = default;
    explicit PeriodStore(const std::vector<Period> &str);

    [[nodiscard]] const std::vector<Period>& getAllPeriods() const;

    int addPeriod(Period period);
    bool rename(int, const std::string&);
    bool addSubject(int, const std::string&);
    bool removeSubject(int, const std::string&);
    bool deletePeriod(int id);
    [[nodiscard]] const Period* getById(int id) const;

private:
    std::vector<Period> store;
    int nextId = 0;

    Period* find(int id);
};


#endif