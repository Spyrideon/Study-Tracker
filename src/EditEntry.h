#ifndef STUDYTRACKER_EDITENTRY_H
#define STUDYTRACKER_EDITENTRY_H

#include <string>

struct EditEntry {
    const int id;
    const std::string subject;
    const int duration;
    const tm start;
    const std::string note;
};

#endif