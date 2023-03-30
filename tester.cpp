#include <iostream>
#include <thread>

int main (int argc, char * argv [])
{
    std::cout << "Hello world! of git" << std::endl;

    int i = 0;
    do {
        std::cout << "i=" << i << std::endl;
        if (++i < 10)
            continue;
    } while (false);
    return 0;
}

