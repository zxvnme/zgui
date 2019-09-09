#ifndef ZGUI_SFML_INPUT_HPP
#define ZGUI_SFML_INPUT_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include "../../../src/input.hpp"
#include "../../../src/graphics.hpp"

namespace zgui {
    class SFMLInput : public Input {
    private:
        sf::RenderWindow &render_window_;

    public:
        explicit SFMLInput(sf::RenderWindow &render_window)
          : render_window_(render_window) {};

        bool key_pressed(int key) override;
        bool mouse_over(Point position, Point size) override;
    };
}

#endif // ZGUI_SFML_INPUT_HPP
