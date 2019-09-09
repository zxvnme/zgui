#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "sfml_input.hpp"
#include "../../../src/graphics.hpp"

bool zgui::SFMLInput::key_pressed(int key) {
  return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
}

bool zgui::SFMLInput::mouse_over(Point position, Point size) {
  const auto mouse_pos = sf::Mouse::getPosition(this->render_window_);
  return mouse_pos.x > position.x && mouse_pos.y > position.y
         && mouse_pos.x < size.x + position.x
         && mouse_pos.y < size.y + position.y;
}