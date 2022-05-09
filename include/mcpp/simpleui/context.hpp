// Copyright Mika Fischer 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <glfwpp/glfwpp.h>

namespace mcpp::simpleui {

class Context {
  private:
    glfw::GlfwLibrary lib_;

  public:
    Context() : lib_(glfw::init()) {
        // Set some default window hints
        auto hints = glfw::WindowHints();
        hints.contextVersionMajor = 3;
        hints.contextVersionMinor = 2;
        hints.openglProfile = glfw::OpenGlProfile::Core;
        hints.openglForwardCompat = true;
        hints.apply();
    }
};

} // namespace mcpp::simpleui