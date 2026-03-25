#pragma once
#include <cstddef>
#include <cstdint>

#include "platform/platform.h"
#include "log/log.h"
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
        log::Level log_level = log::Level::Info;
        bool flush_on_write = true;
    };

    class Engine {
        public:
            static Engine& Get();
            [[nodiscard]] bool Init(const EngineConfig &cfg);
            void Shutdown();
            void RunFrame();


            bool IsRunning() const { return m_running; }

            platform::Platform& Platform() {return *m_platform;};

    private:
        Engine() = default;
        ~Engine() = default;
        // Engine(const Engine&) = delete;
        template<typename T, typename... Args>
    T* _PlaceInStorage(Args&&... args);


        bool _InitLogger(const EngineConfig& cfg);
        bool _InitPlatform(const EngineConfig& cfg);
        // bool _InitMemory(const EngineConfig& cfg);
        // bool _InitJobs(const EngineConfig& cfg);
        // bool _InitECS(const EngineConfig& cfg);
        // bool _InitAssets(const EngineConfig& cfg);

        platform::Platform* m_platform {nullptr};

        bool m_running {false};
        alignas(64) uint8_t m_subsystem_storage[8192];
        size_t m_storage_used{ 0 };
    };


}
