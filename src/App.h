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

    void deleteEntry(int id);
    void editEntry(const EditEntry& editEntry);

    [[nodiscard]] std::vector<Period> getPeriods() const;
    [[nodiscard]] int getActivePeriodId() const;
    [[nodiscard]] const Period* getPeriodById(int) const;
    void addPeriod(const std::string &name);
    void addSubjectToPeriod(int, const std::string &subject);

private:
    PeriodStore periodStore;
    Settings settings;
    EntryStore entryStore;

    std::optional<Entry> openEntry;

    [[nodiscard]] std::string entriesPathFor(int id) const;
};


#endif //STUDYTRACKER_APP_H