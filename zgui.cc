#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <Windows.h>

#include "zgui.hh"
// zgui by zxvnme (https://github.com/zxvnme)
// heres defines that are designed to be modified by your preferences.
// see zgui.hh for complete documentation.
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Color definition. Can be changed at any time just simply by editing this struct.
static struct {
	zgui::color window_border_inner_fill{ 60, 60, 60, 255 };
	zgui::color window_border_fill{ 40, 40, 40, 255 };
	zgui::color window_border_color{ 10, 10, 10, 255 };
	zgui::color window_background{ 40, 40, 40, 255 };

	zgui::color control_outline{ 23, 23, 30, 255 };
	zgui::color control_active_or_clicked{ 108, 92, 231, 255 };
	zgui::color control_idle{ 62, 62, 72, 255 };

	zgui::color color_groupbox_bg{ 50, 50, 50, 255 };
	zgui::color color_text{ 203, 203, 203, 255 };
	zgui::color color_text_dimmer{ 99, 110, 114, 255 };
	zgui::color color_slider{ 108, 92, 231, 255 };
	zgui::color color_combo_bg{ 108, 92, 231, 255 };
} global_colors;

static struct {
	// Base position of first drawn control (px). DO NOT change if its necessary
	zgui::vec2 base_pos{ 16, 23 };
	// Spacing between items (px)
	int item_spacing = 16;
	// Key that will toggle menu visibility unless zgui_window_flags_always_open is set
	int menu_toggle_key = VK_INSERT;
} global_config;

// Window definitions.
static struct gui_context_t {
	zgui::gui_window_context_t window;
} context;

// "Proxy" functions stuff...
zgui::functions_t zgui::functions;

// Globals
static zgui::vec2 mouse_pos;
static zgui::vec2 previous_mouse_pos;


// Input handling stuff
static bool key_state[256];
static bool prev_key_state[256];

// Check for input polling.
static bool input_loop_started = false;

// Function for starting our input loop.
void zgui::poll_input(std::string_view window_name)
{
	if (window_name.empty())
		throw std::exception("No window from where input should be read from specified in function parameter.");

	for (int i = 0; i < 256; i++) {
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
	}

	POINT p_mouse_pos;
	GetCursorPos(&p_mouse_pos);
	ScreenToClient(FindWindow(nullptr, window_name.data()), &p_mouse_pos);
	previous_mouse_pos = mouse_pos;
	mouse_pos = vec2{ static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y) };

	if (!input_loop_started)
		input_loop_started = true;
}

// Function for starting our input loop.
void zgui::poll_input(HWND hwnd)
{
	if (!hwnd)
		throw std::exception("No window from where input should be read from specified in function parameter.");

	for (int i = 0; i < 256; i++) {
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
	}

	POINT p_mouse_pos;
	GetCursorPos(&p_mouse_pos);
	ScreenToClient(hwnd, &p_mouse_pos);
	previous_mouse_pos = mouse_pos;
	mouse_pos = vec2{ static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y) };

	if (!input_loop_started)
		input_loop_started = true;
}

// Input utilities.
constexpr bool key_pressed(const int key) noexcept
{
	return key_state[key] && !prev_key_state[key];
}

constexpr bool key_down(const int key) noexcept
{
	return key_state[key];
}

constexpr bool key_released(const int key) noexcept
{
	return !key_state[key] && prev_key_state[key];
}

// Check if mouse is hovered over specified region.
bool mouse_in_region(const int x, const int y, const int w, const int h) noexcept
{
	return mouse_pos.x > x && mouse_pos.y > y && mouse_pos.x < w + x && mouse_pos.y < h + y;
}

// Push cursor position to the stack
void zgui::push_cursor_pos(const vec2 pos) noexcept
{
	context.window.cursor_pos.push(pos);
}

// Pop cursor position from the stack
zgui::vec2 zgui::pop_cursor_pos() noexcept
{
	const vec2 pos = context.window.cursor_pos.top();
	context.window.cursor_pos.pop();
	return pos;
}

