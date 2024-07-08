/**
 * @file cv-highgui.hpp
 * @brief This library extends OpenCV's HighGUI functionalities.
 * @author Raul Tapia (raultapia.com)
 * @copyright GNU General Public License v3.0
 * @see https://github.com/raultapia/rush
 */
#ifndef RUSH_CV_HIGHGUI_HPP
#define RUSH_CV_HIGHGUI_HPP

#include <algorithm>
#include <cstddef>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/mat.inl.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

namespace rush::cv {

/**
 * @brief Create a montage from a matrix of images.
 *
 * This function takes a vector of vectors of images and creates a montage by resizing and concatenating them.
 *
 * @param images Vector of vectors of images.
 * @return A single montage image.
 */
inline ::cv::Mat montage(std::vector<std::vector<::cv::Mat>> &images) {
  ::cv::Mat montage;
  ::cv::Size ref{images[0][0].size()};
  std::size_t max_col = 0;

  for(const std::vector<::cv::Mat> &v : images) {
    for(const ::cv::Mat &i : v) {
      if(i.size().area() < ref.area()) {
        ref = i.size();
      }
      max_col = std::max(max_col, v.size());
    }
  }

  std::vector<::cv::Mat> rows;
  for(std::vector<::cv::Mat> &v : images) {
    for(::cv::Mat &i : v) {
      ::cv::resize(i, i, ref);
    }
    while(v.size() < max_col) {
      v.emplace_back(ref, images[0][0].type(), ::cv::Scalar(0));
    }
    rows.emplace_back(v.size() * ref.width, ref.height, images[0][0].type());
    ::cv::hconcat(v.data(), v.size(), rows.back());
  }
  ::cv::vconcat(rows.data(), rows.size(), montage);
  return montage;
}

/**
 * @brief Create a montage from a vector of images with a specified step.
 *
 * This function takes a vector of images and creates a montage with a specified step by calling the main montage function.
 *
 * @param images Vector of images.
 * @param step Step size for selecting images.
 * @return A single montage image.
 * @see montage(std::vector<std::vector<::cv::Mat>> &images)
 */
inline ::cv::Mat montage(std::vector<::cv::Mat> &images, std::size_t step = 0) {
  std::vector<std::vector<::cv::Mat>> img_vector;
  if(!static_cast<bool>(step)) {
    step = images.size();
  }
  for(std::size_t i = 0; i < images.size(); i += step) {
    img_vector.emplace_back(images.begin() + static_cast<long>(i), std::min(images.begin() + static_cast<long>(i + step), images.end()));
  }
  return montage(img_vector);
}

} // namespace rush::cv

#endif // RUSH_CV_HIGHGUI_HPP
