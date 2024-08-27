#pragma once

#include <chrono>
#include <functional>
#include <iostream>

class stopwatch_t {
    using tp_t_ = std::chrono::time_point<std::chrono::system_clock>;
    tp_t_ start_;
    std::function<void(size_t)> log_cb_;

    public:
    using tp_t = tp_t_;

    stopwatch_t(std::function<void(size_t)> log_cb)
    : start_(std::chrono::system_clock::now())
    , log_cb_(std::move(log_cb))
    {}

    ~stopwatch_t() {
        auto end = std::chrono::system_clock::now();
        size_t dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count();
        if (log_cb_)
            log_cb_(dur);
        else
            std::cout << "duration: " << dur << "[ms]\n";
    }
};

