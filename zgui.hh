#pragma once

#include <stack>
#include <vector>
#include <string>
#include <string_view>

// zgui by zxvnme (https://github.com/zxvnme) and all the community contributors
#define ZGUI_VER "1.4.1" // the number after second dot is snapshot version.
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
 *  multi_combobox(std::string id, std::vector<multi_select_item> items)
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
 *    zgui::button("button", { 120, 30 });
 *    zgui::button("button", { 120, 30 });
 *
 *  code above won't work correctly because of same name provided. (known consequence is clicking 2 buttons at once)
 *
 *  example:
 *    zgui::button("button#button_1", { 120, 30 });
 *    zgui::button("button#button_2", { 120, 30 });
 *
 *  and now, above code works fine because unique id (used in window input blocking) is provided after '#'
*/


// For examples and function descriptions see zgui header file.
namespace zgui {

	// Multi selectable item.
	struct multi_select_item { std::string_view name; bool *value; };
	// Two dimensional vector.
	struct vec2 { float x, y; };
	// Color with 4 paremeters; red, green, blue and alpha.
	struct color { int r, g, b, a; };

	/// "Proxy" functions definitions.
	using line_t = void(*)(int, int, int, int, color) noexcept;
	using rect_t = void(*)(int, int, int, int, color) noexcept;
	using filled_rect_t = void(*)(int, int, int, int, color) noexcept;
	using text_t = void(*)(int, int, color, int, bool, const char*, ...) noexcept;
	using get_text_size_t = void(*)(unsigned long, const wchar_t*, int&, int&) noexcept;
	using get_frametime = float(*)() noexcept;
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

	// "Proxy" functions stuff...
	struct functions_t {
		line_t draw_line;
		rect_t draw_rect;
		filled_rect_t draw_filled_rect;
		text_t draw_text;
		get_text_size_t get_text_size;
		get_frametime get_frametime;
	};

	extern functions_t functions;

	// Get "Proxy" functions
	constexpr functions_t& get_functions() noexcept
	{
		return functions;
	}

	bool begin_window(std::string_view title, vec2 default_size, unsigned long font, int flags = 0) noexcept;
	void end_window() noexcept;

	void begin_groupbox(std::string_view title, vec2 size) noexcept;
	void end_groupbox() noexcept;

	void slider_int(const char* id, int min, int max, int& value) noexcept;
	void slider_float(const char* id, float min, float max, float& value) noexcept;

	void combobox(const char*, std::vector<std::string> items, int& value) noexcept;
	void multi_combobox(const char* id, std::vector<multi_select_item> items) noexcept; 

	void listbox(const char* id, std::vector<multi_select_item> items) noexcept;

	void checkbox(std::string_view id, bool& value) noexcept;
	void toggle_button(const char* id, vec2 size, bool& value) noexcept;
	bool button(const char* id, vec2 size) noexcept;

	void key_bind(const char* id, int& value) noexcept;
	void text_input(const char* id, std::string& value, int max_length = 16, int flags = 0) noexcept;

	bool clickable_text(const char* id) noexcept;
	void text(std::string_view id) noexcept;
	void dummy() noexcept;

	void next_column(int pusher_x = 174, int pusher_y = 14) noexcept;

	void same_line(float x_axis = -1) noexcept;
	void backup_line() noexcept;
}
