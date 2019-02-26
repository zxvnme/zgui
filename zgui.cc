#include "zgui.hh"

// zgui by zxvnme (https://github.com/zxvnme)
// heres defines that are designed to be modified by your preferences.
// see zgui.hh for complete documentation.
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// base position that first control will be created.
#define BASE_POS vec2 {16, 23}
// spacing between controls.
#define ITEM_SPACING 16
// name of window where input will be read from
#define INPUT_WINDOW ""
// virtual key that will trigger our gui open.
#define MENU_TOGGLE_KEY VK_INSERT
//
#define ZERO_VEC vec2 {0, 0}


zgui* g_zgui = new zgui();

void zgui::set_functions(zgui::functions_t& functions)
{
	this->functions = functions;
}

zgui::functions_t& zgui::get_functions()
{
	return this->functions;
}

bool zgui::mouse_in_region(int x, int y, int w, int h)
{
	if (this->mouse_pos.x > x && this->mouse_pos.y > y && this->mouse_pos.x < w + x && this->mouse_pos.y < h + y)
		return true;
	return false;
}

void zgui::push_cursor_pos(zgui::vec2 pos)
{
	context.window.cursor_pos.push(pos);
}

zgui::vec2 zgui::pop_cursor_pos()
{
	vec2 pos = context.window.cursor_pos.top();
	context.window.cursor_pos.pop();
	return pos;
}

void zgui::poll_input()
{
	if (strlen(INPUT_WINDOW) == 0)
		throw "No window from where input should be read from specified (see defines on the top of zgui.cc file). Comment this if you are aware of this and its not an error.";

	for (int i = 0; i < 256; i++) {
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
	}

	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	ScreenToClient(FindWindow(0, INPUT_WINDOW), &mouse_pos);
	this->previous_mouse_pos = this->mouse_pos;
	this->mouse_pos = vec2{ static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y) };
}

bool zgui::key_pressed(int key)
{
	return key_state[key] == true && prev_key_state[key] == false;
}

bool zgui::key_down(int key)
{
	return key_state[key] == true;
}

bool zgui::key_released(int key)
{
	return key_state[key] == false && prev_key_state[key] == true;
}

