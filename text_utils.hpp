#ifndef TEXT_UTILS_HPP
#define TEXT_UTILS_HPP

#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace text_utils {

inline const std::string double_quote = "\"";
inline const std::string single_quote = "'";
inline const std::string newline = "\n";
inline const std::string tab = "\t";
inline const std::string space = " ";
inline const std::string empty = "";
inline const std::string comma = ",";
inline const std::string period = ".";
inline const std::string colon = ":";
inline const std::string semicolon = ";";
inline const std::string dash = "-";
inline const std::string underscore = "_";
inline const std::string slash = "/";
inline const std::string backslash = "\\";
inline const std::string pipe = "|";
inline const std::string ampersand = "&";
inline const std::string at_sign = "@";
inline const std::string hash = "#";
inline const std::string dollar = "$";
inline const std::string percent = "%";
inline const std::string caret = "^";
inline const std::string asterisk = "*";
inline const std::string plus = "+";
inline const std::string equals = "=";
inline const std::string question_mark = "?";
inline const std::string exclamation_mark = "!";
inline const std::string left_paren = "(";
inline const std::string right_paren = ")";
inline const std::string left_bracket = "[";
inline const std::string right_bracket = "]";
inline const std::string left_brace = "{";
inline const std::string right_brace = "}";
inline const std::string less_than = "<";
inline const std::string greater_than = ">";
inline const std::string newline_windows = "\r\n";
inline const std::string carriage_return = "\r";

inline const std::string natural_numbers = "ℕ";
inline const std::string element_of = "∈";

class StringAccumulator {
  public:
    template <typename... Args> void add(Args &&...args) {
        std::ostringstream oss;
        (oss << ... << args); // fold expression to stream all args
        data_ += oss.str();
    }

    void clear() { data_.clear(); }

    std::string str() const { return data_; }

    size_t size() const { return data_.size(); }

  private:
    std::string data_;
};

class MultilineStringAccumulator {
  public:
    MultilineStringAccumulator() : indent_level_(0), indent_size_(4) {}

    void indent() { ++indent_level_; }
    void unindent() {
        if (indent_level_ > 0) {
            --indent_level_;
        }
    }

    template <typename... Args> void add(Args &&...args) {
        std::ostringstream oss;
        oss << std::string(indent_level_ * indent_size_, ' '); // indent prefix
        (oss << ... << args);                                  // fold expression (C++17+)
        lines_.emplace_back(oss.str());
    }

    void add_multiline(const std::string &multiline_str) {
        std::istringstream iss(multiline_str);
        std::string line;
        while (std::getline(iss, line)) {
            lines_.emplace_back(std::string(indent_level_ * indent_size_, ' ') + line);
        }
    }

    void insert_line(size_t index, const std::string &line) {
        if (index > lines_.size()) {
            throw std::out_of_range("insert_line: index out of range");
        }
        lines_.insert(lines_.begin() + index, std::string(indent_level_ * indent_size_, ' ') + line);
    }

    void insert_lines(size_t index, const MultilineStringAccumulator &other) {
        if (index > lines_.size()) {
            throw std::out_of_range("insert_lines: index out of range");
        }
        lines_.insert(lines_.begin() + index, other.lines_.begin(), other.lines_.end());
    }

    void insert_multiline(size_t index, const std::string &multiline_str) {
        if (index > lines_.size()) {
            throw std::out_of_range("insert_multiline: index out of range");
        }

        std::istringstream iss(multiline_str);
        std::string line;
        std::vector<std::string> new_lines;

        while (std::getline(iss, line)) {
            new_lines.emplace_back(std::string(indent_level_ * indent_size_, ' ') + line);
        }

        lines_.insert(lines_.begin() + index, new_lines.begin(), new_lines.end());
    }

    void remove_line(size_t index) {
        if (index >= lines_.size()) {
            throw std::out_of_range("remove_line: index out of range");
        }
        lines_.erase(lines_.begin() + index);
    }

    std::string str() const {
        std::ostringstream oss;
        for (size_t i = 0; i < lines_.size(); ++i) {
            oss << lines_[i];
            if (i + 1 < lines_.size()) {
                oss << '\n';
            }
        }
        return oss.str();
    }

    void clear() { lines_.clear(); }

    size_t line_count() const { return lines_.size(); }

