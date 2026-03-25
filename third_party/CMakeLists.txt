include(FetchContent)

# ── SDL2 ─────────────────────────────────────────────────────────────────────
set(SDL_STATIC ON  CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_TEST   OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-2.30.2
        GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(SDL2)

# Ensure the engine-expected target exists
if(TARGET SDL2-static AND NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 ALIAS SDL2-static)
endif()

# ── bgfx Family ──────────────────────────────────────────────────────────────
# bgfx.cmake requires bx and bimg to be present.
# We download the 3 core repos + the cmake wrapper.
# 1. Declare all the pieces
FetchContent_Declare(bx   GIT_REPOSITORY https://github.com/bkaradzic/bx.git   GIT_TAG master GIT_SHALLOW TRUE)
FetchContent_Declare(bimg GIT_REPOSITORY https://github.com/bkaradzic/bimg.git GIT_TAG master GIT_SHALLOW TRUE)
FetchContent_Declare(bgfx GIT_REPOSITORY https://github.com/bkaradzic/bgfx.git GIT_TAG master GIT_SHALLOW TRUE)

# 2. Populate them without the deprecated function
# This downloads them and sets the _SOURCE_DIR variables
FetchContent_MakeAvailable(bx bimg bgfx)

# 3. Declare the wrapper
FetchContent_Declare(
        bgfx_cmake
        GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
        GIT_TAG        master
        GIT_SHALLOW    TRUE
)

# 4. Set the paths for the wrapper using the variables created in step 2
# FetchContent defines [name]_SOURCE_DIR automatically after MakeAvailable
set(BX_DIR   "${bx_SOURCE_DIR}"   CACHE PATH "" FORCE)
set(BIMG_DIR "${bimg_SOURCE_DIR}" CACHE PATH "" FORCE)
set(BGFX_DIR "${bgfx_SOURCE_DIR}" CACHE PATH "" FORCE)

set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS    OFF CACHE BOOL "" FORCE)

# 5. Finally, make the wrapper available
FetchContent_MakeAvailable(bgfx_cmake)

# ── toml++ ───────────────────────────────────────────────────────────────────
FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(tomlplusplus)

# ── spdlog ───────────────────────────────────────────────────────────────────
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.13.0 # Or 'master' for latest
        GIT_SHALLOW    TRUE
)

# We don't need to build spdlog's examples or tests
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(spdlog)
# ── Catch2 ───────────────────────────────────────────────────────────────────
if(ENGINE_BUILD_TESTS)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)
endif()

