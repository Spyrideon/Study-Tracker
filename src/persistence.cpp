#include "persistence.h"

namespace persistence {
    using namespace nlohmann;
    std::vector<Entry> loadEntries(std::istream& stream) {
        std::vector<Entry> loadedStore;
        if (!stream) return loadedStore;
        json j;
        try {
            stream >> j;
            loadedStore = j.get<std::vector<Entry>>();
        } catch (const nlohmann::json::exception&) {return {};}

        return loadedStore;
    }

    void saveEntries(const std::vector<Entry> &toSave, std::ostream& ostream) {
        const json j = toSave;
        ostream << j.dump(4);
    }

    std::vector<Period> loadPeriods(std::istream& stream) {
        std::vector<Period> loadedStore;
        if (!stream) return loadedStore;
        json j;
        try {
            stream >> j;
            loadedStore = j.get<std::vector<Period>>();
        } catch (const nlohmann::json::exception&) {return {};}

        return loadedStore;
    }

    void savePeriods(const std::vector<Period> &toSave, std::ostream& ostream) {
        const json j = toSave;
        ostream << j.dump(4);
    }
}