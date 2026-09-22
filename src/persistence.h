#ifndef STUDYTRACKER_PERSISTENCE_H
#define STUDYTRACKER_PERSISTENCE_H

#include "EntryStore.h"
#include "Period.h"
#include "Settings.h"
#include <vector>
#include <iosfwd>

namespace persistence {

    [[nodiscard]] std::vector<Entry> loadEntries(std::istream&);

    void saveEntries(const std::vector<Entry>&, std::ostream&);

    [[nodiscard]] std::vector<Period> loadPeriods(std::istream&);

    void savePeriods(const std::vector<Period>&, std::ostream&);

    [[nodiscard]] Settings loadSettings(std::istream&);

    void saveSettings(const Settings&, std::ostream&);

}

#endif