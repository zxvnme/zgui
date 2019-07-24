//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
void zgui::checkbox(const char *id, bool &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_height = 8;
    const int control_width = 8;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;
    functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width + 6 + text_width, control_height); !active && hovered && utils::input::key_pressed(VK_LBUTTON)) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active && !utils::input::key_down(VK_LBUTTON)) {
        window_ctx.blocking = 0;
        value = !value;
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 14, draw_pos.y - 2}, zgui_render_type::zgui_text, value ? global_colors.color_text : global_colors.color_text_dimmer, id_split[0], vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, value ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width - 2, control_height - 2}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, control_height}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + 14 + text_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + global_config.item_spacing});

    utils::misc::push_font(font);
}
// ========================================================================