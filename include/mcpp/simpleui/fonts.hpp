// Copyright Mika Fischer 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <imgui.h>

namespace mcpp::simpleui {

enum class BuiltinFontType {
    DroisSans,
};

auto load_builtin_font(BuiltinFontType type, float size_pixels, const ImFontConfig *font_cfg = nullptr,
                       const ImWchar *glyph_ranges = nullptr) -> ImFont *;

} // namespace mcpp::simpleui