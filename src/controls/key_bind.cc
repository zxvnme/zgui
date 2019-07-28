//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
void zgui::key_bind(const char *id, int &value) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 80;
    const int control_height = 20;

    value = std::clamp(value, 0, 255);

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x + 14, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    const bool active = window_ctx.blocking == utils::hash::hash(id);

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }
    else if (active) {
        for (int i = 0; i < 256; i++) {
            if (utils::input::key_pressed(i)) {
                if (keys_list[i] != "Error")
                    value = i;

                window_ctx.blocking = 0;
            }
            if (utils::input::key_pressed(VK_ESCAPE))
              value = 0;
        }
    }

    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + 4}, zgui_render_type::zgui_text, global_colors.color_text, active ? "Press any key" : keys_list[value].data(), vec2{0, 0}, font});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, active ? global_colors.control_active_or_clicked : global_colors.control_idle, "", {control_width, control_height}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width + 2, control_height + 2}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12)});

    utils::misc::push_font(font);
}
// ========================================================================

