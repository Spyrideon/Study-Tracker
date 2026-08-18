#ifndef STUDYTRACKER_APP_H
#define STUDYTRACKER_APP_H

#include "EntryStore.h"

class App {
public:
    App();

    [[nodiscard]] std::vector<Entry> loadStore() const;

    void saveStore() const;

    [[nodiscard]] const std::vector<Entry>& getEntries() const;
    void addEntry();

private:

    EntryStore entryStore;
};


#endif //STUDYTRACKER_APP_H