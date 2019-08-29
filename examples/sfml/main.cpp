#include <SFML/Graphics.hpp>
#include <memory>

#include "sfml_graphics.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(600, 600), "zgui_example_sfml_impl");

  auto graphics = std::make_shared<zgui::SFMLGraphics>(window);

  while (window.isOpen()) {
    sf::Event event {};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    graphics->reset();
    // ==== Render things here ====
    // Prototype drawing, there should be ui elements not shapes.
    graphics->fill_rect(zgui::Point(300, 300), zgui::Point(100, 100), zgui::Color(255, 255, 255));
    // =========
    graphics->draw();

    window.display();
  }

  return 0;
}
