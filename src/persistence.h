#ifndef STUDYTRACKER_PERSISTENCE_H
#define STUDYTRACKER_PERSISTENCE_H

#include "EntryStore.h"
#include "Period.h"
#include <vector>
#include <iosfwd>

namespace persistence {

    std::vector<Entry> loadEntries(std::istream&);

    void saveEntries(const std::vector<Entry>&, std::ostream&);

    std::vector<Period> loadPeriods(std::istream&);

    void savePeriods(const std::vector<Period>&, std::ostream&);

}

#endif