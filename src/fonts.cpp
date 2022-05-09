// Copyright Mika Fischer 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at https://www.boost.org/LICENSE_1_0.txt)

#include <mcpp/simpleui/fonts.hpp>

#include "droidsans.h"

#include <imgui.h>

#include <stdexcept>
#include <string>

namespace mcpp::simpleui {

auto load_builtin_font(BuiltinFontType type, float size_pixels, const ImFontConfig *font_cfg,
                       const ImWchar *glyph_ranges) -> ImFont * {
    const void *data = nullptr;
    size_t size = 0;
    switch (type) {
        case BuiltinFontType::DroisSans:
            data = FONT_DROIDSANS;
            size = FONT_DROIDSANS_SIZE;
            break;
        default:
            throw std::runtime_error("Invalid BuiltinFontType: " + std::to_string(static_cast<int>(type)));
    }
    void *font_data = ImGui::MemAlloc(size);
    std::memcpy(font_data, data, size);
    return ImGui::GetIO().Fonts->AddFontFromMemoryTTF(font_data, static_cast<int>(size), size_pixels, font_cfg,
                                                      glyph_ranges);
}

} // namespace mcpp::simpleui