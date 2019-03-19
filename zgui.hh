#pragma once

#include <stack>
#include <vector>
#include <string>
#include <string_view>

// zgui by zxvnme (https://github.com/zxvnme) and all the community contributors
#define ZGUI_VER "1.4.0" // the number after second dot is snapshot version.
/* ================================================================================================
 *
 * zgui is an simple framework created to help people with GUI rendering during their game hacking (but not only) journey.
 * here is glance zgui feature presentation:
 *	- easy to use immediate mode rendering (all you need is to include zgui header and source files to your project).
 *	- simple and aesthetic design.
 *	- extensible code.
 *	... and function documentation in section below:
 *
 *  ================================================================================================
 *
 *  get_functions()
 *  -- function that is used to get our wrapped ones.
 *
 *  begin_window(std::string_view title, vec2 default_size, unsigned long font, int flags);
 *  end_window();
 *  -- functions used to create and end window.
 *
 *  begin_groupbox(std::string_view name, vec2 size);
 *  end_groupbox();
 *  -- functions uses to create our groupbox with desired size and end it.
 *
 *  slider_int(std::string_view id, int min, int max, int* value);
 *  slider_float(std::string_view id, float min, float max, float* value);
 *  -- functions used to create sliders with value type described in function name.
 *
 *  combobox(std::string_view id, std::vector<std::string> items, int* value);
 *  multi_combobox(std::string id, std::vector<multi_combobox_item> items)
 *  -- functions used for creating combo boxes.
 *
 *  checkbox(std::string_view id, bool* value);
 *  -- function that creates checkbox.
 *
 *  toggle_button(std::string_view id, vec2 size, bool* value);
 *  -- function that creates toggle button.
 *
 *  button(std::string_view id, vec2 size);
 *  -- function that creates button.
 *
 *  key_bind(std::string_view id, int* value);
 *  -- function that creates key binder.
 *
 *  text_input(std::string_view id, std::string* value, int max_length = 18);
 *  -- functions that creates text input box.
 *
 *  clickable_text(std::string_view text);
 *  -- function that creates text that can be clicked and eventually perform an action.
 *
 *  text(std::string_view text);
 *  -- function that creates text.
 *
 *  dummy();
 *  -- function that pushes cursor_pos.x to make empty space between our controls.
 *
 *  same_line(float x_axis = -1);
 *  backup_line();
 *  -- functions used for inline controls positioning. NOTE: there can be only one item between these two functions called.
 *
 *  ================================================================================================
 *  the '#' thing in control name is separator that splits our name to two elements; actual displayed name & the one that is "hidden"
 *
 *  example:
 *    g_zgui->button("button", vec2{ 120, 30 });
 *    g_zgui->button("button", vec2{ 120, 30 });
 *
 *  code above won't work correctly because of same name provided. (known consequence is clicking 2 buttons at once)
 *
 *  example:
 *    g_zgui->button("button#button_1", vec2{ 120, 30 });
 *    g_zgui->button("button#button_2", vec2{ 120, 30 });
 *
 *  and now, above code works fine because unique id (used in window input blocking) is provided after '#'
*/


// For examples and function descriptions see zgui header file.
namespace zgui {

	// Multi combobox item.
	static struct multi_combobox_item { std::string name; bool *value; };
	// Two dimensional vector.
	static struct vec2 { float x, y; };
	// Color with 4 paremeters; red, green, blue and alpha.
	static struct color { int r, g, b, a; };

	/// "Proxy" functions definitions.
	using line_t = void(*)(int, int, int, int, color);
	using rect_t = void(*)(int, int, int, int, color);
	using filled_rect_t = void(*)(int, int, int, int, color);
	using text_t = void(*)(int, int, color, int, bool, const char*, ...);
	using get_text_size_t = void(*)(unsigned long, const wchar_t*, int&, int&);
	using get_frametime = float(*)();
	///

