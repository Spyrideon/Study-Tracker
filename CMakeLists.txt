cmake_minimum_required(VERSION 4.0)
project(StudyTracker)

set(CMAKE_CXX_STANDARD 20)

add_library(third_party INTERFACE)
target_include_directories(third_party INTERFACE include)

add_library(StudyTrackerLib
        src/Entry.h
        src/EntryStore.cpp
        src/EntryStore.h)
target_include_directories(StudyTrackerLib PUBLIC src)

add_executable(StudyTracker src/main.cpp)

target_link_libraries(StudyTrackerLib PUBLIC third_party)
target_link_libraries(StudyTracker PRIVATE StudyTrackerLib)

enable_testing()
add_subdirectory(tests)