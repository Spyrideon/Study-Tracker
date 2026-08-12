#ifndef STUDYTRACKER_ENTRYSTORE_H
#define STUDYTRACKER_ENTRYSTORE_H

#include "Entry.h"
#include <string>
#include <vector>
#include <chrono>

class EntryStore {
public:

    EntryStore() = default;
    explicit EntryStore(const std::vector<Entry> &str);

    void addEntry(Entry entry);

    bool deleteEntry(int id);

    [[nodiscard]] std::vector<Entry> getAllBySubject(const std::string& subject) const;
    [[nodiscard]] std::vector<Entry> getAllEntrys() const;

private:
    std::vector<Entry> store;
    int nextId = 0;
};


#endif //STUDYTRACKER_ENTRYSTORE_H