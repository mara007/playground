#pragma once

#include <list>
#include <memory>
#include <cassert>
#include <iostream>
#include <utility>
#include <functional>
#include <condition_variable>

template<typename T>
class my_pool_t {
    std::list<std::unique_ptr<T>> pool_;
    std::mutex pool_mutex_;
    std::condition_variable cv_;
    bool pool_empty_;

    T* pop(bool do_lock = true) {
        auto lock = std::unique_lock<std::mutex>(pool_mutex_, std::defer_lock);
        if (do_lock)
            lock.lock();

        if (pool_.empty())
            return nullptr;

        auto raw_ptr = pool_.front().release();
        pool_.pop_front();

        pool_empty_ = pool_.empty();
        std::cout << "POP:  size: " << pool_.size() << std::endl;
        return raw_ptr;
    }

    void push(std::unique_ptr<T>&& p) {
        {
            auto guard = std::lock_guard<std::mutex>(pool_mutex_);
            pool_.push_back(std::move(p));
            std::cout << "PUSH: size: " << pool_.size() << std::endl;

            pool_empty_ = pool_.empty();
        }
        cv_.notify_one();
    }

    public:

    template<typename... Args>
    my_pool_t(size_t pool_size, Args&&... args)
    : pool_(), pool_mutex_(), cv_()
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

        std::unique_lock<std::mutex> l(pool_mutex_);
        std::cout << "\nget() .. waiting" << std::endl;
        cv_.wait(l, [this]{ return !pool_empty_; });
        std::cout << "\nget() .. WAKING" << std::endl;
        ptr = pop(false);
        assert(ptr);
        return std::unique_ptr<T, std::function<void(T*)>>(ptr, deleter);
    }
};
