#ifndef ZGUI_SFML_INPUT_HPP
#define ZGUI_SFML_INPUT_HPP

#include "../../src/input.hpp"

namespace zgui {
    class SFMLInput : public Input {
    public:
        bool key_pressed(int key) override;
    };
}

#endif // ZGUI_SFML_INPUT_HPP
