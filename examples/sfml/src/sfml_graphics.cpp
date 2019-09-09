#include "sfml_graphics.hpp"

void zgui::SFMLGraphics::draw() {
  std::vector<sf::Vertex> sfml_vertices;
  for (auto &vertex : this->vertices_) {
    sfml_vertices.push_back(this->z_vert2sfml_vert(vertex));
  }

  // TODO: Topology
  this->render_window_.draw(&sfml_vertices.at(0), sfml_vertices.size(), sf::Triangles);
}

void zgui::SFMLGraphics::reset() {
  this->render_window_.clear();
}

sf::Vertex zgui::SFMLGraphics::z_vert2sfml_vert(const zgui::Vertex &source) {
  return sf::Vertex(sf::Vector2f(source.position.x, source.position.y),
                    sf::Color(source.color.r, source.color.g, source.color.b, source.color.a),
                    sf::Vector2f(source.coordinates.x, source.coordinates.y));
}