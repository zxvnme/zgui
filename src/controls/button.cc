//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
bool zgui::button(const char* id, const vec2 size) noexcept
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	const bool active = context.window.blocking == utils::hash::hash(id);

	bool result = false;
	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON))
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (active && !utils::input::key_down(VK_LBUTTON))
	{
		context.window.blocking = 0;
		result = hovered;
	}

	int text_wide, text_tall;
	functions.get_text_size(font, id_split[0].c_str(), text_wide, text_tall);

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + size.x / 2 - text_wide / 2, draw_pos.y + size.y / 2 - text_tall / 2 }, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {size.x - 2, size.y - 2} });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", size });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing });

	utils::misc::push_font(font);

	return result;
}
// ========================================================================

