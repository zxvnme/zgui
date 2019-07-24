//@zgui_packer:ignore
#include "../zgui.hh"

using namespace zgui::globals;

//@zgui_packer:resume
// ========================================================================
std::vector<std::string> zgui::utils::hash::split_str(const char *str, const char separator) {
    std::vector<std::string> output;
    std::string substring;
    std::istringstream stream{str};

    while (std::getline(stream, substring, separator))
        output.push_back(substring);

    return output;
}
// ========================================================================
uint32_t zgui::utils::hash::hash(const char *str, const uint32_t value) {
    return *str ? hash(str + 1, (value ^ *str) * 0x1000193ull) : value;
}
// ========================================================================