// Hashing util
std::vector<std::string> split_str(const char* str, const char separator) noexcept
{
	std::vector<std::string> output;
	std::string substring;
	std::istringstream stream{ str };

	while (std::getline(stream, substring, separator))
		output.push_back(substring);

	return output;
}

static constexpr auto hash(const char* str) noexcept
{
	uint32_t hash = 2166136261;
	for (; *str; str++)
		hash = (hash ^ *str) * 16777619;
	return hash;
}

// Names for each of VKs
constexpr std::string_view keys_list[]{
"Error", "Left Mouse", "Right Mouse", "Break", "Middle Mouse", "Mouse 4", "Mouse 5",
"Error", "Backspace", "TAB", "Error", "Error", "Error", "ENTER", "Error", "Error", "SHIFT",
"CTRL", "ALT","PAUSE","CAPS LOCK", "Error", "Error", "Error", "Error", "Error", "Error",
"Error", "Error", "Error", "Error", "Error", "SPACEBAR","PG UP", "PG DOWN", "END", "HOME", "Left",
"Up", "Right", "Down", "Error", "Print", "Error", "Print Screen", "Insert","Delete", "Error", "0", "1",
"2", "3", "4", "5", "6", "7", "8", "9", "Error", "Error", "Error", "Error", "Error", "Error",
"Error", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
"V", "W", "X","Y", "Z", "Left Windows", "Right Windows", "Error", "Error", "Error", "NUM 0", "NUM 1",
"NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6","NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12","F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21",
"F22", "F23", "F24", "Error", "Error", "Error", "Error", "Error","Error", "Error", "Error",
"NUM LOCK", "SCROLL LOCK", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
"Error", "Error","Error", "Error", "Error", "Error", "Error", "LSHIFT", "RSHIFT", "LCONTROL",
"RCONTROL", "LMENU", "RMENU", "Error","Error", "Error","Error", "Error", "Error", "Error",
"Error", "Error", "Error", "Next Track", "Previous Track", "Stop", "Play/Pause", "Error", "Error",
"Error", "Error", "Error", "Error", ";", "+", ",", "-", ".", "/?", "~", "Error", "Error",
"Error", "Error","Error", "Error", "Error", "Error", "Error", "Error", "Error",
"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error","Error",
"Error", "Error", "Error", "Error", "Error", "Error", "[{", "\\|", "}]", "'\"", "Error",
"Error", "Error", "Error","Error", "Error", "Error", "Error", "Error", "Error",
"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
"Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error", "Error",
"Error", "Error"
};

struct key_code_info {
	int vk;

	char regular;
	char shift;
};

static key_code_info special_characters[22] = {
	{ 48,  '0',  ')' },
	{ 49,  '1',  '!' },
	{ 50,  '2',  '@' },
	{ 51,  '3',  '#' },
	{ 52,  '4',  '$' },
	{ 53,  '5',  '%' },
	{ 54,  '6',  '^' },
	{ 55,  '7',  '&' },
	{ 56,  '8',  '*' },
	{ 57,  '9',  '(' },
	{ 32,  ' ',  ' ' },
	{ 192, '`',  '~' },
	{ 189, '-',  '_' },
	{ 187, '=',  '+' },
	{ 219, '[',  '{' },
	{ 220, '\\', '|' },
	{ 221, ']',  '}' },
	{ 186, ';',  ':' },
	{ 222, '\'', '"' },
	{ 188, ',',  '<' },
	{ 190, '.',  '>' },
	{ 191, '/',  '?' }
};

