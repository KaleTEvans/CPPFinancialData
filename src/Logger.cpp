#include "FinancialData/Logger.h"

namespace managers
{
    std::shared_ptr<spdlog::logger> LogManager::financial_data_logger;

    void LogManager::Initialize() {
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S] %^[ src: %s ] [ function: %! ] [ line: %# ] [ %l ]%$ %v");
        auto fileLogger = std::make_shared<spdlog::sinks::basic_file_sink_mt>("../../logs/logs.txt");
        fileLogger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] [%@] %v");

        std::vector<spdlog::sink_ptr> sinks{ consoleSink, fileLogger };
        financial_data_logger = std::make_shared<spdlog::logger>(CPPFINANCIALDATA_DEFAULT_LOGGGER_NAME, sinks.begin(), sinks.end());
        financial_data_logger->set_level(spdlog::level::trace);
        financial_data_logger->flush_on(spdlog::level::trace);
        spdlog::register_logger(financial_data_logger);
        spdlog::set_default_logger(financial_data_logger);
    }

    void LogManager::Shutdown() {
        spdlog::shutdown();
    }
}
