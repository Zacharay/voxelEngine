#pragma once
#include <chrono>

class CpuTimer {
public:
    void start() {
        m_startTime = std::chrono::high_resolution_clock::now();
    }

    float stop() {
        auto endTime = std::chrono::high_resolution_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

        return (end - start) * 0.001f;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};