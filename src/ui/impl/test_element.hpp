#ifndef ZGUI_TEST_ELEMENT_HPP
#define ZGUI_TEST_ELEMENT_HPP

#include "../element.hpp"

namespace zgui {
    class TestUIElement : public UIElement {
    private:
        bool* value_;
    public:
        TestUIElement(const char *title, bool *value);
        void draw() override;
        void update() override;
    };
}

#endif // ZGUI_TEST_ELEMENT_HPP