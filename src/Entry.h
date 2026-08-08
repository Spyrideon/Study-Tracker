#ifndef STUDYTRACKER_ENTRY_H
#define STUDYTRACKER_ENTRY_H

#include <chrono>
#include <string>
#include "json.hpp"


struct Entry {
    int id = 0;
    std::string subject;
    std::chrono::minutes duration{0};
    std::chrono::time_point<std::chrono::system_clock> start{};
    std::string note;
};

inline void to_json(nlohmann::json& j, const Entry& e) {
    j = nlohmann::json{
            {"id",       e.id},
            {"subject",  e.subject},
            {"duration", e.duration.count()},        // minutes -> integer
            {"start",    std::chrono::duration_cast<std::chrono::seconds>(
                             e.start.time_since_epoch()).count()},  // seconds since epoch
            {"note",     e.note}
    };
}

inline void from_json(const nlohmann::json& j, Entry& e) {
    e.id       = j.at("id").get<int>();
    e.subject  = j.at("subject").get<std::string>();
    e.duration = std::chrono::minutes{ j.at("duration").get<long long>() };
    e.start    = std::chrono::system_clock::time_point{
        std::chrono::seconds{ j.at("start").get<long long>() } };
    e.note     = j.at("note").get<std::string>();
}

#endif //STUDYTRACKER_ENTRY_H