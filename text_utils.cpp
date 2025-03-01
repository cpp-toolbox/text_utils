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

} // namespace text_utils
