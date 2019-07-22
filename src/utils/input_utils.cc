//@zgui_packer:ignore
#include "../zgui.hh"
using namespace zgui::globals;
//@zgui_packer:resume
// ========================================================================
// Function for starting our input loop.
void zgui::poll_input(std::string_view window_name)
{
	if (window_name.empty())
		throw std::exception("No window from where input should be read from specified in function parameter.");

	for (int i = 0; i < 256; i++) {
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
	}

	POINT p_mouse_pos;
	GetCursorPos(&p_mouse_pos);
	ScreenToClient(FindWindow(nullptr, window_name.data()), &p_mouse_pos);
	previous_mouse_pos = mouse_pos;
	mouse_pos = vec2{ static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y) };

	if (!input_loop_started)
		input_loop_started = true;
}
// ========================================================================
// Function for starting our input loop.
void zgui::poll_input(HWND hwnd)
{
	if (!hwnd)
		throw std::exception("No window from where input should be read from specified in function parameter.");

	for (int i = 0; i < 256; i++) {
		prev_key_state[i] = key_state[i];
		key_state[i] = GetAsyncKeyState(i);
	}

	POINT p_mouse_pos;
	GetCursorPos(&p_mouse_pos);
	ScreenToClient(hwnd, &p_mouse_pos);
	previous_mouse_pos = mouse_pos;
	mouse_pos = vec2{ static_cast<float>(p_mouse_pos.x), static_cast<float>(p_mouse_pos.y) };

	if (!input_loop_started)
		input_loop_started = true;
}
// ========================================================================
bool zgui::utils::input::key_pressed(const int key)
{
	return key_state[key] && !prev_key_state[key];
}
// ========================================================================
bool zgui::utils::input::key_down(const int key)
{
	return key_state[key];
}
// ========================================================================
bool zgui::utils::input::key_released(const int key)
{
	return !key_state[key] && prev_key_state[key];
}
// ========================================================================
// Check if mouse is hovered over specified region.
bool zgui::utils::input::mouse_in_region(const int x, const int y, const int w, const int h)
{
	return mouse_pos.x > x && mouse_pos.y > y && mouse_pos.x < w + x && mouse_pos.y < h + y;
}
// ========================================================================