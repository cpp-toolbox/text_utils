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
    /**
     * @brief Append values to the accumulator.
     *
     * Streams all arguments into the internal string buffer.
     *
     * @tparam Args Any streamable types.
     * @param args Values to append.
     */
    template <typename... Args> void add(Args &&...args) {
        std::ostringstream oss;
        (oss << ... << args); // fold expression to stream all args
        data_ += oss.str();
    }

    /// Clear the accumulator.
    void clear() { data_.clear(); }

    /// Get the accumulated string.
    std::string str() const { return data_; }

    /// Get the size of the accumulated string.
    size_t size() const { return data_.size(); }

  private:
    std::string data_;
};

class MultilineStringAccumulator {
  public:
    MultilineStringAccumulator() : indent_level_(0), indent_size_(4) {}

    /// Increase indentation level.
    void indent() { ++indent_level_; }

    /// Decrease indentation level (no-op if already at 0).
    void unindent() {
        if (indent_level_ > 0) {
            --indent_level_;
        }
    }

    /**
     * @brief Add a line with current indentation applied.
     * @tparam Args Any streamable types.
     * @param args Values to append to the line.
     */
    template <typename... Args> void add(Args &&...args) {
        std::ostringstream oss;
        oss << std::string(indent_level_ * indent_size_, ' '); // indent prefix
        (oss << ... << args);                                  // fold expression (C++17+)
        lines_.emplace_back(oss.str());
    }

    /**
     * @brief Add multiple lines with indentation applied.
     * @param multiline_str Input string with newlines.
     */
    void add_multiline(const std::string &multiline_str) {
        std::istringstream iss(multiline_str);
        std::string line;
        while (std::getline(iss, line)) {
            lines_.emplace_back(std::string(indent_level_ * indent_size_, ' ') + line);
        }
    }

    /**
     * @brief Insert a line at the given index.
     * @param index Position in the list of lines.
     * @param line Text to insert.
     * @throws std::out_of_range if index is invalid.
     */
    void insert_line(size_t index, const std::string &line) {
        if (index > lines_.size()) {
            throw std::out_of_range("insert_line: index out of range");
        }
        lines_.insert(lines_.begin() + index, std::string(indent_level_ * indent_size_, ' ') + line);
    }

    /**
     * @brief Insert all lines from another accumulator.
     * @param index Position in the list of lines.
     * @param other Another accumulator containing lines.
     * @throws std::out_of_range if index is invalid.
     */
    void insert_lines(size_t index, const MultilineStringAccumulator &other) {
        if (index > lines_.size()) {
            throw std::out_of_range("insert_lines: index out of range");
        }
        lines_.insert(lines_.begin() + index, other.lines_.begin(), other.lines_.end());
    }

    /**
     * @brief Insert multiple lines from a string.
     * @param index Position in the list of lines.
     * @param multiline_str String containing newlines.
     * @throws std::out_of_range if index is invalid.
     */
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

    /**
     * @brief Remove a line at the given index.
     * @param index Position in the list of lines.
     * @throws std::out_of_range if index is invalid.
     */
    void remove_line(size_t index) {
        if (index >= lines_.size()) {
            throw std::out_of_range("remove_line: index out of range");
        }
        lines_.erase(lines_.begin() + index);
    }

    /// Get the accumulated text as a single string with newlines.
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

    /// Clear all stored lines.
    void clear() { lines_.clear(); }

    /// Get the number of stored lines.
    size_t line_count() const { return lines_.size(); }

  private:
    std::vector<std::string> lines_;
    size_t indent_level_;
    size_t indent_size_;
};

// ---------------- Free functions ----------------

/**
 * @brief Remove consecutive duplicate characters from a string.
 * @param input Input string.
 * @param dedup_chars Characters to deduplicate (empty = all).
 * @return String with duplicates removed.
 */
std::string remove_consecutive_duplicates(const std::string &input, const std::string &dedup_chars = "");

/**
 * @brief Abbreviate a snake_case string by shortening each word.
 * @param input Input snake_case string.
 * @return Abbreviated string.
 */
std::string abbreviate_snake_case(const std::string &input);

/// Check if a string represents an integer.
bool is_integer(const std::string &str);

/// Check if a string represents a rational (floating-point) number.
bool is_rational(const std::string &str);

/**
 * @brief Insert newlines into long strings.
 * @param text Input string.
 * @param max_chars_per_line Maximum characters per line.
 * @return String with line breaks inserted.
 */
std::string add_newlines_to_long_string(const std::string &text, size_t max_chars_per_line = 25);

/**
 * @brief Split a string by a delimiter.
 * @param str Input string.
 * @param delimiter Delimiter string.
 * @return Vector of substrings.
 */
