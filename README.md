<div align="center">
<br>
<img width="258" src="resources/zgui.png" alt="hex-en logo">
</div>
<h5 align="center" color="#f62f6d">
<br>Simple immediate mode gui framework.
</h5>
<div align=center>

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-love.svg)](https://forthebadge.com)
</div>

## General
Simple, two header immediate mode gui framework designed for people
that begins their journey with game hacking (but not only).

#### Preview:
<img src="https://i.imgur.com/rHzQiLd.gif" width="500" height="350" />


### How to use & example
The core thing that you need to do is to wrap some rendering functions.

At first, you should specify window name from where the input will be read from (top of `zgui.cc`)

```cpp
#define INPUT_WINDOW "Window name"
```

In top of `hooks.cc` (whatever) create some proxy functions.
Functions used in this example are from valve_sdk (but their can be from wherever you want)

```cpp
void line(int x, int y, int x2, int y2, Color c) { g_pSurface->Line(x, y, x2, y2, c); }
void rect(int x, int y, int x2, int y2, Color c) { g_pSurface->OutlinedRect(x, y, x2, y2, c); }
void filled_rect(int x, int y, int x2, int y2, Color c) { g_pSurface->FilledRect(x, y, x2, y2, c); }
void text(int x, int y, Color color, int font, bool center, const char* _input, ...) { g_pSurface->DrawT(x, y, color, font, center, _input); }
void get_text_size(unsigned long font, const wchar_t* text, int& wide, int& tall) { g_pSurface->GetTextSize(font, text, wide, tall); }
void get_frametime(float& frametime) { frametime = g_pGlobalVars->frametime; };
```
then in your hooks initialize function
```cpp
auto draw_functions = g_zgui->get_functions();

draw_functions.draw_line = (zgui::line_t)line;
draw_functions.draw_rect = (zgui::rect_t)rect;
draw_functions.draw_filled_rect = (zgui::filled_rect_t)filled_rect;
draw_functions.draw_text = (zgui::text_t)text;
draw_functions.get_text_size = (zgui::get_text_size_t)get_text_size;
draw_functions.get_frametime = (zgui::get_frametime)get_frametime;

g_zgui->set_functions(draw_functions);
  ```
and finally, you can render the gui, here's short demo
```cpp
static bool example = false;
if (g_zgui->begin_window("zgui example window", zgui::vec2{ 500, 350 }, g::Tahoma))
{
  g_zgui->checkbox("sample checkbox", &example);
}
g_zgui->end_window();
```

### Closing speech
Special thanks to [cyanidee](https://github.com/cyanidee) for explaining me loads of things.
... and all the community contributors.
