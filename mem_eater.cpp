#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "stopwatch.h"

int main() {
    std::string cmd;
    using vect_t = std::vector<uint64_t>;
    std::vector<std::unique_ptr<vect_t>> storage;

    do {
        std::cout << "enter command [fill, free, q]: ";
        std::cin >> cmd;

        if (cmd == "fill" || cmd == "f") {
            auto sv = stopwatch_t(nullptr);
            auto v = std::make_unique<vect_t>();

            for (uint64_t i = 0; i < 10*1000*1000; ++i)
                v->push_back(i);

            storage.emplace_back(std::move(v));

            continue;
        }

        if (cmd == "free" || cmd == "d") {
            auto sv = stopwatch_t(nullptr);
            if (!storage.empty())
                storage.pop_back();
            continue;
        }

    } while (cmd != "q");
}
