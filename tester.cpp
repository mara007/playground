#include <iostream>
#include <thread>
#include <vector>
#include <format>
#include "stopwatch.h"


struct tester_t {
    tester_t() { std::cout << "tester_t()\n\n"; }
    tester_t(const tester_t& t) { std::cout << "tester_t(tester_t&)\n\n"; }
    tester_t& operator=(const tester_t& rhs) {
        std::cout << "tester_t::operator=()\n\n";
        return *this;
    }
    tester_t& operator=(const tester_t&& rhs) {
        std::cout << "tester_t::operator=(move)\n\n";
        return *this;
    }
     tester_t(tester_t&& t) { std::cout << "tester_t(move)\n\n"; }
    virtual ~tester_t() { std::cout << "~tester_t\n\n"; }
};

struct virt_test_t {
    void process() {
        std::cout << "virt_test_t::process()\n";
        virt();
    }

    virtual void virt() {
        std::cout << "virt_test_t::virt()\n";
    }
};

struct virt_test2_t : public virt_test_t {
    void virt() override {
        std::cout << "virt_test2_t::virt()\n";
    }
};

void replace_all_of(std::string& where, const std::string& what, const std::string& by) {
    auto pos = where.find_first_of(what);
    while (pos != std::string::npos) {
        where.replace(pos, 1, by);
        pos = where.find_first_of(what, pos);
    }

    // void neco() {
    //     std:: cout << "neco\n";
    // }
    // export TERM=xtermc
}


int main (int argc, char * argv [])
{
    std::vector<int> realoc_me;

    std::cout << std::format("Start size:{} capacity:{}\n", realoc_me.size(), realoc_me.capacity());
    for (int i = 1; i <= 16; ++i) {
        realoc_me.push_back(i);
        std::cout << std::format("iter:{} size:{} capacity:{}\n", i, realoc_me.size(), realoc_me.capacity());
    }
    return 0;

    std::string timeoutStr = "1000 ms";
    auto notDigit = [](char c) { return c != ' ' && !std::isdigit(c); };
    auto unitPos = std::find_if(std::begin(timeoutStr), std::end(timeoutStr), notDigit);
    std::cout << "str: " << timeoutStr << " unitPos = " << *unitPos << std::endl;

    return 0;

    stopwatch_t sw([](size_t dur) { std::cout << "it took: " << dur << "[us]\n";});

    std::string test{"test/stri|ng||gg\\\\"};
    std::cout << "test: " << test << std::endl;
    replace_all_of(test, "/|\\", "");
    std::cout << "test: " << test << std::endl;

    virt_test_t v;
    virt_test2_t v2;
    virt_test_t &vt = v;
    virt_test_t &vt2 = v2;
    vt.process();
    std::cout << "vt2\n";
    vt2.process();
    std::cout << "v2\n";
    v2.process();

    std::vector<int> vec{1,2,3,4};
    std::cout << "velikost je " << vec.end() - vec.begin() << std::endl;
    std::cout << "sizeof std::threa: " << sizeof(std::thread) << std::endl;
    auto s = std::string("popnutoX");
        s.pop_back();
    std::cout << " pop back: " << s << std::endl;
    return 0;


    std::cout << "Hello world! of git" << std::endl;

    std::cout << "constr\n";
    tester_t t1;
    std::cout << "copy oper\n";
    tester_t t2 = t1;
    std::cout << "copy const\n";
    tester_t t3(t1);
    std::cout << "copy oper2\n";
    t1 = t2;
    std::cout << "copy constr from tmp\n";
    tester_t t4{tester_t()};
    std::cout << "copy oper from tmp\n";
    tester_t t5 = tester_t();
    std::cout << "move\n";
    t4 = std::move(t5);
    std::cout << "constr move\n";
    tester_t t6(std::move(t5));



    int i = 0;
    do {
        std::cout << "i=" << i << std::endl;
        if (++i < 10)
            continue;
    } while (false);
    return 0;
}

