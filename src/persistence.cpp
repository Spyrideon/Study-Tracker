#include "persistence.h"

namespace persistence {
    using namespace nlohmann;
    std::vector<Entry> loadEntries(std::istream& istream) {
        std::vector<Entry> loadedStore;
        if (!istream) return loadedStore;
        json j;
        try {
            istream >> j;
            loadedStore = j.get<std::vector<Entry>>();
        } catch (const nlohmann::json::exception&) {return {};}

        return loadedStore;
    }

    void saveEntries(const std::vector<Entry> &toSave, std::ostream& ostream) {
        const json j = toSave;
        ostream << j.dump(4);
    }

    std::vector<Period> loadPeriods(std::istream& istream) {
        std::vector<Period> loadedStore;
        if (!istream) return loadedStore;
        json j;
        try {
            istream >> j;
            loadedStore = j.get<std::vector<Period>>();
        } catch (const nlohmann::json::exception&) {return {};}

        return loadedStore;
    }

    void savePeriods(const std::vector<Period> &toSave, std::ostream& ostream) {
        const json j = toSave;
        ostream << j.dump(4);
    }

    Settings loadSettings(std::istream& istream) {
        Settings loadedSettings;
        if (!istream) return loadedSettings;
        json j;
        try {
            istream >> j;
            loadedSettings.activePeriodId = j.value("activePeriodId", loadedSettings.activePeriodId);
        }catch (const nlohmann::json::exception&){return {};}
        return loadedSettings;
    }

    void saveSettings(const Settings& settings, std::ostream& ostream) {
        json j = {
            {"activePeriodId", settings.activePeriodId}
        };
        ostream << j.dump(4);
    }
}
