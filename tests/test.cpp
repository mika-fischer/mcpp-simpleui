// Copyright Mika Fischer 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <imgui.h>
#include <mcpp/simpleui.hpp>
#include <string>

struct State {
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = {0.45f, 0.55f, 0.60f, 1.00f};
    float f = 0.0f;
    int counter = 0;
    bool dockspace_open = true;
};

struct RGB24Image {
    int width;
    int height;
    const uint8_t *data;
};

template <auto &func, typename... Args>
auto gl_call_nothrow(Args &&...args) noexcept -> GLenum {
    func(std::forward<Args>(args)...);
    return glGetError();
}

template <auto &func, typename... Args>
auto gl_call(Args &&...args) {
    auto error = gl_call_nothrow<func>(std::forward<Args>(args)...);
    if (error != GL_NO_ERROR) {
        throw std::runtime_error(std::string("OpenGL error: ") + std::to_string(error));
    }
}

auto create_texture(RGB24Image img) -> GLuint {
    auto tex = GLuint{};
    gl_call<glGenTextures>(1, &tex);
    gl_call<glBindTexture>(GL_TEXTURE_2D, tex);
    auto levels = 10;
    for (int level = 0; level < levels; ++level) {
        gl_call<glTexImage2D>(GL_TEXTURE_2D, level, GL_RGBA8, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                              nullptr);
    }
    gl_call<glTexParameteri>(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    gl_call<glTexParameteri>(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, levels - 1);
    gl_call<glPixelStorei>(GL_UNPACK_ALIGNMENT, 1);
    gl_call<glPixelStorei>(GL_UNPACK_ROW_LENGTH, 0);
    gl_call<glTexSubImage2D>(GL_TEXTURE_2D, 0, 0, 0, img.width, img.height, GL_RGB, GL_UNSIGNED_BYTE, img.data);
    gl_call<glGenerateMipmap>(GL_TEXTURE_2D);
    gl_call<glTexParameteri>(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    gl_call<glTexParameteri>(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gl_call<glTexParameteri>(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl_call<glTexParameteri>(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    return tex;
}

auto main() -> int {
    auto ctx = mcpp::simpleui::Context();
    auto wnd = mcpp::simpleui::MainWindow();

    auto *font = ImGui::GetIO().Fonts->AddFontFromFileTTF("DroidSans.ttf", 15);

    // Our state
    auto state = State();
    auto width = 16;
    auto height = 16;
    auto img_data = std::vector<uint8_t>(width * height * 3);
    for (size_t y = 0; y < height; ++y) {
        auto *ptr = img_data.data() + y * width * 3;
        for (size_t x = 0; x < width; ++x) {
            *ptr++ = int(float(x) / float(width) * 255.0f);
            *ptr++ = int(float(y) / float(height) * 255.0f);
            *ptr++ = int(float(height - y) / float(height) * 255.0f);
        }
    }
    auto img = RGB24Image{width, height, img_data.data()};
    auto tex = create_texture(img);

    // Main loop
    while (!wnd.shouldClose()) {
        auto frame = wnd.startFrame();

        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &state.dockspace_open, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);
        auto dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        glClearColor(state.clear_color.x * state.clear_color.w, state.clear_color.y * state.clear_color.w,
                     state.clear_color.z * state.clear_color.w, state.clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        if (state.show_demo_window) {
            ImGui::ShowDemoWindow(&state.show_demo_window);
        }

        if (true) {
            ImGui::Begin("Hello, world!");

            ImGui::Text("This is some useful text.");
            ImGui::Checkbox("Demo Window", &state.show_demo_window);
            ImGui::Checkbox("Another Window", &state.show_another_window);

            ImGui::SliderFloat("float", &state.f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float *)&state.clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) {
                state.counter++;
            }
            ImGui::SameLine();
            ImGui::Text("counter = %d", state.counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::End();
        }

        ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoTitleBar);
        auto availmin = ImGui::GetWindowContentRegionMin();
        auto availmax = ImGui::GetWindowContentRegionMax();
        auto avail = ImVec2(availmax.x - availmin.x, availmax.y - availmin.y);

        auto w_scale = avail.x / float(img.width);
        auto h_scale = avail.y / float(img.height);
        auto scale = std::min(w_scale, h_scale);

        auto scaled_size = ImVec2(img.width * scale, img.height * scale);
        auto extra = ImVec2(avail.x - img.width * scale, avail.y - img.height * scale);
        if (extra.x > 0.0f) {
            ImGui::SetCursorPosX(extra.x / 2.0f + availmin.x);
        } else if (extra.y > 0.0f) {
            ImGui::SetCursorPosY(extra.y / 2.0f + availmin.y);
        }

        ImGui::Image((void *)(intptr_t)tex, scaled_size);
        ImGui::End();

        if (state.show_another_window) {
            ImGui::Begin("Another Window", &state.show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) {
                state.show_another_window = false;
            }
            ImGui::End();
        }

        ImGui::End();
    }
}