//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
void zgui::dummy() {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + global_config.item_spacing});
}
// ========================================================================
void zgui::next_column(const int pusher_x, const int pusher_y) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 new_cursor_pos{cursor_pos.x + pusher_x, global_config.base_pos.y + pusher_y};

    if (window_ctx.next_cursor_pos.y != 0)
        new_cursor_pos.y += 14;

    utils::misc::push_cursor_pos(new_cursor_pos);
}
// ========================================================================
void zgui::same_line(const float x_axis) {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();

    if (x_axis != -1)
        utils::misc::push_cursor_pos(vec2{global_config.base_pos.x + x_axis, cursor_pos.x});
}
// ========================================================================
void zgui::backup_line() {
    const vec2 cursor_pos = utils::misc::pop_cursor_pos();

    utils::misc::push_cursor_pos(vec2{window_ctx.next_cursor_pos.x, cursor_pos.y});
}
// ========================================================================