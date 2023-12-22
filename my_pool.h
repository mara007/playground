#pragma once

#include <cassert>
#include <memory>
#include <utility>
#include <deque>
#include <functional>
#include <condition_variable>

template<typename T>
class my_pool_t {
    std::deque<std::unique_ptr<T>> pool_;
    std::mutex pool_mutex_, cv_mutex_;
    std::condition_variable cv_;

    T* pop() {
        auto guard = std::lock_guard<std::mutex>(pool_mutex_);
        if (pool_.empty())
            return nullptr;

        auto raw_ptr = pool_.front().release();
        pool_.pop_front();
        return raw_ptr;
    }

    void push(std::unique_ptr<T>&& p) {
        {
            auto guard = std::lock_guard<std::mutex>(pool_mutex_);
            pool_.push_back(std::move(p));
        }
        cv_.notify_one();
    }

    public:

    template<typename... Args>
    my_pool_t(size_t pool_size, Args&&... args)
    : pool_()
    , pool_mutex_()
    , cv_mutex_()
    , cv_()
    {
        while (pool_size--) {
            pool_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }
    }

    std::unique_ptr<T, std::function<void(T*)>> get() {
        auto deleter = [this](auto p) noexcept { push(std::unique_ptr<T>(p));};
        auto ptr = pop();
        if (ptr)
            return std::unique_ptr<T, std::function<void(T*)>>(ptr, deleter);

        std::unique_lock<std::mutex> l(cv_mutex_);
        std::cout << "get() .. waiting" << std::endl;
        cv_.wait(l);
        ptr = pop();
        assert(ptr);
        return std::unique_ptr<T, std::function<void(T*)>>(ptr, deleter);
    }
};