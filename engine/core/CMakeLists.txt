# core/CMakeLists.txt
# ─────────────────────────────────────────────────────────────────────────────
# Builds the engine_core static library.
# All subsystems in the core layer compile into a single .a / .lib.
# ─────────────────────────────────────────────────────────────────────────────

engine_add_module(
        NAME engine_core
        SOURCES
        src/platform/Platform.cpp
#        # ── Logging (must be first — all others call ENGINE_LOG_*) ──────────
#        src/log/Logger.cpp
#
#        # ── Memory ──────────────────────────────────────────────────────────
#        src/memory/ArenaAllocator.cpp
#        src/memory/MemorySystem.cpp
#
#        # ── String pool ─────────────────────────────────────────────────────
#        src/string/StringId.cpp
#
#        # ── Platform (bgfx + SDL2 window) ───────────────────────────────────
#        src/platform/Platform.cpp
#
#        # ── Job system ──────────────────────────────────────────────────────
#        src/jobs/JobScheduler.cpp
#
#        # ── Event bus ───────────────────────────────────────────────────────
#        src/events/EventBus.cpp
#
#        # ── ECS ─────────────────────────────────────────────────────────────
#        src/ecs/World.cpp
#        src/ecs/Archetype.cpp
#
#        # ── Asset system (core layer only) ───────────────────────────────────
#        src/assets/AssetSystem.cpp
#
#        # ── Engine root ─────────────────────────────────────────────────────
#        src/Engine.cpp

        PUBLIC_DEPS
        # bx types are part of our public API (IAllocator inherits bx::AllocatorI)
        bx
        tomlplusplus

        PRIVATE_DEPS
        # bgfx + SDL2 are PRIVATE — consumers never include bgfx/SDL2 headers directly
        bgfx
        SDL2::SDL2
)

# ── Platform-specific link requirements ──────────────────────────────────────
if(WIN32)
    target_link_libraries(engine_core PRIVATE
            User32 Gdi32 Shell32 Ole32 Advapi32
    )
elseif(APPLE)
    find_library(COCOA_LIB Cocoa REQUIRED)
    find_library(METAL_LIB Metal REQUIRED)
    find_library(QUARTZCORE_LIB QuartzCore REQUIRED)
    target_link_libraries(engine_core PRIVATE
            ${COCOA_LIB} ${METAL_LIB} ${QUARTZCORE_LIB}
    )
elseif(UNIX)
    target_link_libraries(engine_core PRIVATE
            pthread dl X11
    )
endif()

# ── Unit tests (only when ENGINE_BUILD_TESTS is ON) ──────────────────────────
if(ENGINE_BUILD_TESTS AND TARGET Catch2::Catch2WithMain)
    add_subdirectory(tests)
endif()