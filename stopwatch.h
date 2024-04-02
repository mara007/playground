#pragma once

#include <chrono>
#include <functional>

class stopwatch_t {
    using tp_t = std::chrono::time_point<std::chrono::high_resolution_clock>;
     tp_t start_;
    std::function<void(size_t)> log_cb_;

    public:
    using tp_t = tp_t;

    stopwatch_t(std::function<void(size_t)> log_cb)
    : start_(std::chrono::high_resolution_clock::now())
    , log_cb_(std::move(log_cb))
    {}

    ~stopwatch_t() {
        auto end = std::chrono::high_resolution_clock::now();
        size_t dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count();
        if (log_cb_)
            log_cb_(dur);
    }
};

