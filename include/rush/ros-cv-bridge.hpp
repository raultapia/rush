/**
 * @file ros-cv-bridge.hpp
 * @brief This library provides an easier-to-use ROS-OpenCV bridge.
 * @author Raul Tapia (raultapia.com)
 */
#ifndef RUSH_ROS_CV_BRIDGE_HPP
#define RUSH_ROS_CV_BRIDGE_HPP

#include <cv_bridge/cv_bridge.h>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/mat.inl.hpp>
#include <ros/publisher.h>
#include <ros/time.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Header.h>
#include <string>

namespace rush::roscv {

class Encoding {
public:
  Encoding() = delete;

  static inline std::string get(const cv::Mat &mat, const bool invert = true) {
    switch(mat.type()) {
    case CV_8UC1:
      return sensor_msgs::image_encodings::MONO8;
    case CV_8UC3:
      return invert ? sensor_msgs::image_encodings::BGR8 : sensor_msgs::image_encodings::RGB8;
    case CV_8UC4:
      return invert ? sensor_msgs::image_encodings::BGRA8 : sensor_msgs::image_encodings::RGBA8;
    case CV_16UC1:
      return sensor_msgs::image_encodings::MONO16;
    case CV_16UC3:
      return invert ? sensor_msgs::image_encodings::BGR16 : sensor_msgs::image_encodings::RGB16;
    case CV_16UC4:
      return invert ? sensor_msgs::image_encodings::BGRA16 : sensor_msgs::image_encodings::RGBA16;
    default:
      return {};
    }
  }
};

/**
 * @brief Converts an OpenCV Mat to a ROS Image message.
 * @param cv The input OpenCV Mat.
 * @param ros The output ROS Image message.
 * @param header The header for the ROS Image message.
 */
inline void cv2ros(const cv::Mat &cv, sensor_msgs::Image &ros, const std_msgs::Header &header = std_msgs::Header()) {
  cv_bridge::CvImage(header, Encoding::get(cv), cv).toImageMsg(ros);
}

/**
 * @brief Converts a ROS Image message to an OpenCV Mat.
 * @param ros The input ROS Image message.
 * @param cv The output OpenCV Mat.
 */
inline void ros2cv(const sensor_msgs::Image &ros, cv::Mat &cv) {
  const cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(ros, ros.encoding);
  (cv_ptr->image).copyTo(cv);
}

/**
 * @brief This class extends ros::Publisher to directly publish OpenCV matrices.
 */
class Publisher : public ros::Publisher {
  using ros::Publisher::Publisher;

public:
  Publisher &operator=(const ros::Publisher &x) {
    ros::Publisher::operator=(x);
    return *this;
  }

  /**
   * @brief Publishes an OpenCV Mat as a ROS Image message.
   * @param img The input OpenCV Mat to be published.
   * @param time The ROS time to be associated with the message.
   * @param frame_id The frame ID for the ROS message.
   */
  void publish(const cv::Mat &img, const ros::Time &time = ros::Time::now(), const std::string &frame_id = "") {
    std_msgs::Header header;
    header.stamp = time;
    header.frame_id = frame_id;
    sensor_msgs::Image msg;
    cv2ros(img, msg, header);
    ros::Publisher::publish(msg);
  }
};

} // namespace rush::roscv

#endif // RUSH_ROS_CV_BRIDGE_HPP
