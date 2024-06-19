/**
 * @file counter.hpp
 * @brief This library provides counters.
 * @author Raul Tapia (raultapia.com)
 * @copyright GNU General Public License v3.0
 * @see https://github.com/raultapia/rush
 */
#ifndef RUSH_COUNTER_HPP
#define RUSH_COUNTER_HPP

#include <limits>
#include <utility>

namespace rush {

/**
 * @brief A generic counter class with customizable initial value and step.
 *
 * @tparam T The type of the counter, default is unsigned long.
 */
template <typename T = unsigned long>
class Counter {
public:
  /**
   * @brief Constructor
   *
   * @param init The initial value of the counter.
   * @param step The step value for each increment or decrement.
   */
  explicit Counter(const T init = 0, const T step = 1) : init_{init}, step_{step}, counter_{init} {}

  /**
   * @brief Increment the counter by the step value and return the previous value.
   *
   * @return The value of the counter before incrementing.
   */
  T operator()() {
    return std::exchange(counter_, counter_ + step_);
  }

  /**
   * @brief Pre-increment the counter by the step value.
   *
   * @return The new value of the counter after incrementing.
   */
  T operator++() {
    return counter_ += step_;
  }

  /**
   * @brief Post-increment the counter by the step value.
   *
   * @return The value of the counter before incrementing.
   */
  T operator++(int) {
    return std::exchange(counter_, counter_ + step_);
  }

  /**
   * @brief Increment the counter by n steps.
   *
   * @param n The number of steps to increment.
   * @return The value of the counter after incrementing.
   */
  T operator+=(const int n) {
    return std::exchange(counter_, counter_ + n * step_);
  }

  /**
   * @brief Pre-decrement the counter by the step value.
   *
   * @return The new value of the counter after decrementing.
   */
  T operator--() {
    return counter_ -= step_;
  }

  /**
   * @brief Post-decrement the counter by the step value.
   *
   * @return The value of the counter before decrementing.
   */
  T operator--(int) {
    return std::exchange(counter_, counter_ - step_);
  }

  /**
   * @brief Decrement the counter by n steps.
   *
   * @param n The number of steps to decrement.
   * @return The value of the counter after decrementing.
   */
  T operator-=(const int n) {
    return std::exchange(counter_, counter_ - n * step_);
  }

  /**
   * @brief Set the counter to a specific value.
   *
   * @param value The value to set the counter to.
   */
  void set(const T value) {
    counter_ = value;
  }

  /**
   * @brief Reset the counter to its initial value.
   */
  void reset() {
    counter_ = init_;
  }

private:
  const T init_;
  const T step_;
  T counter_;
};

/**
 * @brief A counter class that resets after reaching a specified limit.
 *
 * @tparam T The type of the counter, default is unsigned long.
 */
template <typename T = unsigned long>
class RangeCounter : public Counter<T> {
public:
  /**
   * @brief Constructor.
   *
   * @param init The initial value of the counter.
   * @param reset The value at which the counter resets to zero.
   * @param step The step value for each increment.
   */
  explicit RangeCounter(const T init = 0, const T reset = std::numeric_limits<T>::max(), const T step = 1) : Counter<T>(0, step), offset_{init}, reset_{reset - init} {}

  /**
   * @brief Increment the counter by the step value and return the previous value, modulo the reset value.
   *
   * @return The value of the counter before incrementing, modulo the reset value.
   */
  T operator()() {
    return Counter<T>::operator()() % reset_ + offset_;
  }

  /**
   * @brief Pre-increment the counter by the step value, modulo the reset value.
   *
   * @return The new value of the counter after incrementing, modulo the reset value.
   */
  T operator++() {
    return Counter<T>::operator++() % reset_ + offset_;
  }

  /**
   * @brief Post-increment the counter by the step value, modulo the reset value.
   *
   * @return The value of the counter before incrementing, modulo the reset value.
   */
  T operator++(int) {
    return Counter<T>::operator++(0) % reset_ + offset_;
  }

  /**
   * @brief Increment the counter by n steps, modulo the reset value.
   *
   * @param n The number of steps to increment.
   * @return The value of the counter after incrementing, modulo the reset value.
   */
  T operator+=(const int n) {
    return Counter<T>::operator+=(n) % reset_ + offset_;
  }

  /*! \cond INTERNAL */
  T operator--() = delete;
  T operator--(int) = delete;
  T operator-=(const int n) = delete;
  /*! \endcond */

private:
  const T offset_;
  const T reset_;
};

} // namespace rush

#endif // RUSH_COUNTER_HPP
