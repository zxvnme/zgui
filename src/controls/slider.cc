//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
void zgui::slider_int(const char* id, const int min, const int max, int& value) noexcept
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int font = utils::misc::pop_font();

	const int control_width = 120;
	const int control_height = 10;

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0,0}, font });

		draw_pos.y += text_tall;
	}

	if (context.window.blocking == 0 && utils::input::mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
		value = std::clamp(value - 1, min, max);
	else if (context.window.blocking == 0 && utils::input::mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
		value = std::clamp(value + 1, min, max);

	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (utils::input::key_down(VK_LBUTTON) && context.window.blocking == utils::hash::hash(id))
	{
		float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
		int value_mapped = static_cast<int>(value_unmapped / control_width * (max - min) + min);

		value = value_mapped;
	}
	else if (!utils::input::key_down(VK_LBUTTON) && context.window.blocking == utils::hash::hash(id))
	{
		context.window.blocking = 0;
	}

	const int dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

	int text_wide, text_tall;
	std::string value_str = std::to_string(value);
	functions.get_text_size(font, value_str.c_str(), text_wide, text_tall);

	int text_x = dynamic_width - text_wide;

	if (text_x < 0)
		text_x = 0;

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x - (control_height - 2), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "-", vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + (control_width + 4), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "+", vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + text_x, draw_pos.y }, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0,0}, font });

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.color_slider, "", { static_cast<float>(dynamic_width), control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle,"", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width, control_height } });


	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });

	utils::misc::push_font(font);
}
// ========================================================================
void zgui::slider_float(const char* id, const float min, const float max, float& value) noexcept
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int control_width = 120;
	const int control_height = 10;

	const int font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	vec2 draw_pos{ context.window.position.x + cursor_pos.x + 14, context.window.position.y + cursor_pos.y };

	const bool inlined = id_split[0].empty();

	if (!inlined)
	{
		int text_wide, text_tall;
		functions.get_text_size(font, id_split[0].c_str(), text_wide, text_tall);

		context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y - 4 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0,0}, font });

		draw_pos.y += text_tall;
	}

	if (context.window.blocking == 0 && utils::input::mouse_in_region(draw_pos.x - (control_height - 2), draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
		value = std::clamp(value - 1, min, max);
	else if (context.window.blocking == 0 && utils::input::mouse_in_region(draw_pos.x + control_width, draw_pos.y, 8, 10) && utils::input::key_pressed(VK_LBUTTON))
		value = std::clamp(value + 1, min, max);

	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (utils::input::key_down(VK_LBUTTON) && context.window.blocking == utils::hash::hash(id))
	{
		float value_unmapped = std::clamp(mouse_pos.x - draw_pos.x, 0.0f, static_cast<float>(control_width));
		float value_mapped = static_cast<float>((value_unmapped / static_cast<float>(control_width)) * (max - min) + min);

		value = value_mapped;
	}
	else if (!utils::input::key_down(VK_LBUTTON) && context.window.blocking == utils::hash::hash(id))
	{
		context.window.blocking = 0;
	}

	const float dynamic_width = (static_cast<float>(value) - min) / (max - min) * control_width - 2;

	int text_wide, text_tall;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << value;
	std::string value_str = ss.str();
	functions.get_text_size(font, value_str.c_str(), text_wide, text_tall);

	int text_x = dynamic_width - text_wide;

	if (text_x < 0)
		text_x = 0;

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x - (control_height - 2), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "-", vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + (control_width + 4), draw_pos.y - 2 }, zgui_render_type::zgui_text, global_colors.color_text_dimmer, "+", vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + text_x, draw_pos.y }, zgui_render_type::zgui_text, global_colors.color_text, value_str, vec2{0,0}, font });

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect, global_colors.color_slider, "", { dynamic_width,  control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ {draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle,"", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width, control_height } });


	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + 14 + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });

	utils::misc::push_font(font);
}
// ========================================================================