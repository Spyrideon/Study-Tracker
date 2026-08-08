#include <gtest/gtest.h>
#include "EntryStore.h"

TEST(EntryStoreTest, AddEntry_IncreasesResultsForSubject) {
    EntryStore store;
    store.addEntry(Entry{.subject = "Math"});
    EXPECT_EQ(store.getAllBySubject("Math").size(), 1);
    store.addEntry(Entry{.subject = "Math"});
    EXPECT_EQ(store.getAllBySubject("Math").size(), 2);

}

TEST(EntryStoreTest, AddEntry_TestDefaultValues) {
    EntryStore store;
    store.addEntry(Entry{.subject = "Biology"});

    const auto entries = store.getAllBySubject("Biology");

    ASSERT_EQ(entries.size(), 1u);
    
    EXPECT_EQ(entries[0].duration, std::chrono::minutes{0});
    //EXPECT_EQ(entries[0].start, std::chrono::system_clock::time_point{}); // gets false positive flagged by clangd still works
}

TEST(EntryStoreTest, AddEntry_TestValues){
    EntryStore store;
    const auto timepoint = std::chrono::system_clock::now();
    const Entry entry{.subject = "Math", .duration = std::chrono::minutes{10}, .start = timepoint};
    store.addEntry(entry);

    const auto entries = store.getAllBySubject("Math");

    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries[0].subject, "Math");
    EXPECT_EQ(entries[0].duration, std::chrono::minutes{10});
    //EXPECT_EQ(entries[0].start, timepoint); // gets false positive flagged by clangd still works
}

TEST(EntryStoreTest, AddEntrys_IdIncreases){
    EntryStore store;
    store.addEntry(Entry{.subject = "Math"});
    store.addEntry(Entry{.subject = "Math"});

    const auto entries = store.getAllBySubject("Math");

    ASSERT_EQ(entries.size(), 2u);

    const int firstId = entries[0].id;
    const int secondId = entries[1].id;
    EXPECT_EQ(secondId, firstId + 1);
}