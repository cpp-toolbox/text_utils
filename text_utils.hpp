#ifndef TEXT_UTILS_HPP
#define TEXT_UTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace text_utils {

inline const std::string tab = "    ";

class MultilineStringAccumulator {
  public:
    template <typename... Args> void add(Args &&...args) {
        std::ostringstream oss;
        (oss << ... << args); // fold expression (C++17+)
        lines_.emplace_back(oss.str());
    }

    void add_multiline(const std::string &multiline_str) {
        std::istringstream iss(multiline_str);
        std::string line;
        while (std::getline(iss, line)) {
            lines_.emplace_back(std::move(line));
        }
    }

    void insert_line(size_t index, const std::string &line) {
        if (index > lines_.size()) {
            throw std::out_of_range("insert_line: index out of range");
        }
        lines_.insert(lines_.begin() + index, line);
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
            new_lines.emplace_back(std::move(line));
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
};

std::string abbreviate_snake_case(const std::string &input);

bool is_integer(const std::string &str);

bool is_rational(const std::string &str);

std::string add_newlines_to_long_string(const std::string &text, size_t max_chars_per_line = 25);

std::string join(const std::vector<std::string> &elements, const std::string &separator);
std::string trim(const std::string &s);

} // namespace text_utils

#endif // TEXT_UTILS_HPP
