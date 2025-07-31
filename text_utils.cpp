#include "text_utils.hpp"
#include <regex>

#include <string>
#include <sstream>

namespace text_utils {

std::string abbreviate_snake_case(const std::string &input) {
    std::stringstream abbrev;
    std::stringstream word_stream(input);
    std::string word;

    while (std::getline(word_stream, word, '_')) {
        if (word.empty())
            continue;
        abbrev << word[0];
    }

    return abbrev.str();
}

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

std::string join(const std::vector<std::string> &elements, const std::string &separator) {
    std::ostringstream os;
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0)
            os << separator;
        os << elements[i];
    }
    return os.str();
}

std::string trim(const std::string &s) {
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";

    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, last - first + 1);
}

std::string camel_to_snake_case(const std::string &input) {
    std::string result;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (std::isupper(c)) {
            if (i != 0) {
                result += '_';
            }
            result += std::tolower(c);
        } else {
            result += c;
        }
    }

    return result;
}

std::string join_multiline(const std::string &input, bool replace_newlines_with_space) {
    std::string result;
    std::string buffer;

    for (char c : input) {
        if (c == '\n' || c == '\r') {
            // Trim trailing whitespace from buffer
            while (!buffer.empty() && std::isspace(buffer.back()))
                buffer.pop_back();

            result += buffer;
            buffer.clear();

            if (replace_newlines_with_space && !result.empty() && result.back() != ' ')
                result += ' ';
        } else {
            // Skip leading whitespace at start of a new line
            if (buffer.empty() && std::isspace(c))
                continue;

            buffer += c;
        }
    }

    // Handle remaining buffer
    while (!buffer.empty() && std::isspace(buffer.back()))
        buffer.pop_back();
    result += buffer;

    return result;
}
std::string replace_char(const std::string &input, char from_char, char to_char) {
    std::string result = input;
    std::replace(result.begin(), result.end(), from_char, to_char);
    return result;
}

#include <string>

// Replaces all occurrences of `from_substr` with `to_substr` in `input`
std::string replace_substring(const std::string &input, const std::string &from_substr, const std::string &to_substr) {
    if (from_substr.empty())
        return input; // avoid infinite loop
    std::string result = input;
    size_t pos = 0;

    while ((pos = result.find(from_substr, pos)) != std::string::npos) {
        result.replace(pos, from_substr.length(), to_substr);
        pos += to_substr.length(); // move past the new substring
    }

    return result;
}

} // namespace text_utils
