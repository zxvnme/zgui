//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
void zgui::begin_groupbox(std::string_view title, const vec2 size, const int flags)
{
	const unsigned long font = utils::misc::pop_font();

	const vec2 cursor_pos = utils::misc::pop_cursor_pos();
	const vec2 draw_pos{ context.window.position.x + cursor_pos.x, context.window.position.y + cursor_pos.y };

	functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
	functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

	if (title.length() > 0)
	{
		if (!(flags & zgui_groupbox_flags_title_centered))
		{
			functions.draw_text(draw_pos.x + 4, draw_pos.y - 8, global_colors.color_text, font, false, title.data());
		}
		else
		{
			int text_width, text_height;
			functions.get_text_size(font, title.data(), text_width, text_height);
			functions.draw_text(draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y - 8, global_colors.color_text, font, false, title.data());
		}
	}

	context.window.next_cursor_pos = vec2{ cursor_pos.x, cursor_pos.y + size.y + 10 };

	utils::misc::push_cursor_pos(vec2{ cursor_pos.x + 8, cursor_pos.y + 14 });

	utils::misc::push_font(font);
}
// ========================================================================
void zgui::end_groupbox()
{
	utils::misc::push_cursor_pos(context.window.next_cursor_pos);
	context.window.next_cursor_pos = { };
}
// ========================================================================