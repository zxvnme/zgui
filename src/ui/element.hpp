#ifndef ZGUI_UI_ELEMENT_HPP
#define ZGUI_UI_ELEMENT_HPP

#include <vector>
#include "../graphics.hpp"

namespace zgui {
    class UIElement {
    protected:
        std::vector<UIElement*> children_;
        const char *title_;
        UIElement* parent_;
        Point position_, size_;

    public:
        void add_child(UIElement* child);
        void set_parent(UIElement* parent);

        virtual void draw();
        virtual void update();

        const Point& get_position() const;
        void set_position(const Point &position);

        const Point& get_size() const;
        void set_size(const Point &size);
    };
}

#endif // ZGUI_UI_ELEMENT_HPP