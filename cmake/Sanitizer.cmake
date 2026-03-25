# cmake/Sanitizers.cmake
# ─────────────────────────────────────────────────────────────────────────────
# ASAN / UBSAN / TSAN toggle options.
# Usage:  cmake -DENGINE_ASAN=ON -DENGINE_UBSAN=ON ..
# ─────────────────────────────────────────────────────────────────────────────

option(ENGINE_ASAN  "Enable AddressSanitizer"           OFF)
option(ENGINE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENGINE_TSAN  "Enable ThreadSanitizer"            OFF)

if(ENGINE_ASAN AND ENGINE_TSAN)
    message(FATAL_ERROR "ASAN and TSAN cannot be enabled simultaneously.")
endif()

if(ENGINE_ASAN)
    message(STATUS "[Sanitizers] AddressSanitizer ENABLED")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options   (-fsanitize=address)
endif()

if(ENGINE_UBSAN)
    message(STATUS "[Sanitizers] UBSanitizer ENABLED")
    add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
    add_link_options   (-fsanitize=undefined)
endif()

if(ENGINE_TSAN)
    message(STATUS "[Sanitizers] ThreadSanitizer ENABLED")
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
    add_link_options   (-fsanitize=thread)
endif()