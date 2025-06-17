#include "text_utils.hpp"
#include <regex>

namespace text_utils {

bool is_integer(const std::string &str) {
    // create a stringstream from the input string
    std::stringstream ss(str);
    // declare a temporary integer variable to hold the value
    int temp;
    // try to extract an integer from the stringstream and ensure the whole string was processed
    return (ss >> temp && ss.eof());
}

bool is_rational(const std::string &value) {
    static const std::regex rational_regex(R"(^-?(\d+|\.\d+)(\.\d+)?$)");
    return std::regex_match(value, rational_regex);
}

std::string add_newlines_to_long_string(const std::string &text, size_t max_chars_per_line) {
    std::istringstream stream(text);
    std::ostringstream formatted;
    std::string word;
    size_t current_line_length = 0;

    while (stream >> word) {
        // If the word would exceed the line length, insert a newline first
        if (current_line_length + word.length() + (current_line_length > 0 ? 1 : 0) > max_chars_per_line) {
            formatted << '\n';
            current_line_length = 0;
        }

        // Add a space if this isn't the first word on the line
        if (current_line_length > 0) {
            formatted << ' ';
            current_line_length++;
        }

        formatted << word;
        current_line_length += word.length();
    }

    return formatted.str();
}

} // namespace text_utils
