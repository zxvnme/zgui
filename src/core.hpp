#ifndef ZGUI_CORE_HPP
#define ZGUI_CORE_HPP

#define DRAW_UPDATE(ui_element) \
        ui_element->draw();     \
        ui_element->update();   \

#include <memory>

#include "graphics.hpp"
#include "input.hpp"

namespace zgui::core {
    inline std::shared_ptr<zgui::Graphics> graphics = nullptr;
    inline std::shared_ptr<zgui::Input> input = nullptr;
}

#endif // ZGUI_CORE_HPP