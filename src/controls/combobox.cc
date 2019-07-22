//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
void zgui::combobox(const char* id, std::vector<std::string>items, int& value) noexcept
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int control_width = 70;
	const int control_height = 20;

	value = std::clamp(value, 0, static_cast<int>(items.size()) - 1);

	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_width, text_height;

		functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0,0}, font });

		draw_pos.y += text_height;
	}

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + control_width - 10, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  "+", vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  items.at(value), vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x,  draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width,  control_height } });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });


	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (context.window.blocking == utils::hash::hash(id))
	{
		for (int i = 1; i <= items.size(); i++)
		{
			bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);

			if (hovered && utils::input::key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
				value = i - 1;
			}

			if (!hovered && utils::input::key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
			}
			bool selected = value == i - 1;
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4 }, zgui_render_type::zgui_text, selected ? global_colors.control_active_or_clicked : global_colors.color_text, items.at(i - 1), vec2{0,0}, font });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + (19 * i) + 1 }, zgui_render_type::zgui_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y + 19 * i }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width, control_height } });
		}
	}

	utils::misc::push_font(font);
}
// ========================================================================
void zgui::multi_combobox(const char* id, std::vector<multi_select_item> items) noexcept
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int control_width = 100;
	const int control_height = 20;

	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_width, text_height;
		functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0,0}, font });

		draw_pos.y += text_height;
	}

	std::string value_str;
	int text_width, text_height;

	for (auto &item_t : items) {
		if (*item_t.value) {
			if (value_str.length() > 0)
				value_str += ", ";

			value_str += item_t.name;
		}
	}

	functions.get_text_size(font, value_str.c_str(), text_width, text_height);
	if (text_width > control_width - 18)
	{
		value_str.resize(control_width / 10);
		value_str += " ...";
	}
	if (!value_str.length())
		value_str += "None";

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + control_width - 10, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  "+", vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text,  value_str, vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x,  draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width,  control_height } });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });


	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (context.window.blocking == utils::hash::hash(id))
	{
		for (int i = 1; i <= items.size(); i++)
		{
			bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * i, control_width, control_height);
			const bool outofbounds = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1), control_width, control_height * i);
			if (hovered && utils::input::key_pressed(VK_LBUTTON))
			{
				context.window.blocking = utils::hash::hash(id);
				*items[i - 1].value = !*items[i - 1].value;
			}
			if (!outofbounds && utils::input::key_pressed(VK_LBUTTON))
			{
				context.window.blocking = 0;
			}
			bool selected = *items[i - 1].value >= 1;
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + (control_height - 1) * i + 4 }, zgui_render_type::zgui_text, selected ? global_colors.control_active_or_clicked : global_colors.color_text, items[i - 1].name.data(), vec2{0,0}, font });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + (19 * i) + 1 }, zgui_render_type::zgui_filled_rect, hovered ? global_colors.color_combo_bg : global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
			context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y + 19 * i }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", { control_width, control_height } });
		}
	}

	utils::misc::push_font(font);
}
// ========================================================================