	// Flags for window appereance and its behavior.
	// ex: (zgui_window_flags_no_border | zgui_window_flags_no_titlebar) will cause window to be borderless and without title bar.
	enum zgui_window_flags {
		zgui_window_flags_none = 0,
		zgui_window_flags_no_border = 1 << 0,
		zgui_window_flags_no_titlebar = 1 << 1,
		zgui_window_flags_no_ontoggle_animation = 1 << 2,
		zgui_window_flags_no_move = 1 << 3
	};

	// Flags for text input appereance.
	// ex: (zgui_text_input_flags_password) will convert text input (ex: "abcdef") to "******"
	enum zgui_text_input_flags
	{
		zgui_text_input_flags_none = 0,
		zgui_text_input_flags_password = 1 << 0
	};

	// Color definition. Can be changed at any time just simply by editing this struct.
	static struct stylecolors_t {
		color window_border_inner_fill{ 60, 60, 60, 255 };
		color window_border_fill{ 40, 40, 40, 255 };
		color window_border_color{ 10, 10, 10, 255 };
		color window_background{ 40, 40, 40, 255 };

		color control_outline{ 23, 23, 30, 255 };
		color control_active_or_clicked{ 108, 92, 231, 255 };
		color control_idle{ 62, 62, 72, 255 };

		color color_groupbox_bg{ 50, 50, 50, 255 };
		color color_text{ 203, 203, 203, 255 };
		color color_text_dimmer{ 99, 110, 114, 255 };
		color color_slider{ 108, 92, 231, 255 };
		color color_combo_bg{ 108, 92, 231, 255 };
	} global_colors;

	// Window context.
	struct gui_window_context_t {
		size_t blocking;
		std::stack<vec2> cursor_pos;
		std::string _ /* title */;
		vec2 position, size;
		vec2 next_cursor_pos;
		bool dragging;
		bool opened;
		int font;
		int alpha;
	};

	// Window definitions.
	static struct gui_context_t {
		gui_window_context_t window;
	} context;

	// "Proxy" functions stuff...
	static struct functions_t {
		line_t draw_line;
		rect_t draw_rect;
		filled_rect_t draw_filled_rect;
		text_t draw_text;
		get_text_size_t get_text_size;
		get_frametime get_frametime;
	} functions;

	// Get "Proxy" functions
	functions_t& get_functions() noexcept;

	// Globals
	static vec2 mouse_pos;
	static vec2 previous_mouse_pos;

	// Input handling stuff
	static bool key_state[256];
	static bool prev_key_state[256];

	// Function for starting our input loop.
	void poll_input() noexcept;

	// Input utilities.
	bool key_pressed(int key) noexcept;
	bool key_down(int key) noexcept;
	bool key_released(int key) noexcept;

	// Positioning
	void push_cursor_pos(vec2 pos) noexcept;
	vec2 pop_cursor_pos() noexcept;

	// Check if mouse is hovered over specified region.
	bool mouse_in_region(int x, int y, int w, int h) noexcept;

	bool begin_window(std::string_view title, vec2 default_size, unsigned long font, int flags = 0) noexcept;
	void end_window() noexcept;

	void begin_groupbox(std::string_view title, vec2 size) noexcept;
	void end_groupbox() noexcept;

	void slider_int(std::string_view id, int min, int max, int& value) noexcept;
	void slider_float(std::string_view id, float min, float max, float& value) noexcept;

	void combobox(std::string_view, std::vector<std::string> items, int& value) noexcept;
	void multi_combobox(std::string_view id, std::vector<multi_combobox_item> items) noexcept;

	void checkbox(std::string_view id, bool& value) noexcept;
	void toggle_button(std::string_view id, vec2 size, bool& value) noexcept;
	bool button(std::string_view id, vec2 size) noexcept;

	void key_bind(std::string_view id, int& value) noexcept;
	void text_input(std::string_view id, std::string& value, int max_length = 16, int flags = 0) noexcept;

	bool clickable_text(std::string_view id) noexcept;
	void text(std::string_view id) noexcept;
	void dummy() noexcept;

	void next_column(int pusher_x = 174, int pusher_y = 14) noexcept;

	void same_line(float x_axis = -1) noexcept;
	void backup_line() noexcept;
};