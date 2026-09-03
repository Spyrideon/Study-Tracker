#ifndef STUDYTRACKER_APP_H
#define STUDYTRACKER_APP_H

#include "EntryStore.h"
#include <optional>

class App {
public:
    App();

    [[nodiscard]] std::vector<Entry> loadStore() const;

    void saveStore() const;

    [[nodiscard]] const std::vector<Entry>& getEntries() const;
    void addEntry();

    bool startEntry();
    bool endEntry(const char* note, const char* subject);
    [[nodiscard]] bool isTimerRunning() const;

    void deleteEntry(const int id);

private:
    std::optional<Entry> openEntry;
    EntryStore entryStore;
};


#endif //STUDYTRACKER_APP_H