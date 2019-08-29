#include <SFML/Window/Keyboard.hpp>
#include "sfml_input.hpp"

bool zgui::SFMLInput::key_pressed(int key) {
  return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
}