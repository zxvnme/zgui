//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
// Push cursor position to the stack defined in window context.
void zgui::utils::misc::push_cursor_pos(const vec2 pos)
{
	context.window.cursor_pos.push(pos);
}
// ========================================================================
// Pop cursor position from the stack defined in window context.
zgui::vec2 zgui::utils::misc::pop_cursor_pos()
{
	const vec2 pos = context.window.cursor_pos.top();
	context.window.cursor_pos.pop();
	return pos;
}
// ========================================================================
// Pop font from the stack defined in window context.
void zgui::utils::misc::push_font(const unsigned long font)
{
	context.window.fonts.push(font);
}
// ========================================================================
// Push font to the stack defined in window context.
unsigned long zgui::utils::misc::pop_font()
{
	const unsigned long font = context.window.fonts.top();
	context.window.fonts.pop();
	return font;
}
// ========================================================================