bool zgui::begin_window(std::string_view title, const vec2 default_size, const unsigned long font, const int flags)
{
	if (!input_loop_started)
		throw std::exception("Input loop didnt start or didnt start properly.");;

	context.window.font = font;

	if (!(flags & zgui_window_flags_always_open))
	{
		if (key_pressed(global_config.menu_toggle_key))
			context.window.opened = !context.window.opened;
	}
	else
		context.window.opened = true;


	if (const int prev_alpha = context.window.alpha; !(flags & zgui_window_flags_no_ontoggle_animation))
	{
		const int fade_factor = static_cast<int>(1.0f / 0.15f * functions.get_frametime() * 255);
		context.window.alpha = std::clamp(context.window.alpha + (context.window.opened ? fade_factor : -fade_factor), 0, 255);

		if (context.window.alpha != prev_alpha)
		{
			global_colors.window_border_inner_fill.a = context.window.alpha;
			global_colors.window_border_fill.a = context.window.alpha;
			global_colors.window_border_color.a = context.window.alpha;
			global_colors.window_background.a = context.window.alpha;

			global_colors.control_outline.a = context.window.alpha;
			global_colors.control_active_or_clicked.a = context.window.alpha;
			global_colors.control_idle.a = context.window.alpha;

			global_colors.color_groupbox_bg.a = context.window.alpha;
			global_colors.color_text.a = context.window.alpha;
			global_colors.color_text_dimmer.a = context.window.alpha;
			global_colors.color_slider.a = context.window.alpha;
		}
	}

	if (context.window.opened || context.window.alpha > 0)
	{
		if (!(flags & zgui_window_flags_no_move))
		{
			if ((flags & zgui_window_flags_no_border ? mouse_in_region(context.window.position.x + 9, context.window.position.y + 14, context.window.size.x - 18, 14)
				: mouse_in_region(context.window.position.x - 6, context.window.position.y - 10, context.window.size.x + 12, 16))
				&& key_pressed(VK_LBUTTON) && !context.window.dragging)
			{
				context.window.dragging = true;
			}
			else if (key_down(VK_LBUTTON) && context.window.dragging)
			{
				const vec2 mouse_delta{ mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y };
				const vec2 new_position{ context.window.position.x + mouse_delta.x, context.window.position.y + mouse_delta.y };

				context.window.position = new_position;
			}
			else if (!key_down(VK_LBUTTON) && context.window.dragging)
			{
				context.window.dragging = false;
			}
		}

		if (context.window.size.x < 1 && context.window.size.y < 1)
			context.window.size = default_size;

		if (!(flags & zgui_window_flags_no_border))
		{
			functions.draw_filled_rect(context.window.position.x - 6, context.window.position.y - 10, context.window.size.x + 12, context.window.size.y + 16, global_colors.window_border_inner_fill);
			functions.draw_filled_rect(context.window.position.x - 5, context.window.position.y - 9, context.window.size.x + 10, context.window.size.y + 14, global_colors.window_border_color);
			functions.draw_filled_rect(context.window.position.x - 4, context.window.position.y - 8, context.window.size.x + 8, context.window.size.y + 12, global_colors.window_border_fill);
			functions.draw_filled_rect(context.window.position.x, context.window.position.y + 7, context.window.size.x, context.window.size.y - 7, global_colors.window_border_color);
			functions.draw_filled_rect(context.window.position.x + 1, context.window.position.y + 8, context.window.size.x - 2, context.window.size.y - 9, global_colors.window_border_inner_fill);
			functions.draw_filled_rect(context.window.position.x + 8, context.window.position.y + 15, context.window.size.x - 16, context.window.size.y - 23, global_colors.window_border_color);
		}

		if (!(flags & zgui_window_flags_no_titlebar))
			functions.draw_text(context.window.position.x + context.window.size.x * 0.5, context.window.position.y + (context.window.size.y * 0.010) - 10, global_colors.color_text, context.window.font, true, title.data());

		functions.draw_filled_rect(context.window.position.x + 9, context.window.position.y + 16, context.window.size.x - 18, context.window.size.y - 25, global_colors.window_background);

		push_cursor_pos(global_config.base_pos);
	}

	return context.window.opened || context.window.alpha > 0;
}

