#pragma  once
#include <cstdint>

namespace engine::platform {

    struct PlatformConfig {
        uint32_t width {1920};
        uint32_t height {1080};
        const char* window_title {"Engine"};
        bool vsync {false};
    };

    class Platform {
        public:
            explicit Platform() = default;
            ~Platform() = default;

            Platform(const Platform&) = delete;
            Platform& operator=(const Platform&) = delete;
            Platform(Platform&&) noexcept = default;
            Platform& operator=(Platform&&) noexcept = default;

            void BeginFrame();
            void EndFrame();

            [[nodiscard]] bool Init(const PlatformConfig& cfg); // Must check return value
            void Shutdown();
        private:
            void _TranslateSDLEvent(void* ev);
            void* m_window{ nullptr };      // 8 bytes (Opaque pointer to SDL_Window)
            uint32_t m_width{ 0 };             // 4 bytes [cite: 1381]
            uint32_t m_height{ 0 };            // 4 bytes [cite: 1382]
            bool     m_initialized{ false };
    };

}