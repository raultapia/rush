/**
 * @file algorithm.hpp
 * @brief This library extends C++ std algorithm.
 * @author Raul Tapia (raultapia.com)
 * @copyright GNU General Public License v3.0
 * @see https://github.com/raultapia/rush
 */
#ifndef RUSH_ALGORITHM_HPP
#define RUSH_ALGORITHM_HPP

#include <algorithm>

namespace rush {

/**
 * @brief Clamps a value within the inclusive range [lo, hi].
 *
 * This function ensures that the value `v` is within the range specified by `lo` and `hi`.
 * If `v` is less than `lo`, it returns `lo`. If `v` is greater than `hi`, it returns `hi`.
 * Otherwise, it returns `v`.
 *
 * @tparam T The type of the value and the bounds.
 * @param v The value to clamp.
 * @param lo The lower bound of the range.
 * @param hi The upper bound of the range.
 * @return constexpr const T& The clamped value.
 */
template <class T>
constexpr const T &clamp(const T &v, const T &lo, const T &hi) {
  return std::clamp(v, lo, hi);
}

/**
 * @brief Clamps a value to be not less than a specified lower bound.
 *
 * This function ensures that the value `v` is not less than the lower bound `lo`.
 * If `v` is less than `lo`, it returns `lo`. Otherwise, it returns `v`.
 *
 * @tparam T The type of the value and the lower bound.
 * @param v The value to clamp.
 * @param lo The lower bound.
 * @return constexpr const T& The clamped value.
 */
template <class T>
constexpr const T &clampl(const T &v, const T &lo) {
  return (v < lo) ? lo : v;
}

/**
 * @brief Clamps a value to be not greater than a specified upper bound.
 *
 * This function ensures that the value `v` is not greater than the upper bound `hi`.
 * If `v` is greater than `hi`, it returns `hi`. Otherwise, it returns `v`.
 *
 * @tparam T The type of the value and the upper bound.
 * @param v The value to clamp.
 * @param hi The upper bound.
 * @return constexpr const T& The clamped value.
 */
template <class T>
constexpr const T &clamph(const T &v, const T &hi) {
  return (v > hi) ? hi : v;
}

} // namespace rush

#endif // RUSH_ALGORITHM_HPP
