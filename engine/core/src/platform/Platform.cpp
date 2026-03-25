//
// Created by aufvim on 3/25/26.
//

#include "log/log.h"
#include "platform/platform.h"
#include <Engine.h>


//PRIVATE API
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>


namespace engine::platform {
    bool Platform::Init(const PlatformConfig& cfg) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
            ENGINE_LOG_ERROR("SDL_Init failed: %s", SDL_GetError());
            return false;
        }
        SDL_Window* window = SDL_CreateWindow(
            cfg.window_title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        cfg.width, cfg.height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
        if (!window) {
            ENGINE_LOG_ERROR("SDL_CreateWindow failed: %s", SDL_GetError());
            return false;
        }
        m_window = window;

        SDL_SysWMinfo wm_info;
        SDL_VERSION(&wm_info.version);
        SDL_GetWindowWMInfo(window, &wm_info);

        bgfx::PlatformData pd{};
        switch (wm_info.subsystem) {
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
            case SDL_SYSWM_WINDOWS:
                pd.nwh = wm_info.info.win.window;
                break;
#endif
#if defined(SDL_VIDEO_DRIVER_X11)
            case SDL_SYSWM_X11:
                pd.ndt = wm_info.info.x11.display;
                pd.nwh = (void*)(uintptr_t)wm_info.info.x11.window;
                break;
#endif
#if defined(SDL_VIDEO_DRIVER_WAYLAND)
            case SDL_SYSWM_WAYLAND:
                pd.ndt = wm_info.info.wl.display;
                pd.nwh = wm_info.info.wl.surface;

                // Explicitly tell bgfx we are using Wayland (required for modern bgfx)
                pd.type = bgfx::NativeWindowHandleType::Wayland;
                break;
#endif
            default:
                ENGINE_LOG_FATAL("Unsupported windowing subsystem!");
                return false;
        }

        bgfx::setPlatformData(pd);
        bgfx::Init init;
        init.type = bgfx::RendererType::Count; // auto-select best API
        init.resolution.width = cfg.width;
        init.resolution.height = cfg.height;

        // Setup vsync and MSAA defaults
        init.resolution.reset = cfg.vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE;
        init.resolution.reset |= BGFX_RESET_MSAA_X4; // 4x MSAA by default [cite: 1371]

        init.platformData = pd;
        bgfx::renderFrame();
        if (!bgfx::init(init)) {
            ENGINE_LOG_ERROR("bgfx::init failed");
            return false;
        }

        ENGINE_LOG_INFO("bgfx initialized. Backend: %s", bgfx::getRendererName(bgfx::getRendererType()));
        m_width = cfg.width;
        m_height = cfg.height;
        m_initialized = true;

        return true;

    }
    void Platform::Shutdown() {
        if (!m_initialized) return;

        // bgfx teardown must happen before SDL tears down the window
        bgfx::shutdown();

        if (m_window) {
            SDL_DestroyWindow(static_cast<SDL_Window*>(m_window));
            m_window = nullptr;
        }

        SDL_Quit();
        m_initialized = false;
    }

    void Platform::BeginFrame() {
        // Poll SDL events - translate to engine input events [cite: 1385-1387]
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                // Signal the engine to stop its main loop [cite: 1388-1389]
                // Note: Ensure your Engine singleton implements RequestShutdown() or modifies IsRunning() accordingly.
                engine::Engine::Get().Shutdown();
            }

            // Pass the event address as an opaque void* to hide SDL from the header [cite: 1390]
            _TranslateSDLEvent(&ev);
        }
    }

    void Platform::EndFrame() {
        // bgfx::frame() advances the frame, presents the back buffer. [cite: 1396-1398]
        // Returns the frame number useful for GPU timing.
        bgfx::frame();
    }

    void Platform::_TranslateSDLEvent(void* ev) {
        // Cast back to SDL_Event to process platform-specific inputs
        SDL_Event* sdl_ev = static_cast<SDL_Event*>(ev);

        // E.g., Window resize event handling
        if (sdl_ev->type == SDL_WINDOWEVENT) {
            if (sdl_ev->window.event == SDL_WINDOWEVENT_RESIZED) {
                m_width = static_cast<uint32_t>(sdl_ev->window.data1);
                m_height = static_cast<uint32_t>(sdl_ev->window.data2);

                // Inform bgfx of the new backbuffer size
                uint32_t reset_flags = BGFX_RESET_MSAA_X4; // Re-apply existing flags
                bgfx::reset(m_width, m_height, reset_flags);
            }
        }
    }
}
