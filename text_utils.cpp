#include "text_utils.hpp"
#include <iostream>
#include <regex>

#include <string>
#include <sstream>
#include <unordered_set>

namespace text_utils {

std::string remove_consecutive_duplicates(const std::string &input, const std::string &dedup_chars) {
    if (input.empty())
        return "";

    std::unordered_set<char> dedup_set(dedup_chars.begin(), dedup_chars.end());

    std::string result;
    result.reserve(input.size());
    result.push_back(input[0]);

    for (size_t i = 1; i < input.size(); ++i) {
        char current = input[i];
        char prev = input[i - 1];

        bool should_dedup = dedup_chars.empty() || dedup_set.count(current) > 0;

        if (!(should_dedup && current == prev)) {
            result.push_back(current);
        }
    }

    return result;
}

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

std::vector<std::string> split_once_from_right(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> result;
    size_t delim_pos = str.rfind(delimiter);

    if (delim_pos == std::string::npos) {
        result.push_back(str);
    } else {
        result.push_back(str.substr(0, delim_pos));
        result.push_back(str.substr(delim_pos + delimiter.length()));
    }

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

std::string replace_chars(const std::string &input, const std::unordered_map<char, char> &mapping) {
    std::string result = input;
    for (char &c : result) {
        auto it = mapping.find(c);
        if (it != mapping.end()) {
            c = it->second;
        }
    }
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

std::string generate_abbreviation(const std::string &snake_case_name) {
    // replace all delimiters with a single common one (e.g., "_")
    std::string normalized = snake_case_name;
    for (char &c : normalized) {
        if (c == '/' || c == '\\' || c == '_' || c == '-' || c == '.') {
            c = '_';
        }
    }

    // Now split on "_"
    std::vector<std::string> parts = split(normalized, "_");

    std::string abbreviation;
    for (const auto &part : parts) {
        if (!part.empty()) {
            abbreviation += part[0];
        }
    }

    return abbreviation;
}

std::string generate_unique_abbreviation(std::unordered_map<std::string, std::string> &current_abbreviation_map,
                                         const std::string &word_to_abbreviate) {

    std::string abbreviation = generate_abbreviation(word_to_abbreviate);

    if (auto it = current_abbreviation_map.find(abbreviation); it != current_abbreviation_map.end()) {
        if (it->second != word_to_abbreviate) {
            int suffix = 1;
            std::string original_abbreviation = abbreviation;
            while (current_abbreviation_map.find(abbreviation) != current_abbreviation_map.end() &&
                   current_abbreviation_map[abbreviation] != word_to_abbreviate) {
                abbreviation = original_abbreviation + std::to_string(suffix++);
            }
        }
    }

    current_abbreviation_map[abbreviation] = word_to_abbreviate;
    return abbreviation;
}

std::unordered_map<std::string, std::string> map_words_to_abbreviations(const std::vector<std::string> &words) {
    std::unordered_map<std::string, std::string> abbreviation_to_word;
    std::unordered_map<std::string, std::string> word_to_abbreviation;

    for (const auto &word : words) {
        std::string abbr = generate_unique_abbreviation(abbreviation_to_word, word);
        word_to_abbreviation[word] = abbr;
    }

    return word_to_abbreviation;
}

/**
 * @brief Parses a single token from a string.
 *
 * A token is either a key, a value, or a standalone identifier.
 *
 * @param s The input string.
 * @param pos The current parsing position (will be updated to after the token).
 * @return std::string The parsed token.
 */
std::string parse_token(const std::string &s, size_t &pos) {
    std::string tok;
    while (pos < s.size() && s[pos] != '=' && s[pos] != ',' && s[pos] != '{' && s[pos] != '}' && s[pos] != '(' &&
           s[pos] != ')') {
        tok.push_back(s[pos++]);
    }
    return trim(tok);
}

Node parse_block(const std::string &s, size_t &pos) {
    Node block;
    block.is_block = true;

    if (pos < s.size() && (s[pos] == '{' || s[pos] == '(')) {
        block.block_type = s[pos];
        pos++; // consume opening
    }

    char closing = (block.block_type == '{') ? '}' : ')';

    while (pos < s.size() && s[pos] != closing) {
        Node child;

        size_t lookahead = pos;
        std::string tok = parse_token(s, lookahead);

        if (lookahead < s.size() && s[lookahead] == '=') {
            std::string key = tok;
            pos = lookahead + 1;

            if (pos < s.size() && (s[pos] == '{' || s[pos] == '(')) {
                Node inner = parse_block(s, pos);
                inner.key = trim(key);
                child = std::move(inner);
            } else {
                child.key = key;
                child.value = parse_token(s, pos);
                child.is_block = false;
            }
        } else if (pos < s.size() && (s[pos] == '{' || s[pos] == '(')) {
            child = parse_block(s, pos);
            child.key = "";
        } else {
            child.value = parse_token(s, pos);
            child.is_block = false;
        }

        block.children.push_back(child);

        if (pos < s.size() && s[pos] == ',')
            pos++; // consume comma
    }

    if (pos < s.size() && s[pos] == closing)
        pos++; // consume closing

    return block;
}

/**
 * @brief Recursively formats a Node tree into a "box" representation.
 *
 * Each Node becomes a visual ASCII box with borders and optional title.
 *
 * @param node The Node to format.
 * @return std::vector<std::string> The formatted box as a vector of strings.
 */
std::vector<std::string> format_as_boxes_from_node(const Node &node) {
    const size_t H_PAD = 3;
    const size_t V_PAD = 1;
    const size_t MIN_INNER = 8;

    struct CI {
        bool is_block;
        std::vector<std::string> buf;
        std::string text;
        size_t w;
        size_t h;
    };
    std::vector<CI> infos;
    size_t max_child_w = 0;
    size_t sum_child_h = 0;

    for (const auto &ch : node.children) {
        if (ch.is_block) {
            auto cb = format_as_boxes_from_node(ch);
            size_t cw = cb.empty() ? 0 : cb[0].size();
            infos.push_back(CI{true, cb, "", cw, cb.size()});
            max_child_w = std::max(max_child_w, cw);
            sum_child_h += cb.size();
        } else {
            std::string text;
            if (!ch.key.empty()) {
                if (!ch.value.empty())
                    text = ch.key + " = " + ch.value;
                else
                    text = ch.key;
            } else {
                text = ch.value;
            }
            size_t w = text.size();
            infos.push_back(CI{false, {}, text, w, 1});
            max_child_w = std::max(max_child_w, w);
            sum_child_h += 1;
        }
    }

    size_t title_len = trim(node.key).size();
    size_t inner_content_width = std::max({MIN_INNER, title_len, max_child_w});
    inner_content_width += 2 * H_PAD;

    size_t width = inner_content_width + 2;
    size_t n = infos.size();
    size_t height = 1 + (n + 1) * V_PAD + sum_child_h + 1;

    std::vector<std::string> buf(height, std::string(width, ' '));

    // top border with baked-in key
    if (!node.key.empty()) {
        std::string decorated = " " + node.key + " ";
        size_t left_eq = (width > decorated.size()) ? (width - decorated.size()) / 2 : 0;

        // fill with '='
        buf[0].assign(width, '=');

        // overwrite the middle with the decorated text
        for (size_t i = 0; i < decorated.size() && left_eq + i < width; ++i) {
            buf[0][left_eq + i] = decorated[i];
        }
    } else {
        buf[0].assign(width, '=');
    }

    // bottom border
    buf[height - 1].assign(width, '=');

    // vertical walls
    for (size_t r = 1; r + 1 < height; ++r) {
        buf[r][0] = '|';
        buf[r][width - 1] = '|';
    }

    // place children, left-justified
    size_t y = 1 + V_PAD;
    size_t inner_start = 1 + H_PAD;
    for (const auto &ci : infos) {
        size_t x = inner_start;

        if (ci.is_block) {
            for (size_t r = 0; r < ci.h; ++r) {
                const std::string &src = ci.buf[r];
                for (size_t c = 0; c < src.size() && x + c < width - 1; ++c) {
                    if (y + r < height - 1)
                        buf[y + r][x + c] = src[c];
                }
            }
        } else {
            for (size_t c = 0; c < ci.text.size() && x + c < width - 1; ++c) {
                if (y < height - 1)
                    buf[y][x + c] = ci.text[c];
            }
        }

        y += ci.h;
        y += V_PAD;
    }

    return buf;
}

std::string format_nested_braces_string_recursive_as_boxes(const std::string &input) {
    size_t pos = 0;
    Node root = parse_block(input, pos);
    auto buf = format_as_boxes_from_node(root);

    std::ostringstream out;
    for (auto &l : buf)
        out << l << "\n";
    return out.str();
}

/**
 * @brief Generates a string of spaces for indentation.
 *
 * @param level The indentation level (number of 2-space increments).
 * @return std::string The indentation string.
 */
std::string indent_str(int level) {
    return std::string(level * 2, ' '); // 2 spaces per level
}

/**
 * @brief Formats a Node tree into a human-readable string with newlines and indentation.
 *
 * Each block and key-value pair is placed on its own line, with indentation representing nesting.
 *
 * @param node The Node to format.
 * @param indent Current indentation level (default 0).
 * @return std::string The formatted string with newlines.
 */
std::string format_with_newlines_from_node(const Node &node, int indent) {
    std::string ind = indent_str(indent);
    std::string result;

    if (node.is_block) {
        char open_brace = node.block_type;
        char close_brace = (open_brace == '{') ? '}' : ')';

        if (!node.key.empty())
            result += ind + node.key + " = ";

        result += open_brace;
        if (!node.children.empty())
            result += "\n";

        for (size_t i = 0; i < node.children.size(); ++i) {
            result += format_with_newlines_from_node(node.children[i], indent + 1);
            if (i + 1 < node.children.size())
                result += ",\n";
            else
                result += "\n";
        }

        if (!node.children.empty())
            result += ind;
        result += close_brace;
    } else {
        if (!node.key.empty())
            result += ind + node.key + " = ";
        else
            result += ind;
        result += node.value;
    }

    return result;
}

std::string format_nested_braces_string_recursive_with_newlines(const std::string &input) {
    size_t pos = 0;
    Node root = parse_block(input, pos);

    std::ostringstream out;
    out << format_with_newlines_from_node(root, 0) << "\n"; // call the original function
    return out.str();
}

} // namespace text_utils
