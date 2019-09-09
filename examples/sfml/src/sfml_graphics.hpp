#ifndef ZGUI_SFML_GRAPHICS_HPP
#define ZGUI_SFML_GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <string_view>

#include "../../../src/graphics.hpp"

namespace zgui {

    class SFMLGraphics : public Graphics {
    private:
        sf::RenderWindow &render_window_;
        sf::Vertex z_vert2sfml_vert(const zgui::Vertex &source);

    public:
        explicit SFMLGraphics(sf::RenderWindow &render_window)
            : render_window_(render_window) {};

        void draw() override;
        void reset() override;
    };
}

#endif // ZGUI_SFML_GRAPHICS_HPP
