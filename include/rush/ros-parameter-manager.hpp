/**
 * @file ros-parameter-manager.hpp
 * @brief This library provides functionalities for loading, accessing, and manipulating ROS parameters.
 * @author Raul Tapia (raultapia.com)
 */
#ifndef RUSH_ROS_PARAMETER_MANAGER_HPP
#define RUSH_ROS_PARAMETER_MANAGER_HPP

#include <algorithm>
#include <chrono>
#include <iosfwd>
#include <iterator>
#include <map>
#include <ros/param.h>
#include <ros/this_node.h>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <xmlrpcpp/XmlRpcValue.h>

namespace rush::ros {

/**
 * @brief This class wraps an XmlRpcValue for convenience.
 */
class ParamValue {
public:
  ParamValue() = default;
  explicit ParamValue(const XmlRpc::XmlRpcValue &v) : value_{v} {};

  /**
   * @brief Convert the stored value to a specific type.
   * @tparam T The type to convert to.
   * @return The converted value.
   */
  template <typename T>
  T as() {
    return static_cast<T>(value_);
  }

  /**
   * @brief Convert the stored value to a specific type and store it in the given variable.
   * @tparam T The type to convert to.
   * @param x Reference to the variable where the converted value will be stored.
   */
  template <typename T>
  void as(T &x) {
    x = static_cast<T>(value_);
  }

  /**
   * @brief Convert the stored value to a vector of a specific type.
   * @tparam T The type to convert to.
   * @return The vector of converted values.
   */
  template <typename T>
  std::vector<T> as_vec() {
    std::vector<T> ret;
    as_vec(ret);
    return ret;
  }

  /**
   * @brief Convert the stored value to a vector of a specific type and store it in the given vector.
   * @tparam T The type to convert to.
   * @param x Reference to the vector where the converted values will be stored.
   */
  template <typename T>
  void as_vec(std::vector<T> &x) {
    if(!x.empty()) {
      x.clear();
    }
    int i = 0;
    while(i < value_.size()) {
      x.push_back(static_cast<T>(value_[i++]));
    }
  }

  /**
   * @brief Overloaded stream insertion operator to print the stored value.
   * @param os Output stream.
   * @param obj ParamValue object to be printed.
   * @return The output stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const ParamValue &obj) {
    os << obj.value_;
    return os;
  }

private:
  XmlRpc::XmlRpcValue value_;
};

/**
 * @brief This class represents a map of parameter names to ParamValue objects.
 */
class ParamMapper : public std::map<std::string, ParamValue> {
public:
  ParamMapper() = default;

  /**
   * @brief Constructor that loads parameters from a specified namespace.
   * @param ns The namespace to load parameters from.
   */
  explicit ParamMapper(const std::string &ns) {
    load(ns);
  }

  /**
   * @brief Overloaded subscript operator to access a parameter by name.
   * @param key The name of the parameter.
   * @return Reference to the ParamValue associated with the parameter name.
   * @throws std::runtime_error if the parameter name is not found.
   */
  ParamValue &operator[](const std::string &key) {
    if(std::map<std::string, ParamValue>::count(key) < 1) {
      throw std::runtime_error("Key " + key + " not found");
    }
    return std::map<std::string, ParamValue>::operator[](key);
  }

  /**
   * @brief Load parameters from a specified namespace.
   * @param ns The namespace to load parameters from.
   */
  void load(std::string ns = "") {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ns_.insert(ns);

    std::vector<std::string> names;
    XmlRpc::XmlRpcValue v;
    if(ns.empty() || ns.back() != '/') {
      ns += '/';
    }
    if(ns.front() != '/') {
      ns = ::ros::this_node::getNamespace() + ns;
    }

    ::ros::param::getParamNames(names);
    names.erase(std::remove_if(names.begin(), names.end(), [&ns](const std::string &s) { return static_cast<bool>(s.compare(0, ns.size(), ns)); }), names.end());

    for(const std::string &n : names) {
      ::ros::param::get(n, v);
      const std::string key = n.substr(ns.size());
      if(std::map<std::string, ParamValue>::count(key) < 1) {
        std::map<std::string, ParamValue>::emplace(std::make_pair(key, v));
      } else {
        std::map<std::string, ParamValue>::operator[](key) = ParamValue(v);
      }
    }
  }

  /**
   * @brief Reload parameters from all stored namespaces.
   */
  void reload() {
    std::map<std::string, ParamValue>::clear();
    for(const std::string &ns : ns_) {
      load(ns);
    }
  }

  /**
   * @brief Get a vector of parameter names in the map.
   * @return Vector of parameter names.
   */
  [[nodiscard]] std::vector<std::string> getKeys() const {
    if(this->empty()) {
      return {};
    }
    std::vector<std::string> keys;
    keys.reserve(this->size());
    std::transform(this->begin(), this->end(), std::back_inserter(keys), [](const auto &pair) { return pair.first; });
    return keys;
  }

private:
  std::set<std::string> ns_;
};

} // namespace rush::ros

#endif // RUSH_ROS_PARAMETER_MANAGER_HPP
