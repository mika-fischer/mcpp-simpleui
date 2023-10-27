// Copyright Mika Fischer 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

// Work around missing include in glfwpp
#include <utility>

#include <glad/glad.h>
#include <glfwpp/glfwpp.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace mcpp::simpleui {

class FrameScope {
  private:
    glfw::Window &window_;

  public:
    explicit FrameScope(glfw::Window &window) : window_(window) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // TODO: when to poll
        glfw::pollEvents();
    }

    ~FrameScope() {
        // TODO: When to clear background?
        ImGui::Render();
        // TODO: When to adapt to window size changes?
        auto [display_w, display_h] = window_.getFramebufferSize();
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            auto &ctx = glfw::getCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfw::makeContextCurrent(ctx);
        }
        window_.swapBuffers();
    }

    FrameScope(const FrameScope &) = delete;
    FrameScope(FrameScope &&) = delete;
    auto operator=(const FrameScope &) -> FrameScope & = delete;
    auto operator=(FrameScope &&) -> FrameScope & = delete;
};

} // namespace mcpp::simpleui