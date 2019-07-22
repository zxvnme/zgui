//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
bool zgui::clickable_text(const char* id) noexcept
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_height;
	functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

	const bool active = context.window.blocking == utils::hash::hash(id);
	const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, text_width, text_height);

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y}, zgui_render_type::zgui_text, (hovered || context.window.blocking == utils::hash::hash(id)) ? global_colors.control_active_or_clicked : global_colors.color_text, id_split[0], vec2{0,0}, font });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing });

	bool result = false;

	if (!active && hovered && utils::input::key_pressed(VK_LBUTTON))
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (active && !utils::input::key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		result = hovered;
	}

	utils::misc::push_font(font);

	return result;
}
// ========================================================================
void zgui::text(const char* text) noexcept
{
	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	int text_width, text_height;
	functions.get_text_size(font, text, text_width, text_height);

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_text, global_colors.color_text, text, vec2{0,0}, font });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + text_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + text_height / 2 + global_config.item_spacing });

	utils::misc::push_font(font);
}
// ========================================================================