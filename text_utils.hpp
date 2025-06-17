#ifndef TEXT_UTILS_HPP
#define TEXT_UTILS_HPP

#include <string>
#include <sstream>

namespace text_utils {

bool is_integer(const std::string &str);

bool is_rational(const std::string &str);

std::string add_newlines_to_long_string(const std::string &text, size_t max_chars_per_line = 25);

} // namespace text_utils

#endif // TEXT_UTILS_HPP
