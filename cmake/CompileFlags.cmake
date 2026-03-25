# cmake/CompilerFlags.cmake
# ─────────────────────────────────────────────────────────────────────────────
# Compiler warning / optimization flags.
# Included by root CMakeLists.txt BEFORE any targets are defined.
# ─────────────────────────────────────────────────────────────────────────────

# ── Warning flags ────────────────────────────────────────────────────────────
if(MSVC)
    set(ENGINE_WARNINGS
            /W4
            /WX              # warnings as errors
            /wd4100          # unreferenced formal parameter (too noisy w/ callbacks)
            /wd4505          # unreferenced local function (template heavy code)
    )
else()
    set(ENGINE_WARNINGS
            -Wall
            -Wextra
            -Wpedantic
            -Werror                 # warnings as errors in CI; remove locally if annoying
            -Wno-unused-parameter   # too noisy with interface callbacks
            -Wconversion            # implicit narrowing casts
            -Wshadow                # variable shadowing
            -Wnon-virtual-dtor      # polymorphic class without virtual dtor
            -Wold-style-cast
            -Woverloaded-virtual
    )
endif()

# ── No exceptions / no RTTI ──────────────────────────────────────────────────
# Required for engine core. Eliminates exception-handling overhead and
# prevents accidental use of throw in hot paths.
if(NOT MSVC)
    add_compile_options(-fno-exceptions -fno-rtti)
else()
    add_compile_options(/EHs- /EHc- /GR-)
endif()

# ── Per-config optimization flags ────────────────────────────────────────────
if(NOT MSVC)
    # Debug — full debug info, no opt, define ENGINE_DEBUG
    set(CMAKE_CXX_FLAGS_DEBUG           "-O0 -g3 -DENGINE_DEBUG")

    # Release — max opt, no debug info, LTO-friendly
    set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG -march=native")

    # RelWithDebInfo — used for profiling with Tracy
    # ENGINE_PROFILING_ENABLED enables Tracy macros (see Profiler.h)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g1 -DENGINE_PROFILING_ENABLED")
endif()

# ── Useful global defines ─────────────────────────────────────────────────────
# These are defined on all targets that link engine_core (via INTERFACE props).
# Do NOT add renderer / platform specifics here.
add_compile_definitions(
        $<$<CONFIG:Debug>:ENGINE_DEBUG>
        $<$<CONFIG:RelWithDebInfo>:ENGINE_PROFILING_ENABLED>
)