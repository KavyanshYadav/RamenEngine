#include <iostream>
#include <log/log.h> // Make sure this matches your actual include path
#include <Engine.h>
int main() {
    // 1. Setup Configuration [cite: 281-310]
    engine::EngineConfig cfg;
    cfg.window_title = "Ramen Engine - My First Window";
    cfg.window_width = 1280;
    cfg.window_height = 720;
    cfg.vsync = true;

    // Logger settings are now handled by the EngineConfig
    cfg.log_level = engine::log::Level::Debug;
    cfg.log_file = "engine.log";
    cfg.flush_on_write = true; // Great for debugging, forces flush on every write

    // 2. Get Engine Singleton and Initialize [cite: 315-317]
    auto& engine = engine::Engine::Get();

    if (!engine.Init(cfg)) {
        // If Init fails, the logger is usually already up, so we can log the failure.
        ENGINE_LOG_FATAL("Engine failed to initialize!");
        return -1;
    }
    ENGINE_LOG_INFO("Engine initialized!");

    ENGINE_LOG_INFO("Engine successfully initialized. Entering main loop.");

    // 3. Main Game Loop [cite: 319-321]
    while (engine.IsRunning()) {
        engine.RunFrame(); // Handles SDL events and bgfx rendering
    }

    ENGINE_LOG_INFO("Main loop exited. Shutting down engine.");

    // 4. Teardown [cite: 318]
    engine.Shutdown();

    return 0;
}