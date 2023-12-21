/**
 * @file chrono.hpp
 * @brief This library provides chrono utilities.
 * @author Raul Tapia (raultapia.com)
 * @copyright GNU General Public License v3.0
 * @see https://github.com/raultapia/rush
 */
#ifndef RUSH_CHRONO_HPP
#define RUSH_CHRONO_HPP

#include <chrono>
#include <iostream>
#include <ratio>
#include <string>
#include <utility>

namespace rush::chrono {

/*! \cond INTERNAL */
#define DEFINE_UNIT_MACRO(name, unit)    \
  template <>                            \
  struct Unit<name> {                    \
    static constexpr const char *str() { \
      return unit;                       \
    }                                    \
  };

template <typename T>
struct Unit;

DEFINE_UNIT_MACRO(std::nano, "ns")
DEFINE_UNIT_MACRO(std::micro, "us")
DEFINE_UNIT_MACRO(std::milli, "ms")
DEFINE_UNIT_MACRO(std::ratio<1>, "s")
DEFINE_UNIT_MACRO(std::ratio<60>, "min")
DEFINE_UNIT_MACRO(std::ratio<3600>, "h")
DEFINE_UNIT_MACRO(std::ratio<86400>, "day")
/*! \endcond */

using ns = std::nano;          ///< Convenience alias for nanoseconds
using us = std::micro;         ///< Convenience alias for microseconds
using ms = std::milli;         ///< Convenience alias for milliseconds
using s = std::ratio<1>;       ///< Convenience alias for seconds
using min = std::ratio<60>;    ///< Convenience alias for minutes
using hour = std::ratio<3600>; ///< Convenience alias for hours
using day = std::ratio<86400>; ///< Convenience alias for days

/**
 * @brief A simple chrono class to measure elapsed time.
 *
 * @tparam T Unit of time (default to seconds).
 */
template <typename T = s>
class Chrono {
public:
  Chrono() : t0_(std::chrono::high_resolution_clock::now()) {}
  ~Chrono() = default;
  Chrono(const Chrono &) = delete;
  Chrono(Chrono &&) noexcept = delete;
  Chrono &operator=(const Chrono &) = delete;
  Chrono &operator=(Chrono &&other) noexcept = delete;

  /**
   * @brief Reset the chrono timer.
   *
   */
  inline void tic() {
    t0_ = std::chrono::high_resolution_clock::now();
  }

  /**
   * @brief Get elapsed time since last tic.
   *
   * @return Elapsed time in the specified unit.
   */
  [[nodiscard]] inline double toc() const {
    return std::chrono::duration_cast<std::chrono::duration<double, T>>(std::chrono::high_resolution_clock::now() - t0_).count();
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> t0_;
};

/**
 * @brief A chrono class that automatically prints elapsed time.
 *
 * @tparam T Unit of time (default to seconds).
 */
template <typename T = s>
class Chronometer : public Chrono<T> {
public:
  /**
   * @brief Constructor.
   *
   * @param name Optional name to be printed along with elapsed time.
   */
  explicit Chronometer(std::string name = "") : name_{std::move(name)} {}

  ~Chronometer() {
    double t = Chrono<T>::toc();
    if(!name_.empty()) {
      std::cout << "[" << name_ << "] ";
    }
    std::cout << "Elapsed time: " << t << " " << Unit<T>().str() << std::endl;
  }

  Chronometer(const Chronometer &) = delete;
  Chronometer(Chronometer &&) noexcept = delete;
  Chronometer &operator=(const Chronometer &) = delete;
  Chronometer &operator=(Chronometer &&other) noexcept = delete;

private:
  std::string name_;
};

} // namespace rush::chrono

#endif // RUSH_CHRONO_HPP
