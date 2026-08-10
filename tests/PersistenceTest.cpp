#include <gtest/gtest.h>
#include "persistence.h"

#include <sstream>
#include <vector>
#include <chrono>


//helper
namespace {
using namespace std::chrono;

Entry makeEntry(int id, std::string subject, int minutesStudied,
                long long epochSeconds, std::string note) {
    Entry e;
    e.id       = id;
    e.subject  = std::move(subject);
    e.duration = minutes{minutesStudied};
    e.start    = system_clock::time_point{ seconds{epochSeconds} };
    e.note     = std::move(note);
    return e;
}
}


TEST(PersistenceTest, RoundTripPreservesEntries) {
    std::vector<Entry> original {
        makeEntry(0, "Math",   90, 1700000000, "chapter 3"),
        makeEntry(1, "German", 45, 1700003600, "essay draft"),
    };

    std::ostringstream out;
    persistence::save(original, out);

    std::istringstream in(out.str());
    std::vector<Entry> loaded = persistence::load(in);

    EXPECT_EQ(loaded, original);
}

TEST(PersistenceTest, RoundTripEmptyVector) {
    std::vector<Entry> original;

    std::ostringstream out;
    persistence::save(original, out);

    std::istringstream in(out.str());
    EXPECT_TRUE(persistence::load(in).empty());
}

TEST(PersistenceTest, SaveWritesJsonArray) {
    std::ostringstream out;
    persistence::save({ makeEntry(0, "Math", 30, 1700000000, "warmup") }, out);

    const std::string text = out.str();
    ASSERT_FALSE(text.empty());
    EXPECT_EQ(text.front(), '[');
}

TEST(PersistenceTest, LoadFromFailedStreamReturnsEmpty) {
    std::istringstream in;
    in.setstate(std::ios::failbit);

    EXPECT_TRUE(persistence::load(in).empty());
}