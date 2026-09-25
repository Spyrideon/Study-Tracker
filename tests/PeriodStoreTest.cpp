#include <gtest/gtest.h>
#include "PeriodStore.h"

// ---------- addPeriod / constructor ----------

TEST(PeriodStoreTest, AddPeriod_IdsStartAtZeroAndIncrease) {
    PeriodStore store;
    const int first = store.addPeriod(Period{.name = "WS25"});
    const int second = store.addPeriod(Period{.name = "SS26"});

    EXPECT_EQ(first, 0);
    EXPECT_EQ(second, first + 1);
    EXPECT_EQ(store.getAllPeriods().size(), 2u);
}

TEST(PeriodStoreTest, AddPeriod_OverridesGivenId) {
    PeriodStore store;
    const int id = store.addPeriod(Period{.id = 42, .name = "WS25"});

    EXPECT_EQ(id, 0);
    EXPECT_EQ(store.getById(42), nullptr);
    ASSERT_NE(store.getById(0), nullptr);
}

TEST(PeriodStoreTest, Constructor_NextIdContinuesAfterHighestLoadedId) {
    const std::vector<Period> loaded{
        Period{.id = 3, .name = "A"},
        Period{.id = 7, .name = "B"},
        Period{.id = 5, .name = "C"},
    };
    PeriodStore store(loaded);

    EXPECT_EQ(store.addPeriod(Period{.name = "D"}), 8);
}

TEST(PeriodStoreTest, AddPeriod_StoresValues) {
    PeriodStore store;
    const int id = store.addPeriod(Period{.name = "WS25", .subjects = {"Math", "OS"}});

    const Period* p = store.getById(id);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->name, "WS25");
    EXPECT_EQ(p->subjects, (std::vector<std::string>{"Math", "OS"}));
}

// ---------- getById ----------

TEST(PeriodStoreTest, GetById_UnknownIdReturnsNullptr) {
    PeriodStore store;
    store.addPeriod(Period{.name = "WS25"});

    EXPECT_EQ(store.getById(99), nullptr);
    EXPECT_EQ(store.getById(-1), nullptr);
}

// ---------- rename ----------

TEST(PeriodStoreTest, Rename_ChangesName) {
    PeriodStore store;
    const int id = store.addPeriod(Period{.name = "Old"});

    EXPECT_TRUE(store.rename(id, "New"));
    EXPECT_EQ(store.getById(id)->name, "New");
}

TEST(PeriodStoreTest, Rename_UnknownIdReturnsFalse) {
    PeriodStore store;
    EXPECT_FALSE(store.rename(99, "New"));
}

// ---------- addSubject ----------

TEST(PeriodStoreTest, AddSubject_AppendsSubject) {
    PeriodStore store;
    const int id = store.addPeriod(Period{.name = "WS25"});

    EXPECT_TRUE(store.addSubject(id, "Math"));
    EXPECT_TRUE(store.addSubject(id, "OS"));
    EXPECT_EQ(store.getById(id)->subjects, (std::vector<std::string>{"Math", "OS"}));
}

TEST(PeriodStoreTest, AddSubject_RejectsExactDuplicate) {
    PeriodStore store;
    const int id = store.addPeriod(Period{.name = "WS25"});

    ASSERT_TRUE(store.addSubject(id, "Math"));
    EXPECT_FALSE(store.addSubject(id, "Math"));
    EXPECT_EQ(store.getById(id)->subjects.size(), 1u);
}

TEST(PeriodStoreTest, AddSubject_SameSubjectAllowedInDifferentPeriods) {
    PeriodStore store;
    const int a = store.addPeriod(Period{.name = "WS25"});
    const int b = store.addPeriod(Period{.name = "SS26"});

    EXPECT_TRUE(store.addSubject(a, "Math"));
    EXPECT_TRUE(store.addSubject(b, "Math"));
}

TEST(PeriodStoreTest, AddSubject_UnknownIdReturnsFalse) {
    PeriodStore store;
    EXPECT_FALSE(store.addSubject(99, "Math"));
}

// ---------- deleteSubject ----------

TEST(PeriodStoreTest, DeleteSubject_RemovesOnlyThatSubject) {
    PeriodStore store;
    const int id = store.addPeriod(Period{.name = "WS25", .subjects = {"Math", "OS", "CG"}});

    EXPECT_TRUE(store.deleteSubject(id, "OS"));
    EXPECT_EQ(store.getById(id)->subjects, (std::vector<std::string>{"Math", "CG"}));
}

TEST(PeriodStoreTest, DeleteSubject_MissingSubjectReturnsFalse) {
    PeriodStore store;
    const int id = store.addPeriod(Period{.name = "WS25", .subjects = {"Math"}});

    EXPECT_FALSE(store.deleteSubject(id, "OS"));
    EXPECT_EQ(store.getById(id)->subjects.size(), 1u);
}

TEST(PeriodStoreTest, DeleteSubject_UnknownIdReturnsFalse) {
    PeriodStore store;
    EXPECT_FALSE(store.deleteSubject(99, "Math"));
}

// ---------- deletePeriod ----------

TEST(PeriodStoreTest, DeletePeriod_RemovesPeriod) {
    PeriodStore store;
    const int a = store.addPeriod(Period{.name = "WS25"});
    const int b = store.addPeriod(Period{.name = "SS26"});

    EXPECT_TRUE(store.deletePeriod(a));
    EXPECT_EQ(store.getById(a), nullptr);
    EXPECT_NE(store.getById(b), nullptr);
    EXPECT_EQ(store.getAllPeriods().size(), 1u);
}

TEST(PeriodStoreTest, DeletePeriod_UnknownIdReturnsFalse) {
    PeriodStore store;
    store.addPeriod(Period{.name = "WS25"});

    EXPECT_FALSE(store.deletePeriod(99));
    EXPECT_EQ(store.getAllPeriods().size(), 1u);
}

TEST(PeriodStoreTest, DeletePeriod_IdsAreNotReused) {
    PeriodStore store;
    store.addPeriod(Period{.name = "A"});
    const int b = store.addPeriod(Period{.name = "B"});
    store.deletePeriod(b);

    EXPECT_EQ(store.addPeriod(Period{.name = "C"}), b + 1);
}
