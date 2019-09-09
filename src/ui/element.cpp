
#include "element.hpp"

void zgui::UIElement::add_child(UIElement *child) {
  this->children_.push_back(child);
  child->set_parent(this);
}

void zgui::UIElement::set_parent(UIElement *parent) {
  this->parent_ = parent;
}

void zgui::UIElement::draw() {
  for (const auto child : this->children_)
    child->draw();
}

void zgui::UIElement::update() {
  for (const auto child : this->children_)
    child->update();
}

const zgui::Point &zgui::UIElement::get_position() const {
  return position_;
}

void zgui::UIElement::set_position(const Point &position) {
  position_ = position;
}

const zgui::Point &zgui::UIElement::get_size() const {
  return size_;
}

void zgui::UIElement::set_size(const Point &size) {
  size_ = size;
}