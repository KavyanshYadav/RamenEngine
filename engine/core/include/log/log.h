#pragma once

#include <cstdint>

#if defined(_MSC_VER)
    #define ENGINE_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) || defined(__GNUC__)
    #define ENGINE_DEBUGBREAK() __builtin_trap()
#else
    #include <csignal>
    #define ENGINE_DEBUGBREAK() raise(SIGTRAP)
#endif


namespace engine::log {
    enum class Level : uint8_t{Trace=0, Debug=1, Info=2, Warning=3, Error=4, Fatal=5 };

    void Init (Level min_level , const char* log_file, bool flush_on_write);
    void Shutdown ();

#if defined(__clang__) || defined(__GNUC__)
    void Write(Level level, const char* file, int line, const char* fmt, ...) noexcept __attribute__((format(printf, 4, 5)));
#else
    void Write(Level level, const char* file, int line, const char* fmt, ...) noexcept;
#endif
#define ENGINE_LOG_TRACE(fmt, ...) \
engine::log::Write(engine::log::Level::Trace, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define ENGINE_LOG_DEBUG(fmt, ...) \
engine::log::Write(engine::log::Level::Debug, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define ENGINE_LOG_INFO(fmt, ...) \
engine::log::Write(engine::log::Level::Info, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define ENGINE_LOG_WARN(fmt, ...) \
engine::log::Write(engine::log::Level::Warn, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define ENGINE_LOG_ERROR(fmt, ...) \
engine::log::Write(engine::log::Level::Error, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define ENGINE_LOG_FATAL(fmt, ...) \
do { \
engine::log::Write(engine::log::Level::Fatal, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
ENGINE_DEBUGBREAK(); \
std::abort(); \
} while(0)

}