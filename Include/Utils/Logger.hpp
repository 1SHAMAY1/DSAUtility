#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>

namespace dsa {
namespace utils {

/**
 * @brief Log levels enumeration
 */
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    CRITICAL = 4
};

/**
 * @brief Logger utility class with multiple output streams and log levels
 */
class Logger {
private:
    std::string logger_name;
    LogLevel current_level;
    std::ostream* console_output;
    std::ofstream file_output;
    bool file_enabled;
    bool console_enabled;
    std::mutex log_mutex;
    
    // Static instance for global logging
    static std::unique_ptr<Logger> global_logger;
    static std::mutex global_mutex;

    // Helper function to get current timestamp
    std::string getTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    // Helper function to get log level string
    std::string getLevelString(LogLevel level) const {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }

    // Helper function to get colored log level string (for console)
    std::string getColoredLevelString(LogLevel level) const {
        switch (level) {
            case LogLevel::DEBUG: return "\033[36mDEBUG\033[0m";    // Cyan
            case LogLevel::INFO: return "\033[32mINFO\033[0m";      // Green
            case LogLevel::WARNING: return "\033[33mWARNING\033[0m"; // Yellow
            case LogLevel::ERROR: return "\033[31mERROR\033[0m";     // Red
            case LogLevel::CRITICAL: return "\033[35mCRITICAL\033[0m"; // Magenta
            default: return "\033[37mUNKNOWN\033[0m";               // White
        }
    }

    // Helper function to format log message
    std::string formatMessage(LogLevel level, const std::string& message, bool colored = false) const {
        std::stringstream ss;
        ss << "[" << getTimestamp() << "] ";
        ss << "[" << (colored ? getColoredLevelString(level) : getLevelString(level)) << "] ";
        ss << "[" << logger_name << "] ";
        ss << "[" << std::this_thread::get_id() << "] ";
        ss << message;
        return ss.str();
    }
    
    // Helper function to build message from arguments
    template<typename... Args>
    void buildMessage(std::stringstream& ss, const Args&... args) {
        (ss << ... << args);
    }
    
    template<typename T>
    void buildMessage(std::stringstream& ss, const T& arg) {
        ss << arg;
    }

public:
    Logger(const std::string& name = "DSAUtility", LogLevel level = LogLevel::INFO)
        : logger_name(name), current_level(level), console_output(&std::cout),
          file_enabled(false), console_enabled(true) {}

    ~Logger() {
        if (file_output.is_open()) {
            file_output.close();
        }
    }

    // Set log level
    void setLogLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(log_mutex);
        current_level = level;
    }

    // Get current log level
    LogLevel getLogLevel() const {
        return current_level;
    }

    // Enable/disable console output
    void enableConsole(bool enable = true) {
        std::lock_guard<std::mutex> lock(log_mutex);
        console_enabled = enable;
    }

    // Enable/disable file output
    void enableFile(bool enable = true) {
        std::lock_guard<std::mutex> lock(log_mutex);
        file_enabled = enable;
    }

    // Set file output
    bool setFileOutput(const std::string& filename) {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (file_output.is_open()) {
            file_output.close();
        }
        file_output.open(filename, std::ios::app);
        file_enabled = file_output.is_open();
        return file_enabled;
    }

    // Set console output stream
    void setConsoleOutput(std::ostream& output) {
        std::lock_guard<std::mutex> lock(log_mutex);
        console_output = &output;
    }

    // Logging methods
    template<typename... Args>
    void debug(const Args&... args) {
        log(LogLevel::DEBUG, args...);
    }

    template<typename... Args>
    void info(const Args&... args) {
        log(LogLevel::INFO, args...);
    }

    template<typename... Args>
    void warning(const Args&... args) {
        log(LogLevel::WARNING, args...);
    }

    template<typename... Args>
    void error(const Args&... args) {
        log(LogLevel::ERROR, args...);
    }

    template<typename... Args>
    void critical(const Args&... args) {
        log(LogLevel::CRITICAL, args...);
    }

