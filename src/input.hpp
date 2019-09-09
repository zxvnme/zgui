#ifndef ZGUI_INPUT_HPP
#define ZGUI_INPUT_HPP

#include "graphics.hpp"

namespace zgui {

    class Input {
    public:
        virtual bool key_pressed(int key) = 0;
        virtual bool mouse_over(Point position, Point size) = 0;
    };
}

#endif // ZGUI_INPUT_HPP
