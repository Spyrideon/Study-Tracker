# Study Tracker
# Introduction
This is a personal project, which originated during my exam period.\
I wanted to track the times I spent studying on different subjects but could not find software that was both free to use and seemingly fit for my purpose and preferences.

# Goal
The goal for this piece of software is a simple working study tracker, \
with the tools to document and analyze the time that was spent on studying.
The software should not be heavy or unintuitive to use and also easily adjustable for the needs of other use cases or preferences.
## Building and Running

StudyTracker is a C++20 application built with CMake. SDL2 and GoogleTest are
downloaded automatically during the first configure step, so an internet
connection is required the first time you build. Dear ImGui (docking branch),
ImGuiDatePicker, `imgui_stdlib`, and `json.hpp` are bundled in the repository —
no separate download needed.

### Prerequisites (all platforms)

- **Git**
- **CMake ≥ 4.0**
- A **C++20 toolchain with `std::format` and `std::chrono` time-zone support**.
  The app formats dates and calls `std::chrono::current_zone()`, which need a
  recent standard library:
    - **GCC ≥ 14**, or
    - **Clang ≥ 17** with libc++ ≥ 17, or
    - **MSVC 2022** (Visual Studio 17.4+ / `_MSC_VER` ≥ 1929)
- **OpenGL** development libraries
> SDL2 is built from source by CMake, so on Linux you additionally need SDL2's
> video build dependencies (X11 / Wayland headers). These are listed per-OS
> below.
 
---

### 1. Install the toolchain and system dependencies

#### Windows (MSYS2 / MinGW-w64)

Install [MSYS2](https://www.msys2.org/), then from the **MSYS2 MinGW64** shell:

```bash
pacman -S --needed \
    mingw-w64-x86_64-gcc \
    mingw-w64-x86_64-cmake \
    mingw-w64-x86_64-ninja \
    git
```

OpenGL is provided by the MinGW runtime. Build and run from the **MinGW64**
shell so the compiler and libraries are on your `PATH`.

*(Alternatively, Visual Studio 2022 with the "Desktop development with C++"
workload also works — open the folder and let its CMake integration configure
the project.)*

#### Linux (Debian / Ubuntu)

```bash
sudo apt update
sudo apt install -y \
    build-essential cmake ninja-build git \
    libgl1-mesa-dev \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \
    libxinerama-dev libxss-dev libwayland-dev libxkbcommon-dev \
    wayland-protocols libegl1-mesa-dev libasound2-dev libpulse-dev
```

The last two rows are SDL2's build dependencies; see the
[SDL2 Linux docs](https://wiki.libsdl.org/SDL2/README/linux) for the complete
list if a video backend is missing.

**Compiler version matters:** `std::format` and the `<chrono>` time-zone
database need **GCC ≥ 14**. If `g++ --version` reports an older release
(Ubuntu 22.04 ships GCC 11), install a newer one and point CMake at it:

```bash
sudo apt install -y gcc-14 g++-14
# then add to the configure command in step 3:
#   -DCMAKE_C_COMPILER=gcc-14 -DCMAKE_CXX_COMPILER=g++-14
```

#### macOS

```bash
brew install cmake ninja llvm
```

OpenGL is provided by the system (the deprecated-but-present OpenGL framework).

**Important:** the version of Clang shipped with Xcode / Command Line Tools may
not fully support `std::format` or `std::chrono::current_zone()`. If the build
fails with errors about either, use Homebrew's LLVM by adding these to the
configure command in step 3:

```bash
-DCMAKE_C_COMPILER=$(brew --prefix llvm)/bin/clang
-DCMAKE_CXX_COMPILER=$(brew --prefix llvm)/bin/clang++
```

If the link step then can't find libc++, also add:

```bash
-DCMAKE_EXE_LINKER_FLAGS="-L$(brew --prefix llvm)/lib/c++ -Wl,-rpath,$(brew --prefix llvm)/lib/c++"
```
 
---

### 2. Clone the repository

```bash
git clone <your-repo-url>
cd StudyTracker
```
 
---

### 3. Configure and build

The same two commands work on every platform (add any compiler overrides from
step 1):

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The first `cmake -B build` downloads SDL2 and GoogleTest, so it takes a little
longer and needs internet. To use Ninja explicitly, add `-G Ninja` to the
configure command.
 
---

### 4. Run

The executable location depends on the generator:

- **Single-config** (Ninja, Makefiles — the default on Linux/macOS and MinGW):
```bash
  ./build/StudyTracker           # Windows: build\StudyTracker.exe
```
- **Multi-config** (Visual Studio, Xcode):
```bash
  ./build/Release/StudyTracker   # or Debug/
```

On Windows, `SDL2.dll` is copied next to the executable automatically after the
build.

> **Working directory:** StudyTracker reads and writes `test.json` (your saved
> entries) and `imgui.ini` (window layout) in the **current working directory**.
> Run it from the folder where you want those files to live. Launching from a
> different directory will start with an empty list and a default layout.
 
---

### 5. Running the tests (optional)

The project includes GoogleTest-based unit tests, built automatically. Run them
with:

```bash
ctest --test-dir build --output-on-failure
```


## Acknowledgements
* [JSON for C++ by nlohmann](https://github.com/nlohmann/json)
* [Dear ImGui by ocornut](https://github.com/ocornut/imgui)
