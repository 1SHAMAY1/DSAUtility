#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>

namespace dsa {
namespace utils {

/**
 * @brief High-resolution timer utility for performance measurement
 */
class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool is_running;
    std::string timer_name;

public:
    Timer(const std::string& name = "Timer") 
        : is_running(false), timer_name(name) {}

    /**
     * @brief Start the timer
     */
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        is_running = true;
    }

    /**
     * @brief Stop the timer
     */
    void stop() {
        if (is_running) {
            end_time = std::chrono::high_resolution_clock::now();
            is_running = false;
        }
    }

    /**
     * @brief Reset the timer
     */
    void reset() {
        is_running = false;
    }

    /**
     * @brief Get elapsed time in nanoseconds
     */
    long long getElapsedNanoseconds() const {
        auto end = is_running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_time).count();
    }

    /**
     * @brief Get elapsed time in microseconds
     */
    long long getElapsedMicroseconds() const {
        auto end = is_running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration_cast<std::chrono::microseconds>(end - start_time).count();
    }

    /**
     * @brief Get elapsed time in milliseconds
     */
    long long getElapsedMilliseconds() const {
        auto end = is_running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count();
    }

    /**
     * @brief Get elapsed time in seconds
     */
    double getElapsedSeconds() const {
        auto end = is_running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration_cast<std::chrono::duration<double>>(end - start_time).count();
    }

    /**
     * @brief Get formatted elapsed time string
     */
    std::string getElapsedTimeString() const {
        auto nanoseconds = getElapsedNanoseconds();
        
        if (nanoseconds < 1000) {
            return std::to_string(nanoseconds) + " ns";
        } else if (nanoseconds < 1000000) {
            return std::to_string(nanoseconds / 1000.0) + " μs";
        } else if (nanoseconds < 1000000000) {
            return std::to_string(nanoseconds / 1000000.0) + " ms";
        } else {
            return std::to_string(nanoseconds / 1000000000.0) + " s";
        }
    }

    /**
     * @brief Print elapsed time
     */
    void printElapsedTime() const {
        std::cout << timer_name << ": " << getElapsedTimeString() << std::endl;
    }

    /**
     * @brief Check if timer is running
     */
    bool isRunning() const {
        return is_running;
    }

    /**
     * @brief Get timer name
     */
    const std::string& getName() const {
        return timer_name;
    }

    /**
     * @brief Set timer name
     */
    void setName(const std::string& name) {
        timer_name = name;
    }
};

/**
 * @brief RAII timer that automatically prints elapsed time when destroyed
 */
class ScopedTimer {
private:
    Timer timer;
    bool auto_print;

public:
    ScopedTimer(const std::string& name = "ScopedTimer", bool print_on_destroy = true)
        : timer(name), auto_print(print_on_destroy) {
        timer.start();
    }

    ~ScopedTimer() {
        timer.stop();
        if (auto_print) {
            timer.printElapsedTime();
        }
    }

    Timer& getTimer() { return timer; }
};

/**
 * @brief Utility function to measure execution time of a function
 */
template<typename Func, typename... Args>
auto measureTime(const std::string& name, Func&& func, Args&&... args) {
    Timer timer(name);
    timer.start();
    auto result = std::forward<Func>(func)(std::forward<Args>(args)...);
    timer.stop();
    timer.printElapsedTime();
    return result;
}

/**
 * @brief Utility function to measure execution time without printing
 */
template<typename Func, typename... Args>
auto measureTimeSilent(Func&& func, Args&&... args) {
    Timer timer;
    timer.start();
    auto result = std::forward<Func>(func)(std::forward<Args>(args)...);
    timer.stop();
    return std::make_pair(result, timer.getElapsedNanoseconds());
}

/**
 * @brief Utility function to run a function multiple times and get average time
 */
template<typename Func, typename... Args>
double measureAverageTime(const std::string& name, int iterations, Func&& func, Args&&... args) {
    Timer timer(name);
    long long total_time = 0;
    
    for (int i = 0; i < iterations; ++i) {
        timer.start();
        std::forward<Func>(func)(std::forward<Args>(args)...);
        timer.stop();
        total_time += timer.getElapsedNanoseconds();
    }
    
    double average_time = static_cast<double>(total_time) / iterations;
    std::cout << name << " (average over " << iterations << " runs): " 
              << (average_time / 1000000.0) << " ms" << std::endl;
    
    return average_time;
}

/**
 * @brief Utility function to benchmark two functions
 */
template<typename Func1, typename Func2, typename... Args>
void benchmark(const std::string& name1, Func1&& func1, 
               const std::string& name2, Func2&& func2, 
               int iterations, Args&&... args) {
    std::cout << "Benchmarking " << name1 << " vs " << name2 << " (" << iterations << " iterations):" << std::endl;
    
    double time1 = measureAverageTime(name1, iterations, std::forward<Func1>(func1), std::forward<Args>(args)...);
    double time2 = measureAverageTime(name2, iterations, std::forward<Func2>(func2), std::forward<Args>(args)...);
    
    double ratio = time1 / time2;
    std::cout << "Ratio (" << name1 << "/" << name2 << "): " << std::fixed << std::setprecision(2) << ratio << std::endl;
}

} // namespace utils
} // namespace dsa
