#include <gtest/gtest.h>
#include "EntryStore.h"

TEST(EntryStoreTest, AddEntry_IncreasesResultsForSubject) {
    EntryStore store;
    store.addEntry(Entry{.subject = "Math"});
    EXPECT_EQ(store.getAllBySubject("Math").size(), 1);
}