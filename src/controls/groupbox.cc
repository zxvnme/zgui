//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
void zgui::begin_groupbox(std::string_view title, const vec2 size, const int flags) {
    const unsigned long font = utils::misc::pop_font();

    const vec2 cursor_pos = utils::misc::pop_cursor_pos();
    const vec2 draw_pos{window_ctx.position.x + cursor_pos.x, window_ctx.position.y + cursor_pos.y};

    int text_width, text_height;

    if (!title.empty())
        functions.get_text_size(font, title.data(), text_width, text_height);

    const int header_height = title.empty() ? 16 : text_height + 3;

    if (flags & zgui_groupbox_flags_legacy_design) {
        functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

        if (!title.empty()) {
            functions.draw_text(draw_pos.x + 4, draw_pos.y - 8, global_colors.color_text, font, false, title.data());
        }
    }
    else {
        functions.draw_rect(draw_pos.x - 1, draw_pos.y - 1, size.x + 2, size.y + 2, global_colors.control_outline);
        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, size.y, global_colors.color_groupbox_bg);

        functions.draw_filled_rect(draw_pos.x, draw_pos.y, size.x, header_height, global_colors.color_groupbox_header);
        functions.draw_line(draw_pos.x, draw_pos.y + header_height, draw_pos.x + size.x, draw_pos.y + header_height, global_colors.control_outline);

        if (!title.empty()) {
            functions.draw_text(draw_pos.x + size.x / 2 - text_width / 2, draw_pos.y + 2, global_colors.color_text, font, false, title.data());
        }
    }

    window_ctx.next_cursor_pos = vec2{cursor_pos.x, cursor_pos.y + size.y + 10};

    utils::misc::push_cursor_pos(vec2{cursor_pos.x + 8, cursor_pos.y + header_height + 8});

    utils::misc::push_font(font);
}

// ========================================================================
void zgui::end_groupbox() {
    utils::misc::push_cursor_pos(window_ctx.next_cursor_pos);
    window_ctx.next_cursor_pos = {};
}
// ========================================================================