#include "persistence.h"

namespace persistence {
    using namespace nlohmann;
    std::vector<Entry> load(std::istream& stream) {
        std::vector<Entry> loadedStore;
        if (!stream) return loadedStore;
        json j;
        try {
            stream >> j;
            loadedStore = j.get<std::vector<Entry>>();
        } catch (const nlohmann::json::exception&) {return {};}

        return loadedStore;
    }

    void save(const std::vector<Entry> &toSave, std::ostream& ostream) {
        const json j = toSave;
        ostream << j.dump(4);
    }
}