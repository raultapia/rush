/**
 * @file string.hpp
 * @brief This library provides string-related utilities.
 * @author Raul Tapia (raultapia.com)
 * @copyright GNU General Public License v3.0
 * @see https://github.com/raultapia/rush
 */
#ifndef RUSH_STRING_HPP
#define RUSH_STRING_HPP

#include "rush/algorithm.hpp"
#include "rush/color.hpp"
#include <cstddef>
#include <string>
#include <type_traits>

namespace rush {

/**
 * @brief This class extends std::string
 */
class string : public std::string {
  using std::string::string;

public:
  explicit string(const std::string &str) : std::string(str) {}

  /**
   * @brief Repetition operator: repeat the string a specified number of times.
   *
   * This method creates a new string consisting of the original string
   * repeated a specified number of times.
   *
   * @param times The number of times to repeat the string.
   * @return A new string containing the repeated sequence.
   *
   * @example
   * @code
   * rush::string s("abc");
   * rush::string result = s * 3; // result is "abcabcabc"
   * @endcode
   */
  string operator*(int times) const {
    rush::clampl(times, 0);
    string r;
    r.reserve(string::size() * times);
    while(times-- > 0) {
      r += *this;
    }
    return r;
  }

  /**
   * @brief Format operator: Apply a color or style to the string.
   *
   * This method formats the string with a given color or style
   * by adding the appropriate escape sequences.
   *
   * @tparam T The type of the color or style, which must be one of rush::color::fg, rush::color::bg, or rush::color::st.
   * @param x The color or style to apply.
   * @return A new string formatted with the specified color or style.
   *
   * @example
   * @code
   * rush::string s("Hello");
   * rush::string result = s | rush::color::fg::red; // result is the string "Hello" in red color
   * @endcode
   */
  template <typename T>
  string operator|(const T x) const {
    static_assert(std::is_same<T, rush::color::fg>::value || std::is_same<T, rush::color::bg>::value || std::is_same<T, rush::color::st>::value);
    constexpr size_t s1 = std::char_traits<char>::length(rush::color::reset);
    const std::size_t s2 = string::size();
    if(s1 >= s2 || string::substr(s2 - s1) != rush::color::reset) {
      return static_cast<string>(rush::color::escape_sequence(static_cast<int>(x)) + *this + rush::color::reset);
    }
    return static_cast<string>(rush::color::escape_sequence(static_cast<int>(x)) + *this);
  }

  /**
   * @brief Replace all occurrences of a substring with another substring.
   *
   * This method replaces all occurrences of the substring `from` with the substring `to`
   * in the current string and returns the modified string.
   *
   * @param from The substring to be replaced.
   * @param to The substring to replace `from` with.
   * @return A new string with all occurrences of `from` replaced by `to`.
   *
   * @example
   * @code
   * rush::string s("one two one two");
   * rush::string result = s.replaceSubstr("one", "three"); // result is "three two three two"
   * @endcode
   */
  string replaceSubstr(const std::string &from, const std::string &to) {
    const std::size_t lf = from.length();
    const std::size_t lt = to.length();
    std::size_t index = -lt;
    string ret = *this;
    while((index = ret.find(from, index + lt)) != std::string::npos) {
      ret.replace(index, lf, to);
    }
    return ret;
  }

  /**
   * @brief Count occurrences of a substring within the string.
   *
   * This method counts and returns the number of non-overlapping occurrences
   * of the substring `substr` within the current string.
   *
   * @param substr The substring to search for.
   * @return The number of times `substr` appears in the string.
   *
   * @example
   * @code
   * rush::string s("one two one two");
   * int count = s.countSubstr("one"); // count is 2
   * @endcode
   */
  int countSubstr(const std::string &substr) {
    const std::size_t n = substr.length();
    int count = 0;
    std::size_t index = -n;
    while((index = std::string::find(substr, index + n)) != std::string::npos) {
      ++count;
    }
    return count;
  }
};

} // namespace rush

#endif // RUSH_STRING_HPP
