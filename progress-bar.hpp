/**
 * @file cv-highgui.hpp
 * @brief This library enables progress bar creation and management.
 * @author Raul Tapia (raultapia.com)
 * @copyright GNU General Public License v3.0
 * @see https://github.com/raultapia/rush
 */
#ifndef RUSH_PROGRESS_BAR_HPP
#define RUSH_PROGRESS_BAR_HPP

#include "rush/string.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <utility>

namespace rush::progress {

/**
 * @brief Configuration structure for the progress bar.
 */
struct Configuration {
  std::string name;                                 ///< Name displayed alongside the progress bar.
  std::string complete{"="};                        ///< Character used to represent completed progress.
  std::string uncomplete{" "};                      ///< Character used to represent uncompleted progress.
  std::array<std::string, 2> decorator{{"[", "]"}}; ///< Decorator strings surrounding the progress bar.
  bool percentage{true};                            ///< Flag indicating whether to display percentage completion.
  double decimals{0};                               ///< Number of decimal places for the percentage display.
};

class Bar {
private:
  int width_{0};
  double max_{0};
  double current_{0};
  Configuration config_;

  double update(double p) {
    p = std::clamp(p, 0.0, 1.0);

    std::string s = std::string("\033[0m") + config_.name;
    if(!config_.name.empty()) {
      s += " ";
    }
    s += config_.decorator[0];
    s += static_cast<rush::string>(config_.complete) * static_cast<int>(p * width_);
    s += static_cast<rush::string>(config_.uncomplete) * (width_ - static_cast<int>(p * width_));
    s += config_.decorator[1];

    if(config_.percentage) {
      s += " " + std::to_string(p * 100).substr(0, std::to_string(p * 100).find('.') + (config_.decimals > 0) + config_.decimals) + "%";
    }

    std::cout << "\u001b[1000D\033[31m\033[41m" << s << std::flush;
    return p;
  }

public:
  /**
   * @brief Construct a new Bar object.
   * @param max Maximum value representing 100% progress.
   * @param cfg Configuration for the progress bar appearance.
   */
  explicit Bar(const double max, Configuration cfg = Configuration()) : max_{max}, config_{std::move(cfg)} {
    struct winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width_ = w.ws_col - config_.decorator.size() - config_.name.size() - config_.decimals - 7;
  }

  /*! \cond INTERNAL */
  ~Bar() {
    std::cout << '\n';
  }
  /*! \endcond */

  /**
   * @brief Update the progress bar with a specific value.
   * @param x New current value.
   * @return Updated progress ratio.
   */
  double operator()(const double x) {
    return update((current_ = x) / max_);
  }

  /**
   * @brief Increment the progress bar by 1.
   * @return Updated progress ratio.
   */
  double operator++() {
    return update(++current_ / max_);
  }

  /**
   * @brief Increment the progress bar by 1 (postfix).
   * @return Updated progress ratio.
   */
  double operator++(int) {
    return update(++current_ / max_);
  }

  /**
   * @brief Increment the progress bar by a specified value.
   * @param n Value to increment by.
   * @return Updated progress ratio.
   */
  double operator+=(const int n) {
    return update((current_ += n) / max_);
  }

  /**
   * @brief Decrement the progress bar by 1.
   * @return Updated progress ratio.
   */
  double operator--() {
    return update(--current_ / max_);
  }

  /**
   * @brief Decrement the progress bar by 1 (postfix).
   * @return Updated progress ratio.
   */
  double operator--(int) {
    return update(--current_ / max_);
  }

  /**
   * @brief Decrement the progress bar by a specified value.
   * @param n Value to decrement by.
   * @return Updated progress ratio.
   */
  double operator-=(const int n) {
    return update((current_ -= n) / max_);
  }
};

} // namespace rush::progress

#endif // RUSH_PROGRESS_BAR_HPP