void zgui::end_window() noexcept
{
	for (int i = context.window.render.size() - 1; i >= 0; i--)
	{
		switch (context.window.render[i].render_type)
		{
		case zgui_render_type::zgui_line:
			functions.draw_line(context.window.render[i].draw_position.x, context.window.render[i].draw_position.y, context.window.render[i].size.x, context.window.render[i].size.y, context.window.render[i].color);
			break;
		case zgui_render_type::zgui_rect:
			functions.draw_rect(context.window.render[i].draw_position.x, context.window.render[i].draw_position.y, context.window.render[i].size.x, context.window.render[i].size.y, context.window.render[i].color);
			break;
		case zgui_render_type::zgui_filled_rect:
			functions.draw_filled_rect(context.window.render[i].draw_position.x, context.window.render[i].draw_position.y, context.window.render[i].size.x, context.window.render[i].size.y, context.window.render[i].color);
			break;
		case zgui_render_type::zgui_text:
			functions.draw_text(context.window.render[i].draw_position.x, context.window.render[i].draw_position.y, context.window.render[i].color, context.window.font, false, context.window.render[i].text.c_str());
			break;
		}
	}

	context.window.render.clear();

	while (!context.window.cursor_pos.empty())
	{
		context.window.cursor_pos.pop();
	}
}

void zgui::begin_groupbox(std::string_view title, const vec2 size, const int flags) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

	if (title.length() > 0)
	{
		if (!(flags & zgui_groupbox_flags_title_centered))
		{
			functions.draw_text(draw_pos.x + 4, draw_pos.y - 8, global_colors.color_text, context.window.font, false, title.data());
		}
		else
		{
			int text_wide, text_tall;
			functions.get_text_size(context.window.font, title.data(), text_wide, text_tall);
			functions.draw_text(draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y - 8, global_colors.color_text, context.window.font, false, title.data());
		}
	}

	context.window.next_cursor_pos = vec2{ cursor_pos.x, cursor_pos.y + size.y + 10 };

	push_cursor_pos(vec2{ cursor_pos.x + 8, cursor_pos.y + 14 });
}

void zgui::end_groupbox() noexcept
{
	push_cursor_pos(context.window.next_cursor_pos);
	context.window.next_cursor_pos = { };
}

void zgui::checkbox(const char* id, bool& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_height = 8;
	const int control_width = 8;

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_wide, text_tall;
	functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

	const bool active = context.window.blocking == hash(id);

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width + 6 + text_wide, control_height); !active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = hash(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		value = !value;
	}

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 14, draw_pos.y - 2 }, zgui_render_type::zgui_text, value ? global_colors.color_text : global_colors.color_text_dimmer, id_split[0].c_str() });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width, control_height } });

	push_cursor_pos(vec2{ cursor_pos.x + 14 + text_wide + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + global_config.item_spacing });
}

void zgui::toggle_button(const char* id, const vec2 size, bool& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool active = context.window.blocking == hash(id);

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = hash(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		value = !value;
	}

	int text_wide, text_tall;
	functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y + size.y / 2 - text_tall / 2 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", size });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { size.x + 2, size.y + 2 } });

	push_cursor_pos(vec2{ cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing });
}

bool zgui::button(const char* id, const vec2 size) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool active = context.window.blocking == hash(id);

	bool result = false;
	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = hash(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		result = hovered;
	}

	int text_wide, text_tall;
	functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y + size.y / 2 - text_tall / 2 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {size.x - 2, size.y - 2} });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", size });

	push_cursor_pos(vec2{ cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing });

	return result;
}

void zgui::key_bind(const char* id, int& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 80;
	const int control_height = 20;

	value = std::clamp(value, 0, 255);

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0].c_str() });

		draw_pos.y += text_tall;
	}

	const bool active = context.window.blocking == hash(id);

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = hash(id);
	}
	else if (active)
	{
		for (int i = 0; i < 256; i++)
		{
			if (key_pressed(i))
			{
				if (keys_list[i] != "Error")
					value = i;

				context.window.blocking = 0;
			}
		}
	}

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text, active ? "Press any key" : keys_list[value].data() });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", { control_width, control_height } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width + 2, control_height + 2 } });

	push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });
}

