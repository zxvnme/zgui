#pragma once

#include <Windows.h>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>

// zgui by zxvnme (https://github.com/zxvnme)
#define ZGUI_VER "1.0.2" // the number after second dot is snapshot version.
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// zgui is an simple framework created to help people with GUI rendering during their game hacking (but not only) journey.
// here is quick presentation of features that zgui has:
// - easy to use immediate mode rendering (all you need is to include zgui header and source files to your project).
// - simple and aesthetic design.
// - extensible code.
// ... and function documentation:
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// get_functions()																			- function that is used to get our wrapped ones.
// set_functions(functions_t& functions)													- function that is used to set our wrapped drawing functions.
//
// begin_window(std::string title, vec2 default_size, unsigned long font, int flags);		- function that begins our window. (you need to call end_window(); after it!)
// end_window();																			- function that ends our window.
//
// begin_groupbox(std::string name, vec2 size);												- function that begins our groupbox with desired size. (you need to call end_groupbox(); after it!)
// end_groupbox();																			- function that ends our groupbox.
//
// slider_int(std::string name, int min, int max, int* value);								- function that creates slider with an int type of value.
//
// combobox(std::string name, std::vector<std::string> items, int* value);					- function that creates combobox.
//
// checkbox(std::string name, bool* value);													- function that creates checkbox.
// toggle_button(std::string name, vec2 size, bool* value);									- function that creates toggle button.
// button(std::string name, vec2 size);														- function that creates button.
// clickable_text(std::string text);														- function that creates text that can be clicked and eventually perform an action.
// text(std::string text);																	- function that creates text.
// dummy();																					- function that pushes cursor_pos.x to make empty space between our controls.
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class zgui {
public:
  enum zgui_window_flags_ {
		zgui_window_flags_none = 0,
		zgui_window_flags_no_border = 1 << 0,
		zgui_window_flags_no_titlebar = 1 << 1,
	};

	struct multi_item { std::string name; bool *value; };
	struct vec2 { float x, y; };
	struct color { int r, g, b, a; };
  

	using line_t = void(*)(int, int, int, int, color);
	using rect_t = void(*)(int, int, int, int, color);
	using filled_rect_t = void(*)(int, int, int, int, color);
	using text_t = void(*)(int, int, color, int, bool, const char*, ...);
	using get_text_size_t = void(*)(unsigned long, const wchar_t*, int&, int&);
	using get_frametime = void(*)(float&);
	
private:
	struct stylecolors_t {
		color window_border_inner_fill{ 60, 60, 60, 255 };
		color window_border_fill{ 40, 40, 40, 255 };
		color window_border_color{ 10, 10, 10, 255 };
		color window_background{ 40, 40, 40, 255 };

		color control_outline{ 23, 23, 30, 255 };
		color control_active_or_clicked{ 108, 92, 231, 255 };
		color control_idle{ 62, 62, 72, 255 };

		color color_groupbox_bg{ 50, 50, 50, 255 };
		color color_text{ 203, 203, 203, 255 };
		color color_text_dark{ 99, 110, 114, 255 };
		color color_slider{ 108, 92, 231, 255 };
		color color_combo_bg{ 108, 92, 231, 255 };
	} global_colors;

	struct gui_window_context_t {
		size_t blocking;
		std::stack<vec2> cursor_pos;
		std::string title;
		vec2 position, size;
		vec2 next_cursor_pos;
		bool dragging;
		bool opened;
		int font;
		int alpha;
	};

	struct gui_context_t {
		gui_window_context_t window;
	} context;

	struct functions_t {
		line_t draw_line;
		rect_t draw_rect;
		filled_rect_t draw_filled_rect;
		text_t draw_text;
		get_text_size_t get_text_size;
		get_frametime get_frametime;
	} functions;

	vec2 mouse_pos;
	vec2 previous_mouse_pos;

	bool key_state[256];
	bool prev_key_state[256];

	void poll_input();

	bool key_pressed(int key);
	bool key_down(int key);
	bool key_released(int key);

	void push_cursor_pos(vec2 pos);
	vec2 pop_cursor_pos();

	bool mouse_in_region(int x, int y, int x2, int y2);

public:
	functions_t& get_functions();
	void set_functions(functions_t& functions);

	bool begin_window(std::string title, vec2 default_size, unsigned long font, int flags = 0);
	void end_window();

	void begin_groupbox(std::string name, vec2 size);
	void end_groupbox();

	void slider_int(std::string name, int min, int max, int* value);
	void slider_float(std::string name, float min, float max, float* value);
	
	void combobox(std::string name, std::vector<std::string> items, int* value);

	void multi_combobox(std::string name,std::vector<multi_item> items);
	
	void checkbox(std::string name, bool* value);
	void toggle_button(std::string name, vec2 size, bool* value);
	bool button(std::string name, vec2 size);
	bool clickable_text(std::string text);
	void text(std::string text);
	void dummy();

	void next_column(int pusher_x = 174, int pusher_y = 14);
	void same_line(float x_axis = -1);
};

extern zgui* g_zgui;
