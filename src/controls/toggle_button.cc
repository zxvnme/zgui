//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
void zgui::toggle_button(const char *id, const vec2 size, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, size.x, size.y); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + size.y / 2 - text_height / 2}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", size});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {size.x + 2, size.y + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + size.x + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + size.y / 2 + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================