void zgui::text_input(const char* id, std::string& value, const int max_length, const int flags) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 80;
	const int control_height = 20;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });

		draw_pos.y += text_tall;
	}

	const bool active = context.window.blocking == hash(id);
	const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);

	if (hovered && key_pressed(VK_LBUTTON) && !active)
	{
		context.window.blocking = hash(id);
	}
	else if (active)
	{
		if (key_pressed(VK_ESCAPE) || key_pressed(VK_RETURN) || (!hovered && key_pressed(VK_LBUTTON)))
		{
			context.window.blocking = 0;
		}
		else if (key_pressed(VK_BACK) && !value.empty())
		{
			value.pop_back();
		}
		else if (value.length() < max_length)
		{
			for (int i = 32; i <= 222; i++)
			{
				if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 186))
					continue;

				if (i > 57 && i <= 90)
				{
					if (key_pressed(i))
						value += key_down(VK_SHIFT) ? static_cast<char>(i) : static_cast<char>(i + 32);
				}
				else
				{
					if (key_pressed(i))
					{
						for (int j = 0; j < sizeof(special_characters); j++)
						{
							if (special_characters[j].vk == i)
								value += key_down(VK_SHIFT) ? special_characters[j].shift : special_characters[j].regular;
						}
					}
				}
			}
		}
	}

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text,  global_colors.color_text, flags & zgui_text_input_flags_password ? std::string(value.length(), '*').c_str() : value.c_str() });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width, control_height } });

	push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });
}

void zgui::slider_int(const char* id, const int min, const int max, int& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 120;
	const int control_height = 10;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });

		draw_pos.y += text_tall;
	}

	if (context.window.blocking == 0 && mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value - 1, min, max);
	else if (context.window.blocking == 0 && mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value + 1, min, max);

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = hash(id);
	}
	else if (key_down(VK_LBUTTON) && context.window.blocking == hash(id))
	{
		float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
		int value_mapped = static_cast<int>(value_unmapped / control_width * (max - min) + min);

		value = value_mapped;
	}
	else if (!key_down(VK_LBUTTON) && context.window.blocking == hash(id))
	{
		context.window.blocking = 0;
	}

	const int dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

	int text_wide, text_tall;
	std::string value_str = std::to_string(value);
	functions.get_text_size(context.window.font, value_str.c_str(), text_wide, text_tall);

	int text_x = dynamic_width - text_wide;

	if (text_x < 0)
		text_x = 0;

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x - (control_height - 2), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "-" });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + (control_width + 4), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "+" });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + text_x, draw_pos.y }, zgui_render_type::zgui_text, global_colors.color_text, value_str });

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.color_slider, "", { static_cast<float>(dynamic_width), control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle,"", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width, control_height } });


	push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });
}

void zgui::slider_float(const char* id, const float min, const float max, float& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 120;
	const int control_height = 10;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });

		draw_pos.y += text_tall;
	}

	if (context.window.blocking == 0 && mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value - 1, min, max);
	else if (context.window.blocking == 0 && mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value + 1, min, max);

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = hash(id);
	}
	else if (key_down(VK_LBUTTON) && context.window.blocking == hash(id))
	{
		float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
		float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(control_width)) * (max - min) + min);

		value = value_mapped;
	}
	else if (!key_down(VK_LBUTTON) && context.window.blocking == hash(id))
	{
		context.window.blocking = 0;
	}

	const float dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

	int text_wide, text_tall;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << value;
	std::string value_str = ss.str();
	functions.get_text_size(context.window.font, value_str.c_str(), text_wide, text_tall);

	int text_x = dynamic_width - text_wide;

	if (text_x < 0)
		text_x = 0;

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x - (control_height - 2), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "-" });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + (control_width + 4), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "+" });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + text_x, draw_pos.y }, zgui_render_type::zgui_text, global_colors.color_text, value_str });

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.color_slider, "", { dynamic_width,  control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle,"", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width, control_height } });


	push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });
}

void zgui::combobox(const char* id, std::vector<std::string>items, int& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 70;
	const int control_height = 20;

	value = std::clamp(value, 0, static_cast<int>(items.size()) - 1);

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;

		functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });

		draw_pos.y += text_tall;
	}

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + control_width - 10, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  "+" });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  items.at(value) });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x,  draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width,  control_height } });

	push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = hash(id);
	}
	else if (context.window.blocking == hash(id))
	{
		for (int i = 1; i <= items.size(); i++)
		{
			bool hovered = mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

			if (hovered && key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
				value = i - 1;
			}

			if (!hovered && key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
			}

			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4 }, zgui_render_type::zgui_text, global_colors.color_text, items.at(i - 1) });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + (19 * i) + 1 }, zgui_render_type::zgui_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y + 19 * i }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width, control_height } });
		}
	}
}

