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
constexpr zgui::vec2 BASE_POS{ 16, 23 }; // base position that first control will be created.
constexpr int ITEM_SPACING{ 16 }; // spacing between controls.
constexpr std::string_view INPUT_WINDOW{ "" }; // name of window where input will be read from
constexpr int MENU_TOGGLE_KEY{ VK_INSERT }; // virtual key that will trigger our gui open.
//
constexpr zgui::vec2 ZERO_VEC{ 0, 0 };

std::vector<std::string> split_str(std::string_view str, char separator)
{
	std::vector<std::string> output;
	std::string substring;
	std::istringstream stream{ str.data() };

	while (std::getline(stream, substring, separator))
		output.push_back(substring);

	return output;
}

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

zgui::functions_t& zgui::get_functions() noexcept
{
	return functions;
}

void zgui::push_cursor_pos(vec2 pos) noexcept
{
	context.window.cursor_pos.push(pos);
}

zgui::vec2 zgui::pop_cursor_pos() noexcept
{
	const vec2 pos = context.window.cursor_pos.top();
	context.window.cursor_pos.pop();
	return pos;
}

void zgui::poll_input() noexcept
{
	static_assert(INPUT_WINDOW.length(),
		"No window from where input should be read from specified (see defines on the top of zgui.cc file). Comment this if you are aware of this and its not an error."
	);

	for (int i = 0; i < 256; i++) {
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
	}

	POINT p_mouse_pos;
	GetCursorPos(&p_mouse_pos);
	ScreenToClient(FindWindow(nullptr, INPUT_WINDOW.data()), &p_mouse_pos);
	previous_mouse_pos = mouse_pos;
	mouse_pos = vec2{ static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y) };
}

bool zgui::begin_window(std::string_view title, const vec2 default_size, const unsigned long font, const int flags) noexcept
{
	context.window.font = font;

	if (key_pressed(MENU_TOGGLE_KEY))
		context.window.opened = !context.window.opened;

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

	poll_input();

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

		push_cursor_pos(BASE_POS);
	}

	return context.window.opened || context.window.alpha > 0;
}

void zgui::end_window() noexcept
{
	while (!context.window.cursor_pos.empty())
	{
		context.window.cursor_pos.pop();
	}
}

void zgui::begin_groupbox(std::string_view title, const vec2 size) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

	if (title.length() > 0)
		functions.draw_text(draw_pos.x + 4, draw_pos.y - 6, global_colors.color_text, context.window.font, false, title.data());

	context.window.next_cursor_pos = vec2{ cursor_pos.x, cursor_pos.y + size.y + 10 };

	push_cursor_pos(vec2{ cursor_pos.x + 8, cursor_pos.y + 14 });
}

void zgui::end_groupbox() noexcept
{
	push_cursor_pos(context.window.next_cursor_pos);
	context.window.next_cursor_pos = ZERO_VEC;
}

void zgui::checkbox(std::string_view id, bool& value) noexcept
{
	std::vector<std::string> id_split = split_str(id.data(), '#');

	const int control_height = 8;
	const int control_width = 8;

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, value ? global_colors.control_active_or_clicked : global_colors.control_idle);

	int text_wide, text_tall;
	std::wstring text{ id.begin(), id.end() };
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	const bool active = context.window.blocking == std::hash<std::string_view>()(id);

	functions.draw_text(draw_pos.x + 14, draw_pos.y - 2, value ? global_colors.color_text : global_colors.color_text_dimmer, context.window.font, false, id_split[0].c_str());

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width + 6 + text_wide, control_height); !active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		value = !value;
	}

	push_cursor_pos(vec2{ cursor_pos.x + 14 + text_wide + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + ITEM_SPACING });
}

