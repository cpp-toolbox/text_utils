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

std::vector<std::string> split(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> result;
    size_t pos = 0;
    size_t delim_pos;
    while ((delim_pos = str.find(delimiter, pos)) != std::string::npos) {
        result.push_back(str.substr(pos, delim_pos - pos));
        pos = delim_pos + delimiter.length();
    }
    result.push_back(str.substr(pos)); // add the remaining part
    return result;
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

std::string pascal_to_snake_case(const std::string &input) {
    std::vector<std::string> parts;
    std::string current;

    for (char c : input) {
        if (std::isupper(c)) {
            if (!current.empty()) {
                parts.push_back(current);
            }
            current = std::string(1, std::tolower(c));
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        parts.push_back(current);
    }

    return join(parts, "_");
}

std::string snake_to_pascal_case(const std::string &input) {
    std::vector<std::string> parts = split(input, "_");
    for (std::string &part : parts) {
        if (!part.empty()) {
            part[0] = std::toupper(part[0]);
        }
    }
    return join(parts, "");
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

bool starts_with(const std::string &str, const std::string &prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

bool contains(const std::string &str, const std::string &substr) { return str.find(substr) != std::string::npos; }

std::string get_substring(const std::string &input, size_t start, size_t end) {
    if (start >= end || end > input.size()) {
        return ""; // or throw std::out_of_range if you want stricter handling
    }
    return input.substr(start, end - start);
}

std::string remove_newlines(const std::string &input) {
    std::string result;
    result.reserve(input.size());

    for (char c : input) {
        if (c != '\n' && c != '\r') {
            result += c;
        }
    }

    return result;
}

std::string collapse_whitespace(const std::string &input) {
    std::string result;
    result.reserve(input.size());

    bool in_whitespace = false;
    for (char c : input) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!in_whitespace) {
                // Add a single space for the first whitespace in a run
                result += ' ';
                in_whitespace = true;
            }
            // else skip additional whitespace chars
        } else {
            result += c;
            in_whitespace = false;
        }
    }

    return result;
}

std::string replace_literal_newlines_with_real(const std::string &input) {
    std::string output;
    output.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size() && input[i + 1] == 'n') {
            output.push_back('\n'); // real newline
            ++i;                    // skip 'n'
        } else {
            output.push_back(input[i]);
        }
    }
    return output;
}

std::string indent(const std::string &text, int indent_level, int spaces_per_indent) {
    std::istringstream iss(text);
    std::ostringstream oss;
    std::string line;
    std::string indent(indent_level * spaces_per_indent, ' ');

    while (std::getline(iss, line)) {
        oss << indent << line << '\n';
    }

    return oss.str();
}

std::string surround(const std::string &str, const std::string &left, const std::string &right) {
    return left + str + (right.empty() ? left : right);
}

} // namespace text_utils
