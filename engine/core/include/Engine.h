#pragma once
#include <cstddef>
#include <cstdint>

namespace engine {
    struct  EngineConfig {
        size_t frame_arena_size_mb = 64;
        size_t asset_arena_size_mb= 512;
        size_t general_heap_size_mb = 256;
        // ECS
        uint32_t max_entities= 1 << 20; // 1M
        uint32_t max_archetypes= 4096;
        // Jobs
        uint32_t worker_thread_count = 0; // 0 = hardware_concurrency() - 1
        // Platform (bgfx)
        uint32_t window_width= 1920;
        uint32_t window_height= 1080;
        const char* window_title= "Engine";
        bool vsync= false;
        // Logging
        // log::Level log_level= log::Level::Debug;
        const char* log_file= "engine.log";
    };



}