void zgui::toggle_button(std::string_view id, const vec2 size, bool& value) noexcept
{
	std::vector<std::string> id_split = split_str(id.data(), '#');

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, size.x - 2, size.y - 2, value ? global_colors.control_active_or_clicked : global_colors.control_idle);

	int text_wide, text_tall;
	std::wstring text{ id_split[0].begin(), id_split[0].end() };
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	const bool active = context.window.blocking == std::hash<std::string_view>()(id);

	functions.draw_text(draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y + size.y / 2 - text_tall / 2, global_colors.color_text, context.window.font, false, id_split[0].c_str());

	push_cursor_pos(vec2{ cursor_pos.x + size.x + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + ITEM_SPACING });

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		value = !value;
	}
}

bool zgui::button(std::string_view id, const vec2 size) noexcept
{
	std::vector<std::string> id_split = split_str(id.data(), '#');

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool active = context.window.blocking == std::hash<std::string_view>()(id);

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, size.x - 2, size.y - 2, active ? global_colors.control_active_or_clicked : global_colors.control_idle);

	int text_wide, text_tall;
	std::wstring text{ id_split[0].begin(), id_split[0].end() };
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	functions.draw_text(draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y + size.y / 2 - text_tall / 2, global_colors.color_text, context.window.font, false, id_split[0].data());

	push_cursor_pos(vec2{ cursor_pos.x + size.x + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + ITEM_SPACING });

	bool result = false;
	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		result = hovered;
	}

	return result;
}

void zgui::key_bind(std::string_view id, int& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 80;
	const int control_height = 20;

	value = std::clamp(value, 0, 255);

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		std::wstring text{ id_split[0].begin(), id_split[0].end() };
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, global_colors.color_text, context.window.font, false, id_split[0].c_str());

		draw_pos.y += text_tall;
	}

	const bool active = context.window.blocking == std::hash<std::string_view>()(id);
	
	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, active ? global_colors.control_active_or_clicked : global_colors.control_idle);

	functions.draw_text(draw_pos.x + 4, draw_pos.y + 4, global_colors.color_text, context.window.font, false, active ? "Press any key" : keys_list[value].data());

	push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = std::hash<std::string_view>()(id);
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
}

void zgui::text_input(std::string_view id, std::string& value, const int max_length, const int flags) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 80;
	const int control_height = 20;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		std::wstring text{ id_split[0].begin(), id_split[0].end() };
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, global_colors.color_text, context.window.font, false, id_split[0].c_str());

		draw_pos.y += text_tall;
	}

	const bool active = context.window.blocking == std::hash<std::string_view>()(id);
	const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, active ? global_colors.control_active_or_clicked : global_colors.control_idle);

	functions.draw_text(draw_pos.x + 4, draw_pos.y + 4, global_colors.color_text, context.window.font, false, flags & zgui_text_input_flags_password ? std::string(value.length(), '*').c_str() : value.c_str());

	push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });

	if (hovered && key_pressed(VK_LBUTTON) && !active)
	{
		context.window.blocking = std::hash<std::string_view>()(id);
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
}

void zgui::slider_int(std::string_view id, const int min, const int max, int& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 120;
	const int control_height = 10;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 8, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		std::wstring text{ id_split[0].begin(), id_split[0].end() };
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, global_colors.color_text, context.window.font, false, id_split[0].c_str());

		draw_pos.y += text_tall;
	}

	const int dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, global_colors.control_idle);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, dynamic_width, 10 - 2, global_colors.color_slider);

	int text_wide, text_tall;
	std::string value_str = std::to_string(value);
	std::wstring text{ value_str.begin(), value_str.end() };
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	int text_x = dynamic_width - text_wide;

	if (text_x < 0)
		text_x = 0;

	functions.draw_text(draw_pos.x + text_x, draw_pos.y, global_colors.color_text, context.window.font, false, value_str.c_str());

	functions.draw_text(draw_pos.x - (control_height - 2), draw_pos.y - 2, global_colors.color_text_dimmer, context.window.font, false, "-");
	functions.draw_text(draw_pos.x + (control_width + 4), draw_pos.y - 2, global_colors.color_text_dimmer, context.window.font, false, "+");

	if (context.window.blocking == 0 && mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value - 1, min, max);
	else if (context.window.blocking == 0 && mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value + 1, min, max);

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (key_down(VK_LBUTTON) && context.window.blocking == std::hash<std::string_view>()(id))
	{
		float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
		int value_mapped = static_cast<int>(value_unmapped / control_width * (max - min) + min);

		value = value_mapped;
	}
	else if (!key_down(VK_LBUTTON) && context.window.blocking == std::hash<std::string_view>()(id))
	{
		context.window.blocking = 0;
	}

	push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });

}

