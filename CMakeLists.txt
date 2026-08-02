cmake_minimum_required(VERSION 4.0)
project(StudyTracker)

set(CMAKE_CXX_STANDARD 20)

add_executable(StudyTracker src/main.cpp
        src/Entry.h
        src/EntryStore.cpp
        src/EntryStore.h)
