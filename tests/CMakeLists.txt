include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(EntryStoreTest EntryStoreTest.cpp)
add_executable(PersistenceTest PersistenceTest.cpp)

target_link_libraries(EntryStoreTest PRIVATE StudyTrackerLib GTest::gtest_main)
target_link_libraries(PersistenceTest PRIVATE StudyTrackerLib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(PersistenceTest)
gtest_discover_tests(EntryStoreTest)