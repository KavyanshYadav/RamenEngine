//
// Created by aufvim on 3/25/26.
//

#include <cstdarg>
#include <log/log.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace engine::log {
    static std::shared_ptr<spdlog::logger> s_logger;

    static spdlog::level::level_enum _ToSpdlogLevel(Level level) {
        switch (level) {
            case Level::Trace: return spdlog::level::trace;
            case Level::Debug: return spdlog::level::debug;
            case Level::Info:  return spdlog::level::info;
            case Level::Warning:  return spdlog::level::warn;
            case Level::Error: return spdlog::level::err;
            case Level::Fatal: return spdlog::level::critical;
            default:           return spdlog::level::info;
        }
    }
    void Init(Level min_level, const char* log_file, bool flush_on_write) {
        std::printf("[DEBUG] Init started...\n");

        if (s_logger) {
            std::printf("[DEBUG] Logger already initialized!\n");
            return;
        }

        // Since we disabled exceptions, we must check if spdlog fails manually
        std::vector<spdlog::sink_ptr> sinks;

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("%^[%T] [%l] %v%$");
        sinks.push_back(console_sink);
        std::printf("[DEBUG] Console sink created.\n");

        if (log_file && log_file[0] != '\0') {
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, true);
            file_sink->set_pattern("[%Y-%m-%d %T.%e] [%s:%#] [%l] %v");
            sinks.push_back(file_sink);
            std::printf("[DEBUG] File sink created.\n");
        }

        s_logger = std::make_shared<spdlog::logger>("Engine", sinks.begin(), sinks.end());

        spdlog::register_logger(s_logger);
        spdlog::set_default_logger(s_logger);

        s_logger->set_level(_ToSpdlogLevel(min_level));
        if (flush_on_write) {
            s_logger->flush_on(_ToSpdlogLevel(min_level));
        } else {
            s_logger->flush_on(spdlog::level::err);
        }

        std::printf("[DEBUG] Init finished successfully!\n");
    }
    void Shutdown() {
        spdlog::shutdown();
        s_logger.reset();

    }

    void Write(Level level, const char* file, int line, const char* fmt, ...) noexcept {
         // std::printf("[DEBUG] Write called with: %s\n", fmt);

        if (!s_logger) {
            std::printf("[DEBUG] s_logger is null! Init failed or wasn't called.\n");
            return;
        }

        spdlog::level::level_enum spd_level = _ToSpdlogLevel(level);

        if (!s_logger->should_log(spd_level)) {
            std::printf("[DEBUG] Log level too low to print.\n");
            return;
        }

        va_list args;
        va_start(args, fmt);

        va_list args_copy;
        va_copy(args_copy, args);
        int size = std::vsnprintf(nullptr, 0, fmt, args_copy);
        va_end(args_copy);

        // std::printf("[DEBUG] Calculated string size: %d\n", size);

        if (size > 0) {
            std::vector<char> buffer(static_cast<size_t>(size) + 1);
            std::vsnprintf(buffer.data(), buffer.size(), fmt, args);

            spdlog::source_loc loc{file, line, nullptr};
            s_logger->log(loc, spd_level, "{}", buffer.data());
        } else {
            // std::printf("[DEBUG] size was 0 or negative!\n");
        }

        va_end(args);
    }

}