    // Main logging method
    template<typename... Args>
    void log(LogLevel level, const Args&... args) {
        if (level < current_level) return;

        std::lock_guard<std::mutex> lock(log_mutex);
        
        // Build message from arguments
        std::stringstream ss;
        buildMessage(ss, args...);
        std::string message = ss.str();

        // Format message
        std::string formatted_msg = formatMessage(level, message, false);
        std::string colored_msg = formatMessage(level, message, true);

        // Output to console
        if (console_enabled) {
            *console_output << colored_msg << std::endl;
        }

        // Output to file
        if (file_enabled && file_output.is_open()) {
            file_output << formatted_msg << std::endl;
            file_output.flush();
        }
    }

    // Log with custom format
    template<typename... Args>
    void logf(LogLevel level, const std::string& format, const Args&... args) {
        if (level < current_level) return;

        std::lock_guard<std::mutex> lock(log_mutex);
        
        // Simple format string replacement (basic implementation)
        std::string message = format;
        // TODO: Implement proper format string parsing
        
        std::string formatted_msg = formatMessage(level, message, false);
        std::string colored_msg = formatMessage(level, message, true);

        if (console_enabled) {
            *console_output << colored_msg << std::endl;
        }

        if (file_enabled && file_output.is_open()) {
            file_output << formatted_msg << std::endl;
            file_output.flush();
        }
    }

    // Performance logging
    template<typename Func, typename... Args>
    auto logPerformance(const std::string& operation_name, Func&& func, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = std::forward<Func>(func)(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        info("Performance: ", operation_name, " took ", duration.count(), " μs");
        
        return result;
    }

    // Memory usage logging
    void logMemoryUsage(const std::string& context = "") {
        // Note: This is a placeholder. Real memory usage tracking would require
        // platform-specific implementation or external libraries
        info("Memory usage logged for context: ", context);
    }

    // Exception logging
    template<typename Exception>
    void logException(const Exception& e, const std::string& context = "") {
        error("Exception in ", context, ": ", e.what());
    }

    // Static methods for global logging
    static Logger& getGlobalLogger() {
        std::lock_guard<std::mutex> lock(global_mutex);
        if (!global_logger) {
            global_logger = std::make_unique<Logger>("Global");
        }
        return *global_logger;
    }

    static void setGlobalLogLevel(LogLevel level) {
        getGlobalLogger().setLogLevel(level);
    }

    static void setGlobalFileOutput(const std::string& filename) {
        getGlobalLogger().setFileOutput(filename);
    }

    // Static convenience methods
    template<typename... Args>
    static void debug_static(const Args&... args) {
        getGlobalLogger().debug(args...);
    }

    template<typename... Args>
    static void info_static(const Args&... args) {
        getGlobalLogger().info(args...);
    }

    template<typename... Args>
    static void warning_static(const Args&... args) {
        getGlobalLogger().warning(args...);
    }

    template<typename... Args>
    static void error_static(const Args&... args) {
        getGlobalLogger().error(args...);
    }

    template<typename... Args>
    static void critical_static(const Args&... args) {
        getGlobalLogger().critical(args...);
    }
};

// Static member declarations (defined in separate .cpp file if needed)
// For header-only library, we'll use inline initialization
inline std::unique_ptr<Logger> Logger::global_logger = nullptr;
inline std::mutex Logger::global_mutex;

// Convenience functions for global logging
template<typename... Args>
void logDebug(const Args&... args) {
    Logger::debug_static(args...);
}

template<typename... Args>
void logInfo(const Args&... args) {
    Logger::info_static(args...);
}

template<typename... Args>
void logWarning(const Args&... args) {
    Logger::warning_static(args...);
}

template<typename... Args>
void logError(const Args&... args) {
    Logger::error_static(args...);
}

template<typename... Args>
void logCritical(const Args&... args) {
    Logger::critical_static(args...);
}

} // namespace utils
} // namespace dsa
