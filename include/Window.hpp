#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>

#include "CpuTimer.hpp"
#include "GpuProfiler.hpp"

class Window  {
private:
    void calculateFps();
    int frameCount;
    double previousTime;

    CpuTimer m_cpuTimer;
    std::unique_ptr<GpuProfiler> m_gpuProfiler;

public:
    Window();
    void run();
protected:
    GLFWwindow *m_window;
    ~Window();

    void log(const std::string &message);
    virtual void onUpdate()=0;
    virtual void onRender()=0;

};
