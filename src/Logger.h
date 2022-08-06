#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <memory>

#define CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME "datalogger"

#ifndef CPPFINANCIALDATA_CONFIG_RELEASE
#define CPPFINANCIALDATA_TRACE(...) if (spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME) != nullptr) {spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME)->trace(__VA_ARGS__);}
#define CPPFINANCIALDATA_DEBUG(...) if (spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME) != nullptr) {spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME)->trace(__VA_ARGS__);}
#define CPPFINANCIALDATA_INFO(...) if (spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME) != nullptr) {spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME)->trace(__VA_ARGS__);}
#define CPPFINANCIALDATA_WARN(...) if (spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME) != nullptr) {spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME)->trace(__VA_ARGS__);}
#define CPPFINANCIALDATA_ERROR(...) if (spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME) != nullptr) {spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME)->trace(__VA_ARGS__);}
#define CPPFINANCIALDATA_FATAL(...) if (spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME) != nullptr) {spdlog::get(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME)->trace(__VA_ARGS__);}

#else
// Disable logging for release builds
#define CPPFINANCIALDATA_TRACE(...) (void)0
#define CPPFINANCIALDATA_DEBUG(...) (void)0
#define CPPFINANCIALDATA_INFO(...)  (void)0
#define CPPFINANCIALDATA_WARN(...)   (void)0
#define CPPFINANCIALDATA_ERROR(...) (void)0
#define CPPFINANCIALDATA_FATAL(...) (void)0
#endif

namespace managers
{
    class LogManager {
        public:
            LogManager() = default;
            ~LogManager() = default;

            void Initialize() {
                auto consoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
                consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] %v%$");
                auto fileLogger = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../logs/logs.txt", true);
                fileLogger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] [%@] %v");

                std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileLogger };
                auto logger = std::make_shared<spdlog::logger>(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME, sinks.begin(), sinks.end());
                logger->set_level(spdlog::level::trace);
                logger->flush_on(spdlog::level::trace);
                spdlog::register_logger(logger);
            }
            void Shutdown() {
                spdlog::shutdown();
            }
    };
}