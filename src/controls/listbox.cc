//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
void zgui::listbox(const char *id, std::vector<multi_select_item> items) {
    std::vector<std::string> id_split = utils::hash::split_str(id, '#');

    const int control_width = 100;
    const int control_height = 20;

    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    const bool inlined = id_split[0].empty();

    if (!inlined) {
        int text_width, text_height;
        functions.get_text_size(font, id_split[0].c_str(), text_width, text_height);

        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y - 4}, zgui_render_type::zgui_text, global_colors.color_text, id_split[0], vec2{0, 0}, font});

        draw_pos.y += text_height;
    }

    for (int i = 1; i <= items.size(); i++) {
        const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y + (control_height - 1) * (i - 1), control_width, control_height);

        if (hovered && utils::input::key_pressed(VK_LBUTTON)) {
            window_ctx.blocking = 0;
            *items[i - 1].value = !*items[i - 1].value;
        }
        window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 4, draw_pos.y + (control_height - 1) * (i - 1) + 4}, zgui_render_type::zgui_text, *items[i - 1].value || hovered ? global_colors.control_active_or_clicked : global_colors.color_text, items[i - 1].name.data(), vec2{0, 0}, font});
    }


    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x + 1, draw_pos.y + 1}, zgui_render_type::zgui_filled_rect, global_colors.control_idle, "", {control_width - 2, static_cast<float>(control_height * items.size() - 2)}});
    window_ctx.render.emplace_back(zgui_control_render_t{{draw_pos.x, draw_pos.y}, zgui_render_type::zgui_filled_rect, global_colors.control_outline, "", {control_width, static_cast<float>(control_height * items.size())}});

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + control_width + global_config.item_spacing, cursor_pos.y});
    utils::misc::push_cursor_pos(vec2{cursor_pos.x, cursor_pos.y + control_height / 2 + global_config.item_spacing + (inlined ? 0 : 12) + control_height * (items.size() - 1)});

    if (const bool hovered = utils::input::mouse_in_region(draw_pos.x, draw_pos.y, control_width, control_height); hovered && utils::input::key_pressed(VK_LBUTTON) && window_ctx.blocking == 0) {
        window_ctx.blocking = utils::hash::hash(id);
    }

    utils::misc::push_font(font);
}
// ========================================================================