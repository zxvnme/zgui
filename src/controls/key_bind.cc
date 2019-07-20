//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
void zgui::key_bind(const char* id, int& value) noexcept
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int control_width = 80;
	const int control_height = 20;

	value = std::clamp(value, 0, 255);

	const int font = utils::misc::pop_font();

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

	const bool active = context.window.blocking == utils::hash::hash(id);

	if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && context.window.blocking == 0)
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (active)
	{
		for (int i = 0; i < 256; i++)
		{
			if (utils::input::key_pressed(i))
			{
				if (keys_list[i] != "Error")
					value = i;

				context.window.blocking = 0;
			}
		}
	}

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text, global_colors.color_text, active ? "Press any key" : keys_list[value].data(), vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", { control_width, control_height } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width + 2, control_height + 2 } });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });

	utils::misc::push_font(font);
}
// ========================================================================

