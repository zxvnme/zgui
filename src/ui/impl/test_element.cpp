
#include "test_element.hpp"
#include "../../core.hpp"

zgui::TestUIElement::TestUIElement(const char *title, bool *value) {
  this->value_ = value;
  this->title_ = title;
}

void zgui::TestUIElement::draw() {
  zgui::core::graphics->fill_rect(this->position_, this->size_, Color(255, 255, 255));
}

void zgui::TestUIElement::update() {
  *this->value_ = zgui::core::input->mouse_over(this->position_, this->size_);
}