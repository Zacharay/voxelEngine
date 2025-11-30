#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <string>
#include <atomic>

struct GLFWwindow;

class GuiManager {
public:

    GuiManager(const GuiManager&) = delete;
    GuiManager& operator=(const GuiManager&) = delete;


    static GuiManager& get() {
        static GuiManager instance;
        return instance;
    }


    void init(GLFWwindow* window);
    void shutdown();


    void beginFrame();
    void render();
    void endFrame();


    void setCpuTime(float ms) { m_cpuTime = m_cpuTime * 0.9f + ms * 0.1f; }
    void setGpuTime(float ms) { m_gpuTime = m_gpuTime * 0.9f + ms * 0.1f; }
    void setVertexCount(size_t count) { m_vertexCount = count; }
    void setSelectedBlock(const std::string_view name) { m_selectedBlockName = name; }
    void addVertexCount(size_t count) { m_vertexCount += count; }

    void setGpuRenderer(const std::string& name){m_rendererName = name; };
    void setGpuVendor(const std::string& name){m_vendorName = name; };
    void setDriverVersion(const std::string& name){m_driverVersion = name; };

private:
    GuiManager() = default;
    ~GuiManager() = default;

    void setupStyle();


    float m_cpuTime = 0.0f;
    float m_gpuTime = 0.0f;
    size_t m_vertexCount = 0;

    std::string m_rendererName = "Unknown";
    std::string m_vendorName = "Unknown";
    std::string m_driverVersion = "Unknown";
    std::string_view m_selectedBlockName = "None";
};