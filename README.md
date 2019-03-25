<div align="center">
<img width="258" src="resources/zgui.png" alt="zgui logo">
<h4>Simple immediate mode gui framework</h4>

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)
</div>

#

Simple, two file immediate mode gui framework written in **C++17**, mainly aimed for people beginning their journey with game hacking (but not limited to).
Its main goals are to be **universal** (it works on all graphics APIs; DirectX, OpenGL, Vulkan etc.) and **dependency free**. Therefore zgui is really **easy to use** and it has simple but aesthetic UI that can be extended or modified easily too.

All functions of framework are described in [header file](zgui.hh).
Guide on embedding zgui in your project as well as code examples are on the bottom of README.

See below gif for preview.

<img src="https://i.imgur.com/Y9KWXLf.gif" width="500" height="350">


## How to use & example
The core thing that you need to do is to wrap some rendering functions.

At first, you should specify window name from where the input will be read from (top of `zgui.cc`)

```cpp
constexpr std::string_view INPUT_WINDOW{ "" };
```

In top of `hooks.cc` (whatever) create some proxy functions.

```cpp
void line(int x, int y, int x2, int y2, zgui::color c) noexcept { /* draw line using your renderer */ }
void rect(int x, int y, int x2, int y2, zgui::color c) noexcept { /* draw outlined rectangle using your renderer */ }
void filled_rect(int x, int y, int x2, int y2, zgui::color c) noexcept { /* draw filled rectangle using your renderer */ }
void text(int x, int y, zgui::color color, int font, bool center, const char* _input, ...) noexcept { /* draw text using your renderer */ }
void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall) noexcept { /* calculate text size here */ }
float get_frametime() noexcept { /* return frametime */ }
```

then in your hooks initialize function

```cpp
zgui::functions.draw_line = line;
zgui::functions.draw_rect = rect;
zgui::functions.draw_filled_rect = filled_rect;
zgui::functions.draw_text = text;
zgui::functions.get_text_size = get_text_size;
zgui::functions.get_frametime = get_frametime;
```

and finally, you can render the gui, here's short demo

```cpp
static bool example = false;
static int example_int = 10;
if (zgui::begin_window("zgui example window", { 500, 350 }, g::Tahoma, zgui::zgui_window_flags_none))
{
    zgui::checkbox("sample checkbox #example", example);
    // value before hash is visible ^
    zgui::slider_int("#sample_slider", 0, 40, example_int);
    //                ^ value after hash is hidden"
    //
    // Hashing is demystified in zgui.hh
    zgui::end_window();
}
```

### Acknowledgments
Special thanks to [cyanidee](https://github.com/cyanidee) for explaining me loads of things.
... and all the community contributors.
