<div align="center">
<br>
<img width="258" src="resources/zgui.png" alt="zgui logo">
</div>

<h4 align="center" color="#f62f6d">
<br>Simple immediate mode gui framework.
</h4>

<div align=center>

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)
</div>

#

Simple, two file immediate mode gui framework written in **C++17** that is aimed to people
who begin their journey with game hacking (but not only).
It's main aspect is **universality** (it will work on all APIs; DirectX, OpenGL etc.) and fact that it is fully **dependency free**. Because of that zgui is really **easy to use** and it has simple but aesthetic UI that can be extended or modified easily too.

All functions of framework are described in header file.
Guide how to embed it in your project & example code  is on the bottom of README.

See gif below for preview.

<img src="https://i.imgur.com/Y9KWXLf.gif" width="500" height="350" />


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
auto& draw_functions = g_zgui->get_functions();

draw_functions.draw_line = (zgui::line_t)line;
draw_functions.draw_rect = (zgui::rect_t)rect;
draw_functions.draw_filled_rect = (zgui::filled_rect_t)filled_rect;
draw_functions.draw_text = (zgui::text_t)text;
draw_functions.get_text_size = (zgui::get_text_size_t)get_text_size;
draw_functions.get_frametime = (zgui::get_frametime)get_frametime;
```
and finally, you can render the gui, here's short demo
```cpp
static bool example = false;
static int example_int = 10;
if (g_zgui->begin_window("zgui example window", zgui::vec2{ 500, 350 }, g::Tahoma, zgui_window_flags_none))
{
  g_zgui->checkbox("sample checkbox#example", example);
  //  value before hash is visible ^
  g_zgui->slider_int("#sample_slider", 0, 40, example_int);
  //                  ^ value after hash is hidden"
  //
  // Hashing is demystified in zgui.hh
}
g_zgui->end_window();
```

### Closing speech
Special thanks to [cyanidee](https://github.com/cyanidee) for explaining me loads of things.
... and all the community contributors.
