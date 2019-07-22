//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
void zgui::text_input(const char* id, std::string& value, const int max_length, const int flags)
{
	std::vector<std::string> id_split = utils::hash::split_str(id, '#');

	const int control_width = 80;
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

	const bool active = context.window.blocking == utils::hash::hash(id);
	const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height);

	if (hovered && utils::input::key_pressed(VK_LBUTTON) && !active)
	{
		context.window.blocking = utils::hash::hash(id);
	}
	else if (active)
	{
		if (utils::input::key_pressed(VK_ESCAPE) || utils::input::key_pressed(VK_RETURN) || (!hovered && utils::input::key_pressed(VK_LBUTTON)))
		{
			context.window.blocking = 0;
		}
		else if (utils::input::key_pressed(VK_BACK) && !value.empty())
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
					if (utils::input::key_pressed(i))
						value += utils::input::key_down(VK_SHIFT) ? static_cast<char>(i) : static_cast<char>(i + 32);
				}
				else
				{
					if (utils::input::key_pressed(i))
					{
						for (int j = 0; j < sizeof(special_characters); j++)
						{
							if (special_characters[j].vk == i)
								value += utils::input::key_down(VK_SHIFT) ? special_characters[j].shift : special_characters[j].regular;
						}
					}
				}
			}
		}
	}

	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 4, draw_pos.y + 4 }, zgui_render_type::zgui_text,  global_colors.color_text, flags & zgui_text_input_flags_password ? std::string(value.length(), '*').c_str() : value.c_str(), vec2{0,0}, font });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x + 1, draw_pos.y + 1 }, zgui_render_type::zgui_filled_rect,  active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", { control_width - 2, control_height - 2 } });
	context.window.render.emplace_back(zgui_control_render_t{ { draw_pos.x, draw_pos.y }, zgui_render_type::zgui_filled_rect, global_colors.control_outline,"", { control_width, control_height } });

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y });
	utils::misc::push_cursor_pos(vec2{ cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) });

	utils::misc::push_font(font);
}
// ========================================================================