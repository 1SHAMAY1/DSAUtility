#pragma once
#include <chrono>
#include <iostream>
#include <cstdint>

namespace algo_trace {
    struct TraceContext {
        std::chrono::high_resolution_clock::time_point start_time;
        std::uintptr_t start_stack;
        bool time_enabled = false;
        bool space_enabled = false;
    };
    // Thread-local context for nested/recursive tracing
    inline thread_local TraceContext __trace_ctx;
}

#define TRACE_TIME_COMPLEXITY() \
    algo_trace::__trace_ctx.time_enabled = true; \
    algo_trace::__trace_ctx.start_time = std::chrono::high_resolution_clock::now();

#define TRACE_SPACE_COMPLEXITY() \
    algo_trace::__trace_ctx.space_enabled = true; \
    { int __trace_stack_var; algo_trace::__trace_ctx.start_stack = reinterpret_cast<std::uintptr_t>(&__trace_stack_var); }

#define TRACE_COMPLEXITY() \
    TRACE_TIME_COMPLEXITY(); \
    TRACE_SPACE_COMPLEXITY();

#define STOP_TRACE_TIME_COMPLEXITY() \
    if (algo_trace::__trace_ctx.time_enabled) { \
        auto __trace_end = std::chrono::high_resolution_clock::now(); \
        auto __trace_duration = std::chrono::duration_cast<std::chrono::microseconds>(__trace_end - algo_trace::__trace_ctx.start_time).count(); \
        std::cout << "[Trace] Time Complexity: " << __trace_duration << " us" << std::endl; \
        algo_trace::__trace_ctx.time_enabled = false; \
    }

#define STOP_TRACE_SPACE_COMPLEXITY() \
    if (algo_trace::__trace_ctx.space_enabled) { \
        int __trace_stack_var; \
        std::uintptr_t __trace_end_stack = reinterpret_cast<std::uintptr_t>(&__trace_stack_var); \
        std::ptrdiff_t __trace_stack_diff = algo_trace::__trace_ctx.start_stack - __trace_end_stack; \
        std::cout << "[Trace] Space Complexity (stack diff): " << __trace_stack_diff << " bytes" << std::endl; \
        algo_trace::__trace_ctx.space_enabled = false; \
    }

#define STOP_TRACE_COMPLEXITY() \
    STOP_TRACE_TIME_COMPLEXITY(); \
    STOP_TRACE_SPACE_COMPLEXITY(); 