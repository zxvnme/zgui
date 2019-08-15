#include "graphics.hpp"

void zgui::Graphics::set_topology(zgui::VertexTopology vertex_topology) {
    this->vertex_topology_ = vertex_topology;
}

void zgui::Graphics::append_geometry(zgui::Vertex *vertices, size_t count) {
    for (size_t i = 0; i < count; i++) {
        this->vertices.push_back(vertices[i]);
    }
}

void zgui::Graphics::draw_line(Point start, Point end, Color color) {
    Vertex vertices[] = {
            Vertex(start, Point(0.f, 0.f), color),
            Vertex(end, Point(0.f, 0.f), color)
    };

    this->set_topology(VertexTopology::LINE_LIST);
    this->append_geometry(vertices, 2);
}

void zgui::Graphics::fill_rect(zgui::Point position, zgui::Point size, zgui::Color color) {
    Vertex vertices[] = {
            Vertex(position, Point(0.f, 0.f), color),
            Vertex(Point(position.x + size.x, position.y), Point(0.f, 0.f), color),
            Vertex(Point(position.x, position.y + size.y), Point(0.f, 0.f), color),
            Vertex(Point(position.x, position.y + size.y), Point(0.f, 0.f), color),
            Vertex(Point(position.x + size.x, position.y), Point(0.f, 0.f), color),
            Vertex(Point(position.x + size.x, position.y + size.y), Point(0.f, 0.f), color)
    };

    this->set_topology(VertexTopology::TRIANGLE_LIST);
    this->append_geometry(vertices, 6);
}