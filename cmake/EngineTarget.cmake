# cmake/EngineTargets.cmake
# ─────────────────────────────────────────────────────────────────────────────
# Helper macros for defining engine module targets consistently.
# ─────────────────────────────────────────────────────────────────────────────

# engine_add_module(
#   NAME      <target_name>         # e.g. engine_core
#   SOURCES   file1.cpp file2.cpp   # relative to calling CMakeLists.txt
#   PUBLIC_DEPS  dep1 dep2          # propagated to consumers
#   PRIVATE_DEPS dep3 dep4          # NOT propagated
# )
#
# Creates a STATIC library with:
#   - include/<target_name>/ as public include dir
#   - ENGINE_WARNINGS applied as PRIVATE compile options
#   - C++20 required on PUBLIC interface
macro(engine_add_module)
    cmake_parse_arguments(
            ARG
            ""
            "NAME"
            "SOURCES;PUBLIC_DEPS;PRIVATE_DEPS"
            ${ARGN}
    )

    add_library(${ARG_NAME} STATIC ${ARG_SOURCES})

    target_include_directories(${ARG_NAME}
            PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    target_compile_features(${ARG_NAME} PUBLIC cxx_std_20)

    # Apply warning flags as PRIVATE — consumers don't inherit our warnings
    target_compile_options(${ARG_NAME} PRIVATE ${ENGINE_WARNINGS})

    if(ARG_PUBLIC_DEPS)
        target_link_libraries(${ARG_NAME} PUBLIC ${ARG_PUBLIC_DEPS})
    endif()

    if(ARG_PRIVATE_DEPS)
        target_link_libraries(${ARG_NAME} PRIVATE ${ARG_PRIVATE_DEPS})
    endif()
endmacro()


# engine_add_test(
#   NAME    <test_name>          # e.g. test_arena_allocator
#   SOURCES file.cpp             # relative to calling CMakeLists.txt
#   DEPS    engine_core Catch2   # targets to link
# )
macro(engine_add_test)
    cmake_parse_arguments(ARG "" "NAME" "SOURCES;DEPS" ${ARGN})

    add_executable(${ARG_NAME} ${ARG_SOURCES})

    target_compile_features(${ARG_NAME} PRIVATE cxx_std_20)
    target_compile_options(${ARG_NAME}  PRIVATE ${ENGINE_WARNINGS})

    if(ARG_DEPS)
        target_link_libraries(${ARG_NAME} PRIVATE ${ARG_DEPS})
    endif()

    # Register with CTest
    add_test(NAME ${ARG_NAME} COMMAND ${ARG_NAME})
endmacro()