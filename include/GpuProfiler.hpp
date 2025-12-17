#pragma once
#include <glad/glad.h>
#include <string>
#include <vector>

class GpuProfiler {
public:
    GpuProfiler() {

        glGenQueries(2, m_queries);
        glBeginQuery(GL_TIME_ELAPSED, m_queries[0]);
        glEndQuery(GL_TIME_ELAPSED);
        glBeginQuery(GL_TIME_ELAPSED, m_queries[1]);
        glEndQuery(GL_TIME_ELAPSED);


        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        m_gpuVendor = vendor ? vendor : "Unknown";
        m_gpuRenderer = renderer ? renderer : "Unknown";
        m_driverVersion = version ? version : "Unknown";

    }

    ~GpuProfiler() {
        glDeleteQueries(2, m_queries);
    }


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


    [[nodiscard]] std::string_view  getVendor() const { return m_gpuVendor; }
    [[nodiscard]] std::string_view  getRenderer() const { return m_gpuRenderer; }
    [[nodiscard]] std::string_view  getVersion() const { return m_driverVersion; }

private:
    // Timer Data
    unsigned int m_queries[2];
    int m_currentQuery = 0;

    // Static Info Data
    std::string_view m_gpuVendor;
    std::string_view m_gpuRenderer;
    std::string_view m_driverVersion;
};