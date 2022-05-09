// Copyright Mika Fischer 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <mcpp/simpleui/frame_scope.hpp>

#include <glad/glad.h>
#include <glfwpp/glfwpp.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace mcpp::simpleui {

class MainWindow {
  private:
    glfw::Window window_;

  public:
    // TODO: Pass through parameters
    MainWindow() : window_(1280, 720, "simpleui example") {
        glfw::makeContextCurrent(window_);
        if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
            throw std::runtime_error("Unable to initialize GLAD");
        }
        glfw::swapInterval(1);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        auto &style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init();
    }

    ~MainWindow() {
        if (static_cast<GLFWwindow *>(window_)) {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
    }

    MainWindow(const MainWindow &) = delete;
    auto operator=(const MainWindow &) -> MainWindow & = delete;

    MainWindow(MainWindow &&) = default;
    auto operator=(MainWindow &&) -> MainWindow & = default;

    [[nodiscard]] auto shouldClose() const -> bool { return window_.shouldClose(); }

    [[nodiscard]] auto startFrame() -> FrameScope { return FrameScope(window_); }
};

} // namespace mcpp::simpleui