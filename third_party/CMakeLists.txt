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


# ── fmt ───────────────────────────────────────────────────────────────────

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        11.1.4  # A recent, stable, C++20-compliant release
        GIT_SHALLOW    TRUE
)
set(FMT_OS OFF CACHE BOOL "Disable fmt os module" FORCE)
set(FMT_TEST OFF CACHE BOOL "Disable fmt tests" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "Disable fmt install" FORCE)
set(FMT_USE_EXCEPTIONS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(fmt)
if(TARGET fmt)
    target_compile_options(fmt PRIVATE
            -fexceptions
            -frtti
    )
endif()


# ── spdlog ───────────────────────────────────────────────────────────────────
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.17.0 # Or 'master' for latest
        GIT_SHALLOW    TRUE
)
set(SPDLOG_FMT_EXTERNAL   ON  CACHE BOOL "" FORCE)
set(SPDLOG_NO_EXCEPTIONS  ON  CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL        OFF CACHE BOOL "" FORCE)
# We don't need to build spdlog's examples or tests
FetchContent_MakeAvailable(spdlog)
if(TARGET spdlog)
    target_compile_options(spdlog PRIVATE
            -fexceptions
            -frtti
            -Wno-error -Wno-deprecated-literal-operator
    )
endif()
# ── Catch2 ───────────────────────────────────────────────────────────────────
if(ENGINE_BUILD_TESTS)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.5.2
    )
    FetchContent_MakeAvailable(Catch2)
endif()

