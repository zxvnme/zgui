//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
bool zgui::begin_window(std::string_view title, const vec2 default_size, const unsigned long font, const int flags) {
    if (!input_loop_started)
        throw std::exception("Input loop didnt start or didnt start properly.");;

    if (!(flags & zgui_window_flags_always_open)) {
        if (utils::input::key_pressed(global_config.menu_toggle_key))
            window_ctx.opened = !window_ctx.opened;
    }
    else
        window_ctx.opened = true;

    if (const int prev_alpha = window_ctx.alpha; !(flags & zgui_window_flags_no_ontoggle_animation)) {
        const int fade_factor = static_cast<int>(1.0f / 0.15f * functions.get_frametime() * 255);
        window_ctx.alpha = std::clamp(window_ctx.alpha + (window_ctx.opened ? fade_factor : -fade_factor), 0, 255);

        if (window_ctx.alpha != prev_alpha) {
            global_colors.window_border_inner_fill.a = window_ctx.alpha;
            global_colors.window_border_fill.a = window_ctx.alpha;
            global_colors.window_border_color.a = window_ctx.alpha;
            global_colors.window_background.a = window_ctx.alpha;

            global_colors.control_outline.a = window_ctx.alpha;
            global_colors.control_active_or_clicked.a = window_ctx.alpha;
            global_colors.control_idle.a = window_ctx.alpha;

            global_colors.color_groupbox_bg.a = window_ctx.alpha;
            global_colors.color_text.a = window_ctx.alpha;
            global_colors.color_text_dimmer.a = window_ctx.alpha;
            global_colors.color_slider.a = window_ctx.alpha;
        }
    }

    if (window_ctx.opened || window_ctx.alpha > 0) {
        if (!(flags & zgui_window_flags_no_move)) {
            if ((flags & zgui_window_flags_no_border ? utils::input::mouse_in_region(window_ctx.position.x + 9, window_ctx.position.y + 14, window_ctx.size.x - 18, 14)
                                                     : utils::input::mouse_in_region(window_ctx.position.x - 6, window_ctx.position.y - 10, window_ctx.size.x + 12, 16))
                && utils::input::key_pressed(VK_LBUTTON) && !window_ctx.dragging) {
                window_ctx.dragging = true;
            }
            else if (utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
                const vec2 mouse_delta{mouse_pos.x - previous_mouse_pos.x, mouse_pos.y - previous_mouse_pos.y};
                const vec2 new_position{window_ctx.position.x + mouse_delta.x, window_ctx.position.y + mouse_delta.y};

                window_ctx.position = new_position;
            }
            else if (!utils::input::key_down(VK_LBUTTON) && window_ctx.dragging) {
                window_ctx.dragging = false;
            }
        }

        if (window_ctx.size.x < 1 && window_ctx.size.y < 1)
            window_ctx.size = default_size;

        if (!(flags & zgui_window_flags_no_border)) {
            functions.draw_filled_rect(window_ctx.position.x - 6, window_ctx.position.y - 10, window_ctx.size.x + 12, window_ctx.size.y + 16, global_colors.window_border_inner_fill);
            functions.draw_filled_rect(window_ctx.position.x - 5, window_ctx.position.y - 9, window_ctx.size.x + 10, window_ctx.size.y + 14, global_colors.window_border_color);
            functions.draw_filled_rect(window_ctx.position.x - 4, window_ctx.position.y - 8, window_ctx.size.x + 8, window_ctx.size.y + 12, global_colors.window_border_fill);
            functions.draw_filled_rect(window_ctx.position.x, window_ctx.position.y + 7, window_ctx.size.x, window_ctx.size.y - 7, global_colors.window_border_color);
            functions.draw_filled_rect(window_ctx.position.x + 1, window_ctx.position.y + 8, window_ctx.size.x - 2, window_ctx.size.y - 9, global_colors.window_border_inner_fill);
            functions.draw_filled_rect(window_ctx.position.x + 8, window_ctx.position.y + 15, window_ctx.size.x - 16, window_ctx.size.y - 23, global_colors.window_border_color);
        }

        if (!(flags & zgui_window_flags_no_titlebar))
            functions.draw_text(window_ctx.position.x + window_ctx.size.x * 0.5, window_ctx.position.y + (window_ctx.size.y * 0.010) - 10, global_colors.color_text, font, true, title.data());

        functions.draw_filled_rect(window_ctx.position.x + 9, window_ctx.position.y + 16, window_ctx.size.x - 18, window_ctx.size.y - 25, global_colors.window_background);


        utils::misc::push_font(font);
        utils::misc::push_cursor_pos(global_config.base_pos);
    }

    return window_ctx.opened || window_ctx.alpha > 0;
}

// ========================================================================
void zgui::end_window() {
    for (int i = window_ctx.render.size() - 1; i >= 0; i--) {
        switch (window_ctx.render[i].render_type) {
            case zgui_render_type::zgui_line:
                functions.draw_line(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_rect:
                functions.draw_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_filled_rect:
                functions.draw_filled_rect(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].size.x, window_ctx.render[i].size.y, window_ctx.render[i].color);
                break;
            case zgui_render_type::zgui_text:
                functions.draw_text(window_ctx.render[i].draw_position.x, window_ctx.render[i].draw_position.y, window_ctx.render[i].color, window_ctx.render[i].font, false, window_ctx.render[i].text.c_str());
                break;
        }
    }

    window_ctx.render.clear();

    while (!window_ctx.cursor_pos.empty())
        window_ctx.cursor_pos.pop();
}
// ========================================================================