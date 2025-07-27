#ifndef TEXT_UTILS_HPP
#define TEXT_UTILS_HPP

#include <string>
#include <sstream>
#include <vector>

namespace text_utils {

inline const std::string tab = "    ";

class MultilineStringAccumulator {
  public:
    template <typename... Args> void add(Args &&...args) {
        std::ostringstream oss;
        (oss << ... << args); // fold expression (c++17+)
        lines_.emplace_back(oss.str());
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
