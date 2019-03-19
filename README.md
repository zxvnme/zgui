<div align="center">
<img width="258" src="resources/zgui.png" alt="zgui logo">
<h4>Simple immediate mode gui framework.</h4>

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
Functions used in this example are from valve_sdk (but their can be from wherever you want)

```cpp
void line(int x, int y, int x2, int y2, Color c) { g_pSurface->Line(x, y, x2, y2, c); }
void rect(int x, int y, int x2, int y2, Color c) { g_pSurface->OutlinedRect(x, y, x2, y2, c); }
void filled_rect(int x, int y, int x2, int y2, Color c) { g_pSurface->FilledRect(x, y, x2, y2, c); }
void text(int x, int y, Color color, int font, bool center, const char* _input, ...) { g_pSurface->DrawT(x, y, color, font, center, _input); }
void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall) { g_pSurface->GetTextSize(font, text, wide, tall); }
float get_frametime() { return g_pGlobalVars->frametime; }
```

then in your hooks initialize function

```cpp
auto& draw_functions = zgui::get_functions();

draw_functions.draw_line        = reinterpret_cast<zgui::line_t>(line);
draw_functions.draw_rect        = reinterpret_cast<zgui::rect_t>(rect);
draw_functions.draw_filled_rect = reinterpret_cast<zgui::filled_rect_t>(filled_rect);
draw_functions.draw_text        = reinterpret_cast<zgui::text_t>(text);
draw_functions.get_text_size    = static_cast<zgui::get_text_size_t>(get_text_size);
draw_functions.get_frametime    = static_cast<zgui::get_frametime>(get_frametime);
```

and finally, you can render the gui, here's short demo

```cpp
static bool example = false;
static int example_int = 10;
if (zgui::begin_window("zgui example window", { 500, 350 }, g::Tahoma, zgui::zgui_window_flags_none))
{
    zgui::->checkbox("sample checkbox#example", example);
    //  value before hash is visible ^
    zgui::->slider_int("#sample_slider", 0, 40, example_int);
    //                  ^ value after hash is hidden"
    //
    // Hashing is demystified in zgui.hh
}
zgui::->end_window();
```

### Acknowledgments
Special thanks to [cyanidee](https://github.com/cyanidee) for explaining me loads of things.
... and all the community contributors.