  private:
    std::vector<std::string> lines_;
    size_t indent_level_;
    size_t indent_size_;
};

std::string remove_consecutive_duplicates(const std::string &input, const std::string &dedup_chars = "");

std::string abbreviate_snake_case(const std::string &input);

bool is_integer(const std::string &str);
bool is_rational(const std::string &str);

std::string add_newlines_to_long_string(const std::string &text, size_t max_chars_per_line = 25);

std::vector<std::string> split(const std::string &str, const std::string &delimiter);
std::vector<std::string> split_once_from_right(const std::string &str, const std::string &delimiter);
std::string join(const std::vector<std::string> &elements, const std::string &separator);
std::string trim(const std::string &s);
std::string surround(const std::string &str, const std::string &left, const std::string &right = "");

// template <typename... Args> std::string concat(const Args &...args) { return (std::string(args) + ...); }

std::string pascal_to_snake_case(const std::string &input);
std::string snake_to_pascal_case(const std::string &input);

std::string join_multiline(const std::string &input, bool replace_newlines_with_space = false);

std::string replace_char(const std::string &input, char from_char, char to_char);
std::string replace_chars(const std::string &input, const std::unordered_map<char, char> &mapping);
std::string replace_substring(const std::string &input, const std::string &from_substr, const std::string &to_substr);

bool starts_with(const std::string &str, const std::string &prefix);
bool contains(const std::string &str, const std::string &substr);

std::string get_substring(const std::string &input, size_t start, size_t end);

std::string remove_newlines(const std::string &input);
std::string collapse_whitespace(const std::string &input);
std::string replace_literal_newlines_with_real(const std::string &input);
std::string indent(const std::string &text, int indent_level, int spaces_per_indent = 4);

std::unordered_map<std::string, std::string> map_words_to_abbreviations(const std::vector<std::string> &words);

/**
 * @brief Node in the parsed representation of a nested brace string.
 */
struct Node {
    std::string key;
    std::string value;
    std::vector<Node> children;
    bool is_block = false;
};

/**
 * @brief Parse a `{...}` block from a string into a Node tree.
 *
 * @param s   Input string containing the data.
 * @param pos Position index (will be advanced as parsing progresses).
 * @return Parsed Node representing the block.
 */
Node parse_block(const std::string &s, size_t &pos);

/**
 * @brief Parse a token (identifier, number, etc.) from the input string.
 *
 * @param s   Input string.
 * @param pos Position index (will be advanced).
 * @return Parsed token as a string.
 */
std::string parse_token(const std::string &s, size_t &pos);

/**
 * @brief Recursively build a 2D buffer of the formatted representation.
 *
 * @param node Root node to format.
 * @return Vector of strings, one per line of the formatted box.
 */
std::vector<std::string> build_buffer(const Node &node);

/**
 * @brief Recursively formats nested key-value data into boxed, pretty-printed
 * sections.
 *
 * The input string must follow this format:
 * - Each entry is `key=value`.
 * - Values may either be scalars (numbers, identifiers, strings) or nested
 *   blocks `{...}`.
 * - Entries inside `{}` are separated by commas `,`.
 *
 * Example input:
 * @code
 * {header={type=PacketType::KEYBOARD_MOUSE_UPDATE,
 * size_of_data_without_header=6}, id=32, mku={fire_pressed=1,
 * forward_pressed=0}}
 * @endcode
 *
 * Example output:
 * @code
 * ========================================================
 * |                                                     |
 * |   =====================header=====================  |
 * |   |                                               | |
 * |   |   type = PacketType::KEYBOARD_MOUSE_UPDATE    | |
 * |   |                                               | |
 * |   |   size_of_data_without_header = 6             | |
 * |   |                                               | |
 * |   ================================================  |
 * |                                                     |
 * |   id = 32                                           |
 * |                                                     |
 * |   ============mku============                       |
 * |   |                             |                   |
 * |   |   fire_pressed = 1          |                   |
 * |   |                             |                   |
 * |   |   forward_pressed = 0       |                   |
 * |   |                             |                   |
 * |   ===============================                   |
 * |                                                     |
 * ========================================================
 * @endcode
 */
std::string format_nested_brace_string_recursive(const std::string &input);

} // namespace text_utils

#endif // TEXT_UTILS_HPP
