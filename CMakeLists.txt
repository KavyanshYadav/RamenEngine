cmake_minimum_required(VERSION 4.1)
project(ENGINE)


# ────────────────────────────────────────────────────
# Global settings — set BEFORE any add_subdirectory()
# ────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)            # -std=c++20, not -std=gnu++20

# Export compile commands for clangd / tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type to Debug if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ────────────────────────────────────────────────────
# Load cmake helper modules
# ────────────────────────────────────────────────────
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/CompileFlags.cmake)   # ENGINE_WARNINGS, build-type cflags, -fno-exceptions
include(cmake/Sanitizer.cmake)      # ENGINE_ASAN / ENGINE_UBSAN options
include(cmake/EngineTarget.cmake)   # engine_add_module() helper macro


#include(FetchContent)
#
#
#FetchContent_Declare(
#        SDL2
#        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#        GIT_TAG        release-2.30.2
#        GIT_SHALLOW    TRUE
#)
#set(SDL_STATIC ON  CACHE BOOL "" FORCE)
#set(SDL_SHARED OFF CACHE BOOL "" FORCE)
#
#FetchContent_Declare(
#        bgfx
#        GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
#        GIT_TAG        master
#        GIT_SHALLOW    TRUE
#)
#set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#
#FetchContent_MakeAvailable(SDL2)
#FetchContent_MakeAvailable(bgfx)
add_subdirectory(third_party)
add_subdirectory(engine/core)
# Define the executable
add_executable(RamenEngine src/main.cpp)

# Link the engine_core library
# This automatically handles include paths and dependencies (SDL2, bgfx)
# because of the PUBLIC/PRIVATE keywords in your macro.
target_link_libraries(RamenEngine PRIVATE engine_core)

# If your main.cpp needs to find headers in src/ or include/
target_include_directories(RamenEngine PRIVATE src include)