bool zgui::begin_window(std::string title, zgui::vec2 default_size, unsigned long font)
{
	context.window.font = font;

	if (this->key_pressed(MENU_TOGGLE_KEY))
		context.window.opened = !context.window.opened;

	float frametime;
	functions.get_frametime(frametime);
	int fade_factor = ((1.0f / (150.0f / 1000.0f)) * frametime) * 255;

	int prev_alpha = context.window.alpha;

	if (context.window.opened && context.window.alpha < 255)
		context.window.alpha = std::clamp(context.window.alpha + fade_factor, 0, 255);
	else if (!context.window.opened && context.window.alpha > 0)
		context.window.alpha = std::clamp(context.window.alpha - fade_factor, 0, 255);

	if (context.window.alpha != prev_alpha)
	{
		this->global_colors.window_border_inner_fill = color{ 60, 60, 60, context.window.alpha };
		this->global_colors.window_border_fill = color{ 40, 40, 40, context.window.alpha };
		this->global_colors.window_border_color = color{ 10, 10, 10, context.window.alpha };
		this->global_colors.window_background = color{ 40, 40, 40, context.window.alpha };

		this->global_colors.control_outline = color{ 23, 23, 30, context.window.alpha };
		this->global_colors.control_active_or_clicked = color{ 108, 92, 231, context.window.alpha };
		this->global_colors.control_idle = color{ 62, 62, 72, context.window.alpha };

		this->global_colors.color_groupbox_bg = color{ 50, 50, 50, context.window.alpha };
		this->global_colors.color_text = color{ 203, 203, 203, context.window.alpha };
		this->global_colors.color_text_dark = color{ 99, 110, 114, context.window.alpha };
		this->global_colors.color_slider = color{ 108, 92, 231, context.window.alpha };
	}

	this->poll_input();

	if (context.window.opened || context.window.alpha > 0)
	{
		if (this->mouse_in_region(context.window.position.x - 6, context.window.position.y - 10, context.window.size.x + 12, 16) && this->key_pressed(VK_LBUTTON) && !context.window.dragging)
		{
			context.window.dragging = true;
		}
		else if (this->key_down(VK_LBUTTON) && context.window.dragging)
		{
			vec2 mouse_delta = vec2{ mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y };
			vec2 new_position = vec2{ context.window.position.x + mouse_delta.x, context.window.position.y + mouse_delta.y };

			context.window.position = new_position;
		}
		else if (!this->key_down(VK_LBUTTON) && context.window.dragging)
		{
			context.window.dragging = false;
		}

		if (context.window.size.x < 1 && context.window.size.y < 1)
			context.window.size = default_size;

		functions.draw_filled_rect(context.window.position.x - 6, context.window.position.y - 10, context.window.size.x + 12, context.window.size.y + 16, this->global_colors.window_border_inner_fill);
		functions.draw_filled_rect(context.window.position.x - 5, context.window.position.y - 9, context.window.size.x + 10, context.window.size.y + 14, this->global_colors.window_border_color);
		functions.draw_filled_rect(context.window.position.x - 4, context.window.position.y - 8, context.window.size.x + 8, context.window.size.y + 12, this->global_colors.window_border_fill);
		functions.draw_filled_rect(context.window.position.x, context.window.position.y + 7, context.window.size.x, context.window.size.y - 7, this->global_colors.window_border_color);
		functions.draw_filled_rect(context.window.position.x + 1, context.window.position.y + 8, context.window.size.x - 2, context.window.size.y - 9, this->global_colors.window_border_inner_fill);
		functions.draw_filled_rect(context.window.position.x + 8, context.window.position.y + 15, context.window.size.x - 16, context.window.size.y - 23, this->global_colors.window_border_color);

		functions.draw_filled_rect(context.window.position.x + 9, context.window.position.y + 16, context.window.size.x - 18, context.window.size.y - 25, this->global_colors.window_background);

		functions.draw_text(context.window.position.x + context.window.size.x * 0.5, context.window.position.y + (context.window.size.y * 0.010) - 10, this->global_colors.color_text, context.window.font, true, title.c_str());

		this->push_cursor_pos(BASE_POS);
	}

	return context.window.opened || context.window.alpha > 0;
}

void zgui::end_window()
{
	while (context.window.cursor_pos.empty() == false)
	{
		context.window.cursor_pos.pop();
	}
}

void zgui::begin_groupbox(std::string name, vec2 size)
{
	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, this->global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, this->global_colors.color_groupbox_bg);

	if (name.length() > 0)
		functions.draw_text(draw_pos.x + 4, draw_pos.y - 6, this->global_colors.color_text, context.window.font, false, name.c_str());

	context.window.next_cursor_pos = vec2{ cursor_pos.x, cursor_pos.y + size.y + 10 };

	this->push_cursor_pos(vec2{ cursor_pos.x + 8, cursor_pos.y + 14 });
}

void zgui::end_groupbox()
{
	this->push_cursor_pos(context.window.next_cursor_pos);
	context.window.next_cursor_pos = ZERO_VEC;
}

void zgui::checkbox(std::string name, bool* value)
{
	int control_height = 8;
	int control_width = 8;

	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, this->global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, *value ? this->global_colors.control_active_or_clicked : this->global_colors.control_idle);

	int text_wide, text_tall;
	std::wstring text = std::wstring(name.begin(), name.end());
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	functions.draw_text(draw_pos.x + 14, draw_pos.y - 2, this->global_colors.color_text, context.window.font, false, name.c_str());

	if (this->mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height) && this->key_pressed(VK_LBUTTON) && context.window.blocking == 0)
		*value = !*value;

	this->push_cursor_pos(vec2{ cursor_pos.x + 14 + text_wide + ITEM_SPACING, cursor_pos.y });
	this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + ITEM_SPACING });
}

void zgui::toggle_button(std::string name, vec2 size, bool* value)
{
	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, this->global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, size.x - 2, size.y - 2, *value ? this->global_colors.control_active_or_clicked : this->global_colors.control_idle);

	int text_wide, text_tall;
	std::wstring text = std::wstring(name.begin(), name.end());
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	functions.draw_text(draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y + size.y / 2 - text_tall / 2, this->global_colors.color_text, context.window.font, false, name.c_str());

	this->push_cursor_pos(vec2{ cursor_pos.x + size.x + ITEM_SPACING, cursor_pos.y });
	this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + ITEM_SPACING });

	if (this->mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y) && this->key_pressed(VK_LBUTTON) && context.window.blocking == 0)
		*value = !*value;
}

