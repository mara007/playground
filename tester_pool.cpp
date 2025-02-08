#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "my_pool.h"

using namespace std::chrono_literals;
static int int_i = 1;

struct test_t {
    int i_;
    test_t(int i) {
        i_ = int_i++;
        std::cout << "test_t(" << i_ << ", " << i << ") created\n";
    }

    virtual ~test_t() {
        std::cout << "~test_t(" << i_ << ") destroyed\n";
    }

    void use_me(int x) {
        std::cout << "test_t(" << i_ << ") used from thread: " << x << std::endl;
        // std::this_thread::sleep_for(i_ * 1ns);
    }
};

#define THREAD_LOOPS 1000000

int main() {
    std::cout << "Hello, arbitrary world!\n";

    auto p = my_pool_t<test_t>(2, 5);
    // my_pool_t<std::vector<std::string>> my_pool_vector(5, 5);

    size_t threads_num = 20;
    std::vector<std::thread> threads;
    auto thread_func = [&](auto i){
        for (auto j = 0; j < THREAD_LOOPS; ++j) {
            std::cout << "---> T[" << i << "]" << std::endl;
            p.get()->use_me(i);
            std::cout << "<--- T[" << i << "]" << std::endl;
        }
        std::cout << "==== T[" << i << "] ==== done" << std::endl;
    };

    for (auto i = 0; i < threads_num ; i++)
        threads.emplace_back(thread_func, i);

    for (auto& t: threads)
        t.join();

    return 0;
}
