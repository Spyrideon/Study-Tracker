#ifndef STUDYTRACKER_PERSISTENCE_H
#define STUDYTRACKER_PERSISTENCE_H

#include "EntryStore.h"
#include <vector>
#include <iosfwd>

namespace persistence {

    std::vector<Entry> load(std::istream&);

    void save(const std::vector<Entry> &, std::ostream&);

}

#endif