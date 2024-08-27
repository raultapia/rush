/**
 * @file color.hpp
 * @brief This library provides text color and style formating utilities.
 * @author Raul Tapia (raultapia.com)
 * @copyright GNU General Public License v3.0
 * @see https://github.com/raultapia/rush
 */
#ifndef RUSH_COLOR_HPP
#define RUSH_COLOR_HPP

#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>

namespace rush::color {

/**
 * @brief Stream buffer foreground color.
 *
 * This enumeration defines various foreground colors that can be applied to text in console output.
 */
enum class Foreground : std::uint8_t {
  black = 30,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  gray,
  white = 0
};

/**
 * @brief Stream buffer background color.
 *
 * This enumeration defines various background colors that can be applied to text in console output.
 */
enum class Background : std::uint8_t {
  black = 40,
  red,
  green,
  yellow,
  blue,
  magenta,
  cyan,
  gray
};

/**
 * @brief Stream buffer style.
 *
 * This enumeration defines various text styles that can be applied to text in console output.
 */
enum class Style : std::uint8_t {
  bold = 1,
  dim,
  italic,
  underline,
  blink,
  rblink,
  reversed,
  conceal,
  crossed
};

constexpr char reset[] = "\033[0m";  /**< Escape sequence to reset text formatting */
constexpr char endl[] = "\033[0m\n"; /**< Escape sequence to reset text formatting and add a newline */

using fg = Foreground; /**< Alias for Foreground */
using bg = Background; /**< Alias for Background */
using st = Style;      /**< Alias for Style */

/*! \cond INTERNAL */
template <typename T>
using checkType = std::enable_if_t<std::is_same_v<T, Style> || std::is_same_v<T, Foreground> || std::is_same_v<T, Background>, std::ostream &>;

inline std::string escape_sequence(int x) {
  return static_cast<std::string>("\033[") + std::to_string(x) + static_cast<std::string>("m");
}
/*! \endcond */

/**
 * @brief Stream insertion operator for applying styles and colors.
 *
 * This operator allows applying styles and colors to an output stream using the defined enumerations.
 *
 * @tparam T The type of the style or color.
 * @param os The output stream.
 * @param x The style or color to apply.
 * @return The modified output stream.
 */
template <typename T>
checkType<T> operator<<(std::ostream &os, const T x) {
  os << escape_sequence(static_cast<int>(x));
  return os;
}

} // namespace rush::color

#endif // RUSH_COLOR_HPP
