#include "Window.hpp"
#include <iostream>
#include "Config.hpp"
#include <fstream>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Constructor
Window::Window() : m_window(nullptr) {

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW3");
    }


    m_window = glfwCreateWindow(Config::windowWidth, Config::windowHeight, "OpenGL Window", nullptr, nullptr);
    if (!m_window) {

        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }


    glfwMakeContextCurrent(m_window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 440");

    glViewport(0, 0, Config::windowWidth, Config::windowHeight);
    if(Config::wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);;


}

// Destructor
Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}
void Window::log(const std::string& message) {
    std::ofstream logFile("voxelEngine.log", std::ios::app); // open in append mode
    if (logFile.is_open()) {
        logFile << message << std::endl;
        logFile.flush();
    }
}


void Window::calculateFps() {
    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - previousTime;
    frameCount++;

    if (elapsedTime >= 1.0) { // If a second has passed
        double fps = static_cast<double>(frameCount) / elapsedTime;
        log("FPS: " + std::to_string(fps) + '\n');



        // Reset for the next calculation
        previousTime = currentTime;
        frameCount = 0;
    }
}



void Window::run()  {
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);
    while (!glfwWindowShouldClose(m_window)) {

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0.53f,0.8f,0.93f,1.0f);
        
        onUpdate();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Performance");

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);


        ImGui::Text("Frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

        ImGui::End();

        ImGui::Render();

        onRender();


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}



