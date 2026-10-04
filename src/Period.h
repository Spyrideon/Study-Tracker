#ifndef STUDYTRACKER_PERIOD_H
#define STUDYTRACKER_PERIOD_H

#include <string>
#include <vector>
#include "json.hpp"

struct Period {
    int id = -1;
    std::string name;
    std::vector<std::string> subjects;
};

inline void to_json(nlohmann::json& j, const Period& e) {
    j = nlohmann::json{
                {"id",       e.id},
                {"name",  e.name},
                {"subjects", e.subjects}
    };
}

inline void from_json(const nlohmann::json& j, Period& p) {
    p.id       = j.at("id").get<int>();
    p.name  = j.at("name").get<std::string>();
    p.subjects = j.at("subjects").get<std::vector<std::string>>();
}

#endif