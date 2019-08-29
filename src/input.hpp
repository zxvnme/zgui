#ifndef ZGUI_INPUT_HPP
#define ZGUI_INPUT_HPP

namespace zgui {

    class Input {
    public:
        virtual bool key_pressed(int key) = 0;
    };
}

#endif // ZGUI_INPUT_HPP
