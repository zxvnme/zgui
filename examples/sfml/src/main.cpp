#include <SFML/Graphics.hpp>
#include <memory>

#include "sfml_graphics.hpp"
#include "sfml_input.hpp"
#include "../../../src/core.hpp"
#include "../../../src/ui/impl/test_element.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(600, 600), "zgui_example_sfml_impl");

  zgui::core::graphics = std::make_shared<zgui::SFMLGraphics>(window);
  zgui::core::input = std::make_shared<zgui::SFMLInput>(window);

  static bool sample_boolean = false;

  const auto test_element = std::make_shared<zgui::TestUIElement>("test", &sample_boolean);
  test_element->set_position(zgui::Point(16, 16));
  test_element->set_size(zgui::Point(100, 30));

  while (window.isOpen()) {
    sf::Event event {};
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    zgui::core::graphics->reset();

    // =========

    DRAW_UPDATE(test_element)

    // =========

    zgui::core::graphics->draw();

    window.display();
  }

  return 0;
}