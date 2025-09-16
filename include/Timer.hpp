#include <iostream>
#include <chrono>

class Timer {
public:
    std::chrono::high_resolution_clock::time_point start;
    Timer():start(std::chrono::high_resolution_clock::now()){}
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Took "
                  << std::chrono::duration<double, std::milli>(end - start).count()
                  << " ms\n";
    }
};