bool zgui::button(std::string name, vec2 size)
{
	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	bool active = context.window.blocking == std::hash<std::string>()(name);
	bool hovered = this->mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y);

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, this->global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, size.x - 2, size.y - 2, active ? this->global_colors.control_active_or_clicked : this->global_colors.control_idle);

	int text_wide, text_tall;
	std::wstring text = std::wstring(name.begin(), name.end());
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	functions.draw_text(draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y + size.y / 2 - text_tall / 2, this->global_colors.color_text, context.window.font, false, name.c_str());

	this->push_cursor_pos(vec2{ cursor_pos.x + size.x + ITEM_SPACING, cursor_pos.y });
	this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + ITEM_SPACING });

	bool result = false;

	if (active == false && hovered == true && this->key_pressed(VK_LBUTTON))
	{
		context.window.blocking = std::hash<std::string>()(name);
	}
	else if (active == true && this->key_down(VK_LBUTTON) == false)
	{
		context.window.blocking = 0;
		result = hovered;
	}

	return result;
}

void zgui::slider_int(std::string name, int min, int max, int* value)
{
	int control_width = 120;
	int control_height = 10;

	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x + 8, context.window.position.y + cursor_pos.y };

	bool inlined = name.empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		std::wstring text = std::wstring(name.begin(), name.end());
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, this->global_colors.color_text, context.window.font, false, name.c_str());

		draw_pos.y += text_tall;
	}

	bool hovered = this->mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);
	int dynamic_width = ((float)*value - (float)min) / ((float)max - (float)min) * 118.0f;

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, this->global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, this->global_colors.control_idle);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, dynamic_width, 10 - 2, this->global_colors.color_slider);

	int text_wide, text_tall;
	std::string value_str = std::to_string(*value);
	std::wstring text = std::wstring(value_str.begin(), value_str.end());
	functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

	int text_x = dynamic_width - text_wide;

	if (text_x < 0)
		text_x = 0;

	functions.draw_text(draw_pos.x + text_x, draw_pos.y, this->global_colors.color_text, context.window.font, false, value_str.c_str());

	functions.draw_text(draw_pos.x - (control_height - 2), draw_pos.y - 2, this->global_colors.color_text_dark, context.window.font, false, "-");
	functions.draw_text(draw_pos.x + (control_width + 4), draw_pos.y - 2, this->global_colors.color_text_dark, context.window.font, false, "+");
	
	if(context.window.blocking == 0 && this->mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && this->key_pressed(VK_LBUTTON))
		*value = std::clamp(*value - 1, min, max);
	else if (context.window.blocking == 0 && this->mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && this->key_pressed(VK_LBUTTON))
		*value = std::clamp(*value + 1, min, max);

	if (hovered && this->key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = std::hash<std::string>()(name);
	}
	else if (this->key_down(VK_LBUTTON) && context.window.blocking == std::hash<std::string>()(name))
	{
		float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, 120.f);
		int value_mapped = static_cast<int>((value_unmapped / 120.f) * ((float)max - (float)min) + (float)min);

		*value = value_mapped;
	}
	else if (!this->key_down(VK_LBUTTON) && context.window.blocking == std::hash<std::string>()(name))
	{
		context.window.blocking = 0;
	}

	this->push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
	this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });
}