void zgui::combobox(std::string_view id, std::vector<std::string>items, int& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 70;
	const int control_height = 20;

	value = std::clamp(value, 0, static_cast<int>(items.size()) - 1);

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		std::wstring text{ id_split[0].begin(), id_split[0].end() };
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, global_colors.color_text, context.window.font, false, id_split[0].c_str());

		draw_pos.y += text_tall;
	}

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, global_colors.control_idle);

	functions.draw_text(draw_pos.x + 4, draw_pos.y + 4, global_colors.color_text, context.window.font, false, items.at(value).c_str());
	functions.draw_text(draw_pos.x + control_width - 10, draw_pos.y + 4, global_colors.color_text, context.window.font, false, "+");

	if (context.window.blocking == std::hash<std::string_view>()(id))
	{
		push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
		push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) + 20 * items.size() });
	}
	else
	{
		push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
		push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });
	}

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (context.window.blocking == std::hash<std::string_view>()(id))
	{
		for (int i = 1; i <= items.size(); i++)
		{
			bool hovered = mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

			if (hovered && key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
				value = i - 1;
			}

			functions.draw_filled_rect(draw_pos.x, draw_pos.y + 19 * i, control_width, control_height, global_colors.control_outline);
			functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + (19 * i) + 1, control_width - 2, control_height - 2, hovered ? global_colors.color_combo_bg : global_colors.control_idle);
			functions.draw_text(draw_pos.x + 3, draw_pos.y + (control_height - 1) * i + 4, global_colors.color_text, context.window.font, false, items.at(i - 1).c_str());
		}
	}
}

void zgui::slider_float(std::string_view id, const float min, const float max, float& value) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const int control_width = 120;
	const int control_height = 10;

	const vec2 cursor_pos = pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 8, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		std::wstring text{ id_split[0].begin(), id_split[0].end() };
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, global_colors.color_text, context.window.font, false, id_split[0].c_str());

		draw_pos.y += text_tall;
	}

	const float dynamic_width = (value - min) / (max - min) * static_cast<float>(control_width) - 2.f;

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, global_colors.control_idle);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, dynamic_width, 10 - 2, global_colors.color_slider);

	int text_wide, text_tall;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << value;
	std::string value_str = ss.str();
	std::wstring text{ value_str.begin(), value_str.end() };
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	int text_x = dynamic_width - text_wide;

	if (text_x < 0)
		text_x = 0;

	functions.draw_text(draw_pos.x + text_x, draw_pos.y, global_colors.color_text, context.window.font, false, value_str.c_str());

	functions.draw_text(draw_pos.x - (control_height - 2), draw_pos.y - 2, global_colors.color_text_dimmer, context.window.font, false, "-");
	functions.draw_text(draw_pos.x + (control_width + 4), draw_pos.y - 2, global_colors.color_text_dimmer, context.window.font, false, "+");

	if (context.window.blocking == 0 && mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value - 1, min, max);
	else if (context.window.blocking == 0 && mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && key_pressed(VK_LBUTTON))
		value = std::clamp(value + 1, min, max);

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (key_down(VK_LBUTTON) && context.window.blocking == std::hash<std::string_view>()(id))
	{
		float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
		float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(control_width)) * (max - min) + min);

		value = value_mapped;
	}
	else if (!key_down(VK_LBUTTON) && context.window.blocking == std::hash<std::string_view>()(id))
	{
		context.window.blocking = 0;
	}

	push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });
}

