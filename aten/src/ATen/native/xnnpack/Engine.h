#pragma once

#include <ATen/ATen.h>

namespace at {
namespace native {
namespace xnnpack {

//
// Clamp
//

bool use_clamp(
    const Tensor& input,
    float output_min,
    float output_max);

Tensor clamp(
    const Tensor& input,
    float output_min,
    float output_max);

bool use_clamp_(
    Tensor& input,
    float output_min,
    float output_max);

Tensor& clamp_(
    Tensor& input,
    float output_min,
    float output_max);

bool use_clamp_out(
    Tensor& output,
    const Tensor& input,
    float output_min,
    float output_max);

Tensor& clamp_out(
    Tensor& output,
    const Tensor& input,
    float output_min,
    float output_max);

template <typename Function, typename... Arguments>
inline bool use_clamp_optional(
    Function&& function,
    const optional<Scalar> output_min_,
    const optional<Scalar> output_max_,
    Arguments&&... arguments) {
  if (output_min_ && !output_min_->isFloatingPoint()) {
    return false;
  }

  if (output_max_ && !output_max_->isFloatingPoint()) {
    return false;
  }

  const float output_min = output_min_ ?
      output_min_->to<float>() :
      -std::numeric_limits<float>::infinity();

  const float output_max = output_max_ ?
      output_max_->to<float>() :
      std::numeric_limits<float>::infinity();

  return function(
      std::forward<Arguments>(arguments)...,
      output_min,
      output_max);
}

template <typename Function, typename... Arguments>
inline auto clamp_optional(
    Function&& function,
    const optional<Scalar> output_min_,
    const optional<Scalar> output_max_,
    Arguments&&... arguments) -> decltype(auto) {
  const float output_min = output_min_ ?
      output_min_->to<float>() :
      -std::numeric_limits<float>::infinity();

  const float output_max = output_max_ ?
      output_max_->to<float>() :
      std::numeric_limits<float>::infinity();

  return function(
      std::forward<Arguments>(arguments)...,
      output_min,
      output_max);
}

//
// Convolution
//

bool use_convolution2d(
    const Tensor& input,
    const Tensor& weight,
    const Tensor& bias,
    const IntArrayRef padding,
    const IntArrayRef stride,
    const IntArrayRef dilation,
    const int64_t groups);

Tensor convolution2d(
    const Tensor& input,
    const Tensor& weight,
    const Tensor& bias,
    const IntArrayRef padding,
    const IntArrayRef stride,
    const IntArrayRef dilation,
    const int64_t groups);

//
// Linear
//

bool use_linear(
  const Tensor& input,
  const Tensor& weight,
  const Tensor& bias);

Tensor linear(
  const Tensor& input,
  const Tensor& weight,
  const Tensor& bias);

//
// Max Pooling
//

bool use_max_pool2d(
    const Tensor& input,
    IntArrayRef kernel,
    IntArrayRef padding,
    IntArrayRef stride,
    IntArrayRef dilation,
    bool ceil_mode,
    float output_min = -std::numeric_limits<float>::infinity(),
    float output_max = +std::numeric_limits<float>::infinity());

Tensor max_pool2d(
    const Tensor& input,
    IntArrayRef kernel,
    IntArrayRef padding,
    IntArrayRef stride,
    IntArrayRef dilation,
    bool ceil_mode,
    float output_min = -std::numeric_limits<float>::infinity(),
    float output_max = +std::numeric_limits<float>::infinity());

//
// ReLU
//

inline bool use_relu(const Tensor& input) {
  return use_clamp(
      input,
      0.0f,
      std::numeric_limits<float>::infinity());
}

inline Tensor relu(const Tensor& input) {
  return clamp(
      input,
      0.0f,
      std::numeric_limits<float>::infinity());
}

inline bool use_relu_(Tensor& input) {
  return use_clamp_(
      input,
      0.0f,
      std::numeric_limits<float>::infinity());
}

inline Tensor& relu_(Tensor& input) {
  return clamp_(
      input,
      0.0f,
      std::numeric_limits<float>::infinity());
}

} // namespace xnnpack
} // namespace native
} // namespace at