void zgui::combobox(std::string name, std::vector<std::string>items, int* value)
{
	int control_width = 70;
	int control_height = 20;

	*value = std::clamp(*value, 0, (int)items.size() - 1);

	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	bool inlined = name.empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		std::wstring text = std::wstring(name.begin(), name.end());
		functions.get_text_size(context.window.font, text.c_str(), text_wide, text_tall);

		functions.draw_text(draw_pos.x, draw_pos.y - 4, this->global_colors.color_text, context.window.font, false, name.c_str());

		draw_pos.y += text_tall;
	}

	bool hovered = this->mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);

	functions.draw_filled_rect(draw_pos.x, draw_pos.y, control_width, control_height, this->global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + 1, control_width - 2, control_height - 2, this->global_colors.control_idle);

	functions.draw_text(draw_pos.x + 4, draw_pos.y + 4, this->global_colors.color_text, context.window.font, false, items.at(*value).c_str());
	functions.draw_text(draw_pos.x + 60, draw_pos.y + 4, this->global_colors.color_text, context.window.font, false, "+");
	
	if (context.window.blocking == std::hash<std::string>()(name))
	{
		this->push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
		this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) + 20 * items.size() });
	}
	else
	{
		this->push_cursor_pos(vec2{ cursor_pos.x + control_width + ITEM_SPACING, cursor_pos.y });
		this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + ITEM_SPACING + (inlined ? 0 : 12) });
	}

	if (hovered && this->key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = std::hash<std::string>()(name);
	}
	else if (context.window.blocking == std::hash<std::string>()(name))
	{
		for (int i = 1; i <= items.size(); i++)
		{
			bool hovered = this->mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

			if (hovered && this->key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
				*value = i - 1;
			}

			functions.draw_filled_rect(draw_pos.x, draw_pos.y + 19 * i, control_width, control_height, this->global_colors.control_outline);
			functions.draw_filled_rect(draw_pos.x + 1, draw_pos.y + (19 * i) + 1, control_width - 2, control_height - 2, hovered ? this->global_colors.color_combo_bg : this->global_colors.control_idle);
			functions.draw_text(draw_pos.x + 3, draw_pos.y + (control_height - 1) * i + 4, this->global_colors.color_text, context.window.font, false, items.at(i - 1).c_str());
		}
	}
}

bool zgui::clickable_text(std::string text)
{
	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_tall;
	std::wstring text_wide = std::wstring(text.begin(), text.end());
	functions.get_text_size(context.window.font, text_wide.c_str(), text_width, text_tall);

	bool active = context.window.blocking == std::hash<std::string>()(text);
	bool hovered = this->mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_tall);

	functions.draw_text(draw_pos.x, draw_pos.y, this->global_colors.color_text, context.window.font, false, text.c_str());

	this->push_cursor_pos(vec2{ cursor_pos.x + text_width + ITEM_SPACING, cursor_pos.y });
	this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_tall / 2 + ITEM_SPACING });

	bool result = false;

	if (active == false && hovered == true && this->key_pressed(VK_LBUTTON))
	{
		context.window.blocking = std::hash<std::string>()(text);
	}
	else if (active == true && this->key_down(VK_LBUTTON) == false)
	{
		context.window.blocking = 0;
		result = hovered;
	}

	return result;
}

void zgui::text(std::string text)
{
	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 draw_pos = vec2{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_tall;
	std::wstring text_wide = std::wstring(text.begin(), text.end());
	functions.get_text_size(context.window.font, text_wide.c_str(), text_width, text_tall);

	bool active = context.window.blocking == std::hash<std::string>()(text);
	bool hovered = this->mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_tall);

	functions.draw_text(draw_pos.x, draw_pos.y, this->global_colors.color_text, context.window.font, false, text.c_str());

	this->push_cursor_pos(vec2{ cursor_pos.x + text_width + ITEM_SPACING, cursor_pos.y });
	this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_tall / 2 + ITEM_SPACING });
}

void zgui::dummy()
{
	vec2 cursor_pos = this->pop_cursor_pos();
	this->push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + ITEM_SPACING });
}

void zgui::next_column(int pusher_x, int pusher_y)
{
	vec2 cursor_pos = this->pop_cursor_pos();
	vec2 new_cursor_pos = { cursor_pos.x + pusher_x, BASE_POS.y + pusher_y};

	if (context.window.next_cursor_pos.y != 0)
		new_cursor_pos.y += 14;

	this->push_cursor_pos(new_cursor_pos);
}

void zgui::same_line(float x_axis)
{
	vec2 cursor_pos = this->pop_cursor_pos();

	if (x_axis != -1)
		this->push_cursor_pos(vec2{ BASE_POS.x + x_axis, cursor_pos.x });
}