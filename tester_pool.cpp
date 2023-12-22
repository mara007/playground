#include <iostream>
#include <string>
#include <locale>
#include <thread>
#include "arbitrary_pool.h"

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
        std::cout << "test_t(" << i_ << ") used: " << x << std::endl;
    }
};

int main() {
    std::cout << "Hello, arbitrary world!\n";

    auto p = my_pool_t<test_t>(5, 10);

    size_t threads_num = 10;
    std::vector<std::thread> threads(threads_num);
    auto thread_func = [&](auto i){
        for (auto j = 0; j < 5; ++j)
            p.get()->use_me(i);
    };
    for (auto i = 0; i <= threads_num ; i++)
        threads.emplace_back(thread_func, i);

    for (auto& t: threads)
        t.join();

    return 0;
}
