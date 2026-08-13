cmake_minimum_required(VERSION 4.0)
project(StudyTracker)

set(CMAKE_CXX_STANDARD 20)

# ----- GUI dependencies -----
find_package(OpenGL REQUIRED)
include(FetchContent)
FetchContent_Declare(
        SDL2
        URL https://github.com/libsdl-org/SDL/releases/download/release-2.32.0/SDL2-2.32.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(SDL2)

# ----- Dear IMGUI as own static lib ------
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui)
add_library(imgui STATIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)
target_link_libraries(imgui PUBLIC SDL2::SDL2 OpenGL::GL)


# ----- third-party headers -----
add_library(third_party INTERFACE)
target_include_directories(third_party INTERFACE include)

# ----- logic layer -----
add_library(StudyTrackerLib
        src/Entry.h
        src/EntryStore.cpp
        src/EntryStore.h
        src/persistence.cpp
        src/persistence.h
        src/App.cpp
        src/App.h)
target_include_directories(StudyTrackerLib PUBLIC src)
target_link_libraries(StudyTrackerLib PUBLIC third_party)


# ------ GUI executable -------
add_executable(StudyTracker src/main.cpp)
target_link_libraries(StudyTracker PRIVATE
        StudyTrackerLib
        imgui)


# on windows SDL2.dll needs to be copied next to the exe
add_custom_command(TARGET StudyTracker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2> $<TARGET_FILE_DIR:StudyTracker>)

# ------ tests --------
enable_testing()
add_subdirectory(tests)