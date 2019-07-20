//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
bool zgui::begin_window(std::string_view title, const vec2 default_size, const unsigned long font, const int flags)
{
	if (!input_loop_started)
		throw std::exception("Input loop didnt start or didnt start properly.");;

	if (!(flags & zgui_window_flags_always_open))
	{
		if (utils::input::key_pressed(global_config.menu_toggle_key))
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
			if ((flags & zgui_window_flags_no_border ? utils::input::mouse_in_region(context.window.position.x + 9, context.window.position.y + 14, context.window.size.x - 18, 14)
				: utils::input::mouse_in_region(context.window.position.x - 6, context.window.position.y - 10, context.window.size.x + 12, 16))
				&& utils::input::key_pressed(VK_LBUTTON) && !context.window.dragging)
			{
				context.window.dragging = true;
			}
			else if (utils::input::key_down(VK_LBUTTON) && context.window.dragging)
			{
				const vec2 mouse_delta{ mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y };
				const vec2 new_position{ context.window.position.x + mouse_delta.x, context.window.position.y + mouse_delta.y };

				context.window.position = new_position;
			}
			else if (!utils::input::key_down(VK_LBUTTON) && context.window.dragging)
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
			functions.draw_text(context.window.position.x + context.window.size.x * 0.5, context.window.position.y + (context.window.size.y * 0.010) - 10, global_colors.color_text, font, true, title.data());

		functions.draw_filled_rect(context.window.position.x + 9, context.window.position.y + 16, context.window.size.x - 18, context.window.size.y - 25, global_colors.window_background);


		utils::misc::push_font(font);
		utils::misc::push_cursor_pos(global_config.base_pos);
	}

	return context.window.opened || context.window.alpha > 0;
}
// ========================================================================
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
			functions.draw_text(context.window.render[i].draw_position.x, context.window.render[i].draw_position.y, context.window.render[i].color, context.window.render[i].font, false, context.window.render[i].text.c_str());
			break;
		}
	}

	context.window.render.clear();

	while (!context.window.cursor_pos.empty())
		context.window.cursor_pos.pop();

}
// ========================================================================