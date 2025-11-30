#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>

class GpuProfiler {
public:
    GpuProfiler() {
        // 1. Inicjalizacja Query (Timer)
        glGenQueries(2, m_queries);
        glBeginQuery(GL_TIME_ELAPSED, m_queries[0]);
        glEndQuery(GL_TIME_ELAPSED);
        glBeginQuery(GL_TIME_ELAPSED, m_queries[1]);
        glEndQuery(GL_TIME_ELAPSED);

        // 2. Pobranie Static Info (Raz przy starcie)
        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        m_gpuVendor = vendor ? vendor : "Unknown";
        m_gpuRenderer = renderer ? renderer : "Unknown";
        m_driverVersion = version ? version : "Unknown";

        // (Opcjonalnie) Pobranie limitów pamięci VRAM (Intel to lubi!)
        // GLint vram = 0;
        // glGetIntegerv(0x9048, &vram); // GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX (tylko Nvidia/niektóre sterowniki)
        // Ale bezpieczniej zostać przy podstawach.
    }

    ~GpuProfiler() {
        glDeleteQueries(2, m_queries);
    }

    // --- Sekcja Dynamiczna (Timer) ---
    void startFrame() {
        glBeginQuery(GL_TIME_ELAPSED, m_queries[m_currentQuery]);
    }

    void endFrame() {
        glEndQuery(GL_TIME_ELAPSED);
        m_currentQuery = 1 - m_currentQuery;
    }

    float getLastFrameTimeMs() {
        GLuint64 timeNs = 0;
        glGetQueryObjectui64v(m_queries[m_currentQuery], GL_QUERY_RESULT, &timeNs);
        return static_cast<float>(timeNs) / 1000000.0f;
    }

    // --- Sekcja Statyczna (Info) ---
    const std::string& getVendor() const { return m_gpuVendor; }
    const std::string& getRenderer() const { return m_gpuRenderer; }
    const std::string& getVersion() const { return m_driverVersion; }

private:
    // Timer Data
    unsigned int m_queries[2];
    int m_currentQuery = 0;

    // Static Info Data
    std::string m_gpuVendor;
    std::string m_gpuRenderer;
    std::string m_driverVersion;
};