#ifndef STUDYTRACKER_APP_H
#define STUDYTRACKER_APP_H

#include "EntryStore.h"
#include "PeriodStore.h"
#include "Settings.h"
#include <optional>
#include "EditEntry.h"

class App {
public:
    App();

    [[nodiscard]] std::vector<Entry> loadEntryStore() const;
    void saveEntryStore() const;

    [[nodiscard]] std::vector<Period> loadPeriodStore() const;
    void savePeriodStore() const;

    [[nodiscard]] Settings loadSettings() const;
    void saveSettings() const;

    [[nodiscard]] const std::vector<Entry>& getEntries() const;

    bool startEntry();
    bool endEntry(const std::string &note, const std::string &subject);
    [[nodiscard]] bool isTimerRunning() const;

    void deleteEntry(const int id);
    void editEntry(const EditEntry& editEntry);

private:
    std::optional<Entry> openEntry;
    EntryStore entryStore;
    PeriodStore periodStore;
    Settings settings;
};


#endif //STUDYTRACKER_APP_H