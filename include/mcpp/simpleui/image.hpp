// Copyright Mika Fischer 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <glad/glad.h>
#include <stdexcept>
#include <string>
#include <utility>

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

inline auto create_texture(RGB24Image img) -> GLuint {
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