void zgui::multi_combobox(std::string_view id, std::vector<multi_combobox_item> items) noexcept
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
		std::wstring text{ id_split[0].begin(), id_split[0].end() };
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, global_colors.color_text, context.window.font, false, id_split[0].c_str());

		draw_pos.y += text_tall;
	}

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, global_colors.control_idle);

	functions.draw_text(draw_pos.x + control_width - 10, draw_pos.y + 4, global_colors.color_text, context.window.font, false, "+");

	std::string value_str;
	int text_wide, text_tall;

	for (auto &item_t : items) {
		if (*item_t.value) {
			if (value_str.length() > 0)
				value_str += ", ";

			value_str += item_t.name;
		}
	}

	std::wstring text = std::wstring(value_str.begin(), value_str.end());
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);
	if (text_wide > control_width - 18)
	{
		value_str.resize(control_width / 10);
		value_str += " ...";
	}
	if (!value_str.length())
		value_str += "None";

	functions.draw_text(draw_pos.x + 4, draw_pos.y + 4, global_colors.color_text, context.window.font, false, value_str.c_str());

	if (context.window.blocking == std::hash<std::string_view>()(id))
	{
		push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
		push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) + 20 * items.size() });
	}
	else
	{
		push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
		push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });
	}

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (context.window.blocking == std::hash<std::string_view>()(id))
	{
		for (int i = 1; i <= items.size(); i++)
		{
			bool hovered = mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

			if (hovered && key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
				*items[i - 1].value = !*items[i - 1].value;
			}

			functions.draw_filled_rect(draw_pos.x, draw_pos.y + 19 * i, control_width, control_height, global_colors.control_outline);
			functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + (19 * i) + 1, control_width - 2, control_height - 2, *items[i - 1].value || hovered ? global_colors.control_active_or_clicked : global_colors.control_idle);

			functions.draw_text(draw_pos.x + 3, draw_pos.y + (control_height - 1) * i + 4, global_colors.color_text, context.window.font, false, items[i - 1].name.c_str());
		}
	}
}

bool zgui::clickable_text(std::string_view id) noexcept
{
	std::vector<std::string> id_split = split_str(id, '#');

	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_tall;
	std::wstring text_wide{ id_split[0].begin(), id_split[0].end() };
	functions.get_text_size(context.window.font, text_wide.c_str(), text_width, text_tall);

	const bool active = context.window.blocking == std::hash<std::string_view>()(id);

	functions.draw_text(draw_pos.x, draw_pos.y, global_colors.color_text, context.window.font, false, id_split[0].c_str());

	push_cursor_pos(vec2{ cursor_pos.x + text_width + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_tall / 2 + ITEM_SPACING });

	bool result = false;

	if (const bool hovered = mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_tall); !active && hovered && key_pressed(VK_LBUTTON))
	{
		context.window.blocking = std::hash<std::string_view>()(id);
	}
	else if (active && !key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		result = hovered;
	}

	return result;
}

void zgui::text(std::string_view id) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_tall;
	std::wstring text_wide{ id.begin(), id.end() };
	functions.get_text_size(context.window.font, text_wide.c_str(), text_width, text_tall);

	functions.draw_text(draw_pos.x, draw_pos.y, global_colors.color_text, context.window.font, false, id.data());

	push_cursor_pos(vec2{ cursor_pos.x + text_width + ITEM_SPACING, cursor_pos.y });
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_tall / 2 + ITEM_SPACING });
}

void zgui::dummy() noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + ITEM_SPACING });
}

void zgui::next_column(int pusher_x, int pusher_y) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();
	vec2 new_cursor_pos{ cursor_pos.x + pusher_x, BASE_POS.y + pusher_y };

	if (context.window.next_cursor_pos.y != 0)
		new_cursor_pos.y += 14;

	push_cursor_pos(new_cursor_pos);
}

void zgui::same_line(float x_axis) noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();

	if (x_axis != -1)
		push_cursor_pos(vec2{ BASE_POS.x + x_axis, cursor_pos.x });
}

void zgui::backup_line() noexcept
{
	const vec2 cursor_pos = pop_cursor_pos();

	push_cursor_pos(vec2{ BASE_POS.x, cursor_pos.y });
}