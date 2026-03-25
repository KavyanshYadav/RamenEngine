// core/src/Engine.cpp
#include <Engine.h>
#include <log/log.h>
#include <platform/platform.h>
#include <memory>

namespace engine {

Engine& Engine::Get() {
    static Engine s_instance;
    return s_instance;
}

// Placement-new into static storage to avoid early heap allocations [cite: 380-388]
template<typename T, typename... Args>
T* Engine::_PlaceInStorage(Args&&... args) {
    constexpr size_t sz = sizeof(T);
    constexpr size_t align = alignof(T);

    size_t space = sizeof(m_subsystem_storage) - m_storage_used;
    void* ptr = m_subsystem_storage + m_storage_used;

    ptr = std::align(align, sz, ptr, space);
    // ENGINE_ASSERT(ptr != nullptr, "Subsystem storage exhausted");

    m_storage_used = sizeof(m_subsystem_storage) - space + sz;
    return new(ptr) T(std::forward<Args>(args)...);
}

bool Engine::Init(const EngineConfig& cfg) {
    if (m_running) return true;

    // 1. Logger MUST be first
    if (!_InitLogger(cfg)) return false;

    // 2. Platform
    if (!_InitPlatform(cfg)) return false;

    m_running = true;
    return true;
}

void Engine::Shutdown() {
    if (!m_running) return;

    // Teardown occurs in reverse order of initialization
    if (m_platform) {
        m_platform->Shutdown();
        m_platform->~Platform();
        m_platform = nullptr;
    }

    log::Shutdown();

    m_storage_used = 0;
    m_running = false;
}

void Engine::RunFrame() {
    m_platform->BeginFrame();
    m_platform->EndFrame();
}

bool Engine::_InitLogger(const EngineConfig& cfg) {
    log::Init(cfg.log_level, cfg.log_file, cfg.flush_on_write);
    return true;
}

bool Engine::_InitPlatform(const EngineConfig& cfg) {
    m_platform = _PlaceInStorage<platform::Platform>();

    platform::PlatformConfig plat_cfg;
    plat_cfg.width = cfg.window_width;
    plat_cfg.height = cfg.window_height;
    plat_cfg.window_title = cfg.window_title;
    plat_cfg.vsync = cfg.vsync;

    return m_platform->Init(plat_cfg);
}

} // namespace engine