std::vector<std::string> split(const std::string &str, const std::string &delimiter);

/**
 * @brief Split a string once from the right.
 * @param str Input string.
 * @param delimiter Delimiter string.
 * @return Vector with up to two substrings.
 */
std::vector<std::string> split_once_from_right(const std::string &str, const std::string &delimiter);

/**
 * @brief Join elements into a single string with a separator.
 * @param elements Vector of strings.
 * @param separator Separator string.
 * @return Concatenated string.
 */
std::string join(const std::vector<std::string> &elements, const std::string &separator);

/// Trim whitespace from both ends of a string.
std::string trim(const std::string &s);

/**
 * @brief Surround a string with left and right substrings.
 * @param str Input string.
 * @param left Left surround string.
 * @param right Right surround string (default empty).
 * @return Surrounded string.
 */
std::string surround(const std::string &str, const std::string &left, const std::string &right = "");

/**
 * @brief Convert a PascalCase string to snake_case.
 * @param input Input PascalCase string.
 * @return Converted string.
 */
std::string pascal_to_snake_case(const std::string &input);

/**
 * @brief Convert a snake_case string to PascalCase.
 * @param input Input snake_case string.
 * @return Converted string.
 */
std::string snake_to_pascal_case(const std::string &input);

/**
 * @brief Join a string with newlines removed or replaced.
 * @param input Input string with multiple lines.
 * @param replace_newlines_with_space Whether to replace newlines with spaces.
 * @return Joined string.
 */
std::string join_multiline(const std::string &input, bool replace_newlines_with_space = false);

/// Replace a character with another in a string.
std::string replace_char(const std::string &input, char from_char, char to_char);

/// Replace characters in a string according to a mapping.
std::string replace_chars(const std::string &input, const std::unordered_map<char, char> &mapping);

/// Replace all occurrences of a substring with another substring.
std::string replace_substring(const std::string &input, const std::string &from_substr, const std::string &to_substr);

/// Check if a string starts with a prefix.
bool starts_with(const std::string &str, const std::string &prefix);

/// Check if a string contains a substring.
bool contains(const std::string &str, const std::string &substr);

/// Extract a substring from start to end indices.
std::string get_substring(const std::string &input, size_t start, size_t end);

/// Remove all newlines from a string.
std::string remove_newlines(const std::string &input);

/// Collapse consecutive whitespace into a single space.
std::string collapse_whitespace(const std::string &input);

/// Replace literal "\n" sequences with real newlines.
std::string replace_literal_newlines_with_real(const std::string &input);

/**
 * @brief Indent text by a given number of levels.
 * @param text Input string.
 * @param indent_level Indentation level.
 * @param spaces_per_indent Number of spaces per level.
 * @return Indented string.
 */
std::string indent(const std::string &text, int indent_level, int spaces_per_indent = 4);

/**
 * @brief Create a map from words to their abbreviations.
 * @param words List of words.
 * @return Map where keys are words and values are abbreviations.
 */
std::unordered_map<std::string, std::string> map_words_to_abbreviations(const std::vector<std::string> &words);

// startfold formatting {"attr"= value, ... }

/**
 * @struct Node
 * @brief Represents a node in a nested key-value or block structure.
 *
 * A Node can either be a simple key-value pair or a block containing child Nodes.
 */
struct Node {
    std::string key;            /**< The key of this node (empty if not applicable). */
    std::string value;          /**< The value of this node (empty if block). */
    std::vector<Node> children; /**< Child nodes if this node is a block. */
    bool is_block = false;      /**< True if this node represents a block. */
    char block_type = '{';      /**< Type of block: '{' for {}, '(' for (). */
};

/**
 * @brief Parses a block from a string starting at the given position.
 *
 * Recursively parses nested braces or parentheses into a Node tree.
 *
 * @param s The input string containing nested blocks.
 * @param pos The current parsing position (will be updated to the end of the block).
 * @return Node The parsed block node.
 */
Node parse_block(const std::string &s, size_t &pos);

/**
 * @brief Formats a nested braces string into a visual ASCII box.
 *
 * Combines parsing and formatting into a single step.
 *
 * @param input The input string containing nested blocks.
 * @return std::string The formatted ASCII box representation.
 */
std::string format_nested_braces_string_recursive_as_boxes(const std::string &input);

/**
 * @brief Convenience function to format a nested braces string with newlines and indentation.
 *
 * Combines parsing and formatting into a single step.
 *
 * @param input The input string containing nested blocks.
 * @return std::string The formatted string with newlines and indentation.
 *
 * @todo doesn't deal with unordered maps very well
 *
 */
std::string format_nested_braces_string_recursive_with_newlines(const std::string &input);

// endfold

} // namespace text_utils

#endif // TEXT_UTILS_HPP