void zgui::multi_combobox(const char* id, std::vector<multi_select_item> items) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 100;
	const int control_height = 20;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });

		draw_pos.y += text_tall;
	}

	std::string value_str;
	int text_wide, text_tall;

	for (auto &item_t : items) {
		if (*item_t.value) {
			if (value_str.length() > 0)
				value_str += ", ";

			value_str += item_t.name;
		}
	}

	functions.get_text_size(context.window.font, value_str.c_str(), text_wide, text_tall);
	if (text_wide > control_width - 18)
	{
		value_str.resize(control_width / 10);
		value_str += " ...";
	}
	if (!value_str.length())
		value_str += "None";

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + control_width - 10, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  "+" });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  value_str });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x,  draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width,  control_height } });

	push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });


	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = hash(id);
	}
	else if (context.window.blocking == hash(id))
	{
		for (int i = 1; i <= items.size(); i++)
		{
			bool hovered = mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

			if (hovered && key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
				*items[i - 1].value = !*items[i - 1].value;
			}
			if (!hovered && key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
			}

			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4 }, zgui_render_type::zgui_text, global_colors.color_text, items[i - 1].name.data() });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + (19 * i) + 1 }, zgui_render_type::zgui_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y + 19 * i }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width, control_height } });
		}
	}
}

void zgui::listbox(const char* id, std::vector<multi_select_item> items) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 100;
	const int control_height = 20;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(context.window.font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0] });

		draw_pos.y += text_tall;
	}

	for (int i = 1; i <= items.size(); i++)
	{
		const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * (i - 1), control_width, control_height);

		if (hovered && key_pressed(VK_LBUTTON))
		{
			context.window.blocking = 0;
			*items[i - 1].value = !*items[i - 1].value;
		}
		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + (control_height - 1) * (i - 1) + 4}, zgui_render_type::zgui_text, *items[i - 1].value || hovered ? global_colors.control_active_or_clicked : global_colors.color_text, items[i - 1].name.data() });
	}


	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", { control_width - 2, static_cast<float>(control_height * items.size() - 2) } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x,  draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width, static_cast<float>(control_height * items.size()) } });

	push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) + control_height * (items.size() - 1) });

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = hash(id);
	}

}

bool zgui::clickable_text(const char* id) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_tall;
	functions.get_text_size(context.window.font, id_split[0].c_str(), text_width, text_tall);

	const bool active = context.window.blocking == hash(id);
	const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_tall);

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y}, zgui_render_type::zgui_text, (hovered || context.window.blocking == hash(id)) ? global_colors.control_active_or_clicked : global_colors.color_text, id_split[0] });

	push_cursor_pos(vec2{ cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_tall / 2 + global_config.item_spacing });

	bool result = false;

	if (!active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = hash(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		result = hovered;
	}

	return result;
}

void zgui::text(const char* text) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_tall;
	functions.get_text_size(context.window.font, text, text_width, text_tall);

	functions.draw_text(draw_pos.x, draw_pos.y, global_colors.color_text, context.window.font, false, text);

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_text, global_colors.color_text, text });

	push_cursor_pos(vec2{ cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_tall / 2 + global_config.item_spacing });
}

void zgui::dummy() noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + global_config.item_spacing });
}

void zgui::next_column(const int pusher_x, const int pusher_y) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	vec2 new_cursor_pos{ cursor_pos.x + pusher_x, global_config.base_pos.y + pusher_y };

	if (context.window.next_cursor_pos.y != 0)
		new_cursor_pos.y += 14;

	push_cursor_pos(new_cursor_pos);
}

void zgui::same_line(const float x_axis) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();

	if (x_axis != -1)
		push_cursor_pos(vec2{ global_config.base_pos.x + x_axis, cursor_pos.x });
}

void zgui::backup_line() noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();

	push_cursor_pos(vec2{ context.window.next_cursor_pos.x, cursor_pos.y });
}