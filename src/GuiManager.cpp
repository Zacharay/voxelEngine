#include "GuiManager.hpp"
#include <GLFW/glfw3.h>

void GuiManager::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    setupStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 440");
}

void GuiManager::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GuiManager::setupStyle() {
    ImGui::StyleColorsDark();
    // Tweak transparency if you want
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.5f;
}

void GuiManager::beginFrame() {


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GuiManager::render() {

    const float PAD = 10.0f;
    ImGui::SetNextWindowPos(ImVec2(PAD, PAD), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.75f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing |
                             ImGuiWindowFlags_NoNav;


    if (ImGui::Begin("Engine Stats", nullptr, flags)) {

        // FPS
        ImGui::SetWindowFontScale(1.3f);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%.0f FPS", ImGui::GetIO().Framerate);
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "(%.2f ms)", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::SetWindowFontScale(1.0f);

        ImGui::Separator();

        //TIMERS
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "TIMINGS");

        if (ImGui::BeginTable("Timers", 2)) {
            ImGui::TableNextColumn();
            ImGui::Text("CPU Latency:");
            ImGui::TableNextColumn();
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "%.2f ms", (float)m_cpuTime);

            ImGui::TableNextColumn();
            ImGui::Text("GPU Latency:");
            ImGui::TableNextColumn();
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%.2f ms", (float)m_gpuTime);

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();

        //SCENE ---
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "GEOMETRY");

        ImGui::Text("Vertices:  %zu", (size_t)m_vertexCount);
        ImGui::Text("Triangles: %zu", (size_t)m_vertexCount / 3);


        ImGui::Text("Selection: ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%.*s",
                           static_cast<int>(m_selectedBlockName.size()), m_selectedBlockName.data());

        ImGui::Spacing();
        ImGui::Separator();

        //HARDWARE INFO
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 1.0f, 1.0f), "HARDWARE");

        ImGui::PushTextWrapPos(ImGui::GetWindowSize().x - 10.0f);
        ImGui::TextDisabled("%s", m_rendererName.c_str());
        ImGui::PopTextWrapPos();


        ImGui::SetWindowFontScale(0.8f);
        ImGui::TextDisabled("Driver: %s", m_driverVersion.c_str());
        ImGui::SetWindowFontScale(1.0f);
    }
    ImGui::End();

}

void GuiManager::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    m_vertexCount = 0;

}