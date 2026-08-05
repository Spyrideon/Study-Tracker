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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Entry, id, subject, duration, start, note);

#endif //STUDYTRACKER_ENTRY_H