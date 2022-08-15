#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <memory>

namespace managers
{
    class LogManager {
        public:
            static void Initialize();
            static void Shutdown();

            static std::shared_ptr<spdlog::logger>& getCoreLogger() { return financial_data_logger; }

        private:
            static std::shared_ptr<spdlog::logger> financial_data_logger;
    };
}

#define CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME "financialdatalog"

#ifndef CPPFINANCIALDATA_CONFIG_RELEASE
#define CPPFINANCIALDATA_TRACE(...) SPDLOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__)
#define CPPFINANCIALDATA_DEBUG(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#define CPPFINANCIALDATA_INFO(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#define CPPFINANCIALDATA_WARN(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#define CPPFINANCIALDATA_ERROR(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)
#define CPPFINANCIALDATA_FATAL(...) SPDLOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__)

#else
// Disable logging for release builds
#define CPPFINANCIALDATA_TRACE(...) (void)0
#define CPPFINANCIALDATA_DEBUG(...) (void)0
#define CPPFINANCIALDATA_INFO(...)  (void)0
#define CPPFINANCIALDATA_WARN(...)   (void)0
#define CPPFINANCIALDATA_ERROR(...) (void)0
#define CPPFINANCIALDATA_FATAL(...) (void)0
#endif