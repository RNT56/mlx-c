/* Copyright © 2023-2024 Apple Inc.                   */
/*                                                    */
/* This file is auto-generated. Do not edit manually. */
/*                                                    */

#include "mlx/c/fast.h"
#include "mlx/c/error.h"
#include "mlx/c/private/mlx.h"
#include "mlx/array.h"
#include "mlx/fast.h"

#include <atomic>
#include <chrono>
#include <cstring>
#include <fstream>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <json.hpp>

#if !defined(_WIN32)
#include <fcntl.h>
#include <unistd.h>
#endif

namespace mlx::core {
void prefault(const array& a) {
  if (!a.data_shared_ptr()) {
    return;
  }

  const auto* ptr = static_cast<const uint8_t*>(
      const_cast<allocator::Buffer&>(a.buffer()).raw_ptr());
  if (!ptr) {
    return;
  }

  volatile uint8_t tmp = 0;
  size_t size = a.buffer_size();
  for (size_t i = 0; i < size; i += 16384) {
    tmp = static_cast<uint8_t>(tmp + ptr[i]);
  }

  if (size > 0) {
    tmp = static_cast<uint8_t>(tmp + ptr[size - 1]);
  }
}
} // namespace mlx::core

struct mlx_fast_cuda_kernel_config_cpp_ {
  std::vector<mlx::core::Shape> output_shapes;
  std::vector<mlx::core::Dtype> output_dtypes;
  std::tuple<int, int, int> grid;
  std::tuple<int, int, int> thread_group;
  std::vector<std::pair<std::string, mlx::core::fast::TemplateArg>>
      template_args;
  std::optional<float> init_value;
  bool verbose;
};

inline mlx_fast_cuda_kernel_config mlx_fast_cuda_kernel_config_new_() {
  return mlx_fast_cuda_kernel_config({new mlx_fast_cuda_kernel_config_cpp_()});
}

inline mlx_fast_cuda_kernel_config_cpp_& mlx_fast_cuda_kernel_config_get_(
    mlx_fast_cuda_kernel_config d) {
  if (!d.ctx) {
    throw std::runtime_error(
        "expected a non-empty mlx_fast_cuda_kernel_config");
  }
  return *static_cast<mlx_fast_cuda_kernel_config_cpp_*>(d.ctx);
}

inline void mlx_fast_cuda_kernel_config_free_(mlx_fast_cuda_kernel_config d) {
  if (d.ctx) {
    delete static_cast<mlx_fast_cuda_kernel_config_cpp_*>(d.ctx);
  }
}

extern "C" mlx_fast_cuda_kernel_config mlx_fast_cuda_kernel_config_new(void) {
  try {
    return mlx_fast_cuda_kernel_config_new_();
  } catch (std::exception& e) {
    mlx_error(e.what());
  }
  return {nullptr};
}

extern "C" void mlx_fast_cuda_kernel_config_free(
    mlx_fast_cuda_kernel_config cls) {
  mlx_fast_cuda_kernel_config_free_(cls);
}

extern "C" int mlx_fast_cuda_kernel_config_add_output_arg(
    mlx_fast_cuda_kernel_config cls,
    const int* shape,
    size_t size,
    mlx_dtype dtype) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).output_shapes.push_back(
        mlx::core::Shape(shape, shape + size));
    mlx_fast_cuda_kernel_config_get_(cls).output_dtypes.push_back(
        mlx_dtype_to_cpp(dtype));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_set_grid(
    mlx_fast_cuda_kernel_config cls,
    int grid1,
    int grid2,
    int grid3) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).grid =
        std::make_tuple(grid1, grid2, grid3);
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_set_thread_group(
    mlx_fast_cuda_kernel_config cls,
    int thread1,
    int thread2,
    int thread3) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).thread_group =
        std::make_tuple(thread1, thread2, thread3);
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_set_init_value(
    mlx_fast_cuda_kernel_config cls,
    float value) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).init_value = value;
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_set_verbose(
    mlx_fast_cuda_kernel_config cls,
    bool verbose) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).verbose = verbose;
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_add_template_arg_dtype(
    mlx_fast_cuda_kernel_config cls,
    const char* name,
    mlx_dtype dtype) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), mlx_dtype_to_cpp(dtype)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_add_template_arg_int(
    mlx_fast_cuda_kernel_config cls,
    const char* name,
    int value) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), value));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_add_template_arg_uint32(
    mlx_fast_cuda_kernel_config cls,
    const char* name,
    uint32_t value) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), value));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_cuda_kernel_config_add_template_arg_bool(
    mlx_fast_cuda_kernel_config cls,
    const char* name,
    bool value) {
  try {
    mlx_fast_cuda_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), value));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

struct mlx_fast_cuda_kernel_cpp_ {
  mlx::core::fast::CustomKernelFunction mkf;
  mlx_fast_cuda_kernel_cpp_(mlx::core::fast::CustomKernelFunction mkf)
      : mkf(mkf) {};
};

inline mlx_fast_cuda_kernel mlx_fast_cuda_kernel_new_(
    const std::string& name,
    const std::vector<std::string>& input_names,
    const std::vector<std::string>& output_names,
    const std::string& source,
    const std::string& header,
    bool ensure_row_contiguous,
    int shared_memory) {
  return mlx_fast_cuda_kernel({new mlx_fast_cuda_kernel_cpp_(
      mlx::core::fast::cuda_kernel(
          name,
          input_names,
          output_names,
          source,
          header,
          ensure_row_contiguous,
          shared_memory))});
}

extern "C" mlx_fast_cuda_kernel mlx_fast_cuda_kernel_new(
    const char* name,
    const mlx_vector_string input_names,
    const mlx_vector_string output_names,
    const char* source,
    const char* header,
    bool ensure_row_contiguous,
    int shared_memory) {
  try {
    return mlx_fast_cuda_kernel_new_(
        name,
        mlx_vector_string_get_(input_names),
        mlx_vector_string_get_(output_names),
        source,
        header,
        ensure_row_contiguous,
        shared_memory);
  } catch (std::exception& e) {
    mlx_error(e.what());
  }
  return {nullptr};
}

inline mlx::core::fast::CustomKernelFunction& mlx_fast_cuda_kernel_get_(
    mlx_fast_cuda_kernel d) {
  if (!d.ctx) {
    throw std::runtime_error("expected a non-empty mlx_fast_cuda_kernel");
  }
  return static_cast<mlx_fast_cuda_kernel_cpp_*>(d.ctx)->mkf;
}

inline void mlx_fast_cuda_kernel_free_(mlx_fast_cuda_kernel d) {
  if (d.ctx) {
    delete static_cast<mlx_fast_cuda_kernel_cpp_*>(d.ctx);
  }
}

extern "C" void mlx_fast_cuda_kernel_free(mlx_fast_cuda_kernel cls) {
  mlx_fast_cuda_kernel_free_(cls);
}

extern "C" int mlx_fast_cuda_kernel_apply(
    mlx_vector_array* outputs,
    mlx_fast_cuda_kernel cls,
    const mlx_vector_array inputs,
    const mlx_fast_cuda_kernel_config config,
    const mlx_stream stream) {
  try {
    auto config_ctx = mlx_fast_cuda_kernel_config_get_(config);
    mlx_vector_array_set_(
        *outputs,
        mlx_fast_cuda_kernel_get_(cls)(
            mlx_vector_array_get_(inputs),
            config_ctx.output_shapes,
            config_ctx.output_dtypes,
            config_ctx.grid,
            config_ctx.thread_group,
            config_ctx.template_args,
            config_ctx.init_value,
            config_ctx.verbose,
            mlx_stream_get_(stream)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

extern "C" int mlx_fast_layer_norm(
    mlx_array* res,
    const mlx_array x,
    const mlx_array weight /* may be null */,
    const mlx_array bias /* may be null */,
    float eps,
    const mlx_stream s) {
  try {
    mlx_array_set_(
        *res,
        mlx::core::fast::layer_norm(
            mlx_array_get_(x),
            (weight.ctx ? std::make_optional(mlx_array_get_(weight))
                        : std::nullopt),
            (bias.ctx ? std::make_optional(mlx_array_get_(bias))
                      : std::nullopt),
            eps,
            mlx_stream_get_(s)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

struct mlx_fast_metal_kernel_config_cpp_ {
  std::vector<mlx::core::Shape> output_shapes;
  std::vector<mlx::core::Dtype> output_dtypes;
  std::tuple<int, int, int> grid;
  std::tuple<int, int, int> thread_group;
  std::vector<std::pair<std::string, mlx::core::fast::TemplateArg>>
      template_args;
  std::optional<float> init_value;
  bool verbose;
};

inline mlx_fast_metal_kernel_config mlx_fast_metal_kernel_config_new_() {
  return mlx_fast_metal_kernel_config(
      {new mlx_fast_metal_kernel_config_cpp_()});
}

inline mlx_fast_metal_kernel_config_cpp_& mlx_fast_metal_kernel_config_get_(
    mlx_fast_metal_kernel_config d) {
  if (!d.ctx) {
    throw std::runtime_error(
        "expected a non-empty mlx_fast_metal_kernel_config");
  }
  return *static_cast<mlx_fast_metal_kernel_config_cpp_*>(d.ctx);
}

inline void mlx_fast_metal_kernel_config_free_(mlx_fast_metal_kernel_config d) {
  if (d.ctx) {
    delete static_cast<mlx_fast_metal_kernel_config_cpp_*>(d.ctx);
  }
}

extern "C" mlx_fast_metal_kernel_config mlx_fast_metal_kernel_config_new(void) {
  try {
    return mlx_fast_metal_kernel_config_new_();
  } catch (std::exception& e) {
    mlx_error(e.what());
  }
  return {nullptr};
}

extern "C" void mlx_fast_metal_kernel_config_free(
    mlx_fast_metal_kernel_config cls) {
  mlx_fast_metal_kernel_config_free_(cls);
}

extern "C" int mlx_fast_metal_kernel_config_add_output_arg(
    mlx_fast_metal_kernel_config cls,
    const int* shape,
    size_t size,
    mlx_dtype dtype) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).output_shapes.push_back(
        mlx::core::Shape(shape, shape + size));
    mlx_fast_metal_kernel_config_get_(cls).output_dtypes.push_back(
        mlx_dtype_to_cpp(dtype));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_set_grid(
    mlx_fast_metal_kernel_config cls,
    int grid1,
    int grid2,
    int grid3) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).grid =
        std::make_tuple(grid1, grid2, grid3);
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_set_thread_group(
    mlx_fast_metal_kernel_config cls,
    int thread1,
    int thread2,
    int thread3) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).thread_group =
        std::make_tuple(thread1, thread2, thread3);
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_set_init_value(
    mlx_fast_metal_kernel_config cls,
    float value) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).init_value = value;
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_set_verbose(
    mlx_fast_metal_kernel_config cls,
    bool verbose) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).verbose = verbose;
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_add_template_arg_dtype(
    mlx_fast_metal_kernel_config cls,
    const char* name,
    mlx_dtype dtype) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), mlx_dtype_to_cpp(dtype)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_add_template_arg_int(
    mlx_fast_metal_kernel_config cls,
    const char* name,
    int value) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), value));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_add_template_arg_uint32(
    mlx_fast_metal_kernel_config cls,
    const char* name,
    uint32_t value) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), value));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_metal_kernel_config_add_template_arg_bool(
    mlx_fast_metal_kernel_config cls,
    const char* name,
    bool value) {
  try {
    mlx_fast_metal_kernel_config_get_(cls).template_args.push_back(
        std::make_pair(std::string(name), value));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

struct mlx_fast_metal_kernel_cpp_ {
  mlx::core::fast::CustomKernelFunction mkf;
  mlx_fast_metal_kernel_cpp_(mlx::core::fast::CustomKernelFunction mkf)
      : mkf(mkf) {};
};

inline mlx_fast_metal_kernel mlx_fast_metal_kernel_new_(
    const std::string& name,
    const std::vector<std::string>& input_names,
    const std::vector<std::string>& output_names,
    const std::string& source,
    const std::string& header,
    bool ensure_row_contiguous,
    bool atomic_outputs) {
  return mlx_fast_metal_kernel({new mlx_fast_metal_kernel_cpp_(
      mlx::core::fast::metal_kernel(
          name,
          input_names,
          output_names,
          source,
          header,
          ensure_row_contiguous,
          atomic_outputs))});
}

extern "C" mlx_fast_metal_kernel mlx_fast_metal_kernel_new(
    const char* name,
    const mlx_vector_string input_names,
    const mlx_vector_string output_names,
    const char* source,
    const char* header,
    bool ensure_row_contiguous,
    bool atomic_outputs) {
  try {
    return mlx_fast_metal_kernel_new_(
        name,
        mlx_vector_string_get_(input_names),
        mlx_vector_string_get_(output_names),
        source,
        header,
        ensure_row_contiguous,
        atomic_outputs);
  } catch (std::exception& e) {
    mlx_error(e.what());
  }
  return {nullptr};
}

inline mlx::core::fast::CustomKernelFunction& mlx_fast_metal_kernel_get_(
    mlx_fast_metal_kernel d) {
  if (!d.ctx) {
    throw std::runtime_error("expected a non-empty mlx_fast_metal_kernel");
  }
  return static_cast<mlx_fast_metal_kernel_cpp_*>(d.ctx)->mkf;
}

inline void mlx_fast_metal_kernel_free_(mlx_fast_metal_kernel d) {
  if (d.ctx) {
    delete static_cast<mlx_fast_metal_kernel_cpp_*>(d.ctx);
  }
}

extern "C" void mlx_fast_metal_kernel_free(mlx_fast_metal_kernel cls) {
  mlx_fast_metal_kernel_free_(cls);
}

extern "C" int mlx_fast_metal_kernel_apply(
    mlx_vector_array* outputs,
    mlx_fast_metal_kernel cls,
    const mlx_vector_array inputs,
    const mlx_fast_metal_kernel_config config,
    const mlx_stream stream) {
  try {
    auto config_ctx = mlx_fast_metal_kernel_config_get_(config);
    mlx_vector_array_set_(
        *outputs,
        mlx_fast_metal_kernel_get_(cls)(
            mlx_vector_array_get_(inputs),
            config_ctx.output_shapes,
            config_ctx.output_dtypes,
            config_ctx.grid,
            config_ctx.thread_group,
            config_ctx.template_args,
            config_ctx.init_value,
            config_ctx.verbose,
            mlx_stream_get_(stream)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

extern "C" int mlx_fast_rms_norm(
    mlx_array* res,
    const mlx_array x,
    const mlx_array weight /* may be null */,
    float eps,
    const mlx_stream s) {
  try {
    mlx_array_set_(
        *res,
        mlx::core::fast::rms_norm(
            mlx_array_get_(x),
            (weight.ctx ? std::make_optional(mlx_array_get_(weight))
                        : std::nullopt),
            eps,
            mlx_stream_get_(s)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_rope(
    mlx_array* res,
    const mlx_array x,
    int dims,
    bool traditional,
    mlx_optional_float base,
    float scale,
    int offset,
    const mlx_array freqs /* may be null */,
    const mlx_stream s) {
  try {
    mlx_array_set_(
        *res,
        mlx::core::fast::rope(
            mlx_array_get_(x),
            dims,
            traditional,
            (base.has_value ? std::make_optional<float>(base.value)
                            : std::nullopt),
            scale,
            offset,
            (freqs.ctx ? std::make_optional(mlx_array_get_(freqs))
                       : std::nullopt),
            mlx_stream_get_(s)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_rope_dynamic(
    mlx_array* res,
    const mlx_array x,
    int dims,
    bool traditional,
    mlx_optional_float base,
    float scale,
    const mlx_array offset,
    const mlx_array freqs /* may be null */,
    const mlx_stream s) {
  try {
    mlx_array_set_(
        *res,
        mlx::core::fast::rope(
            mlx_array_get_(x),
            dims,
            traditional,
            (base.has_value ? std::make_optional<float>(base.value)
                            : std::nullopt),
            scale,
            mlx_array_get_(offset),
            (freqs.ctx ? std::make_optional(mlx_array_get_(freqs))
                       : std::nullopt),
            mlx_stream_get_(s)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_scaled_dot_product_attention(
    mlx_array* res,
    const mlx_array queries,
    const mlx_array keys,
    const mlx_array values,
    float scale,
    const char* mask_mode,
    const mlx_array mask_arr /* may be null */,
    const mlx_array sinks /* may be null */,
    const mlx_stream s) {
  try {
    mlx_array_set_(
        *res,
        mlx::core::fast::scaled_dot_product_attention(
            mlx_array_get_(queries),
            mlx_array_get_(keys),
            mlx_array_get_(values),
            scale,
            std::string(mask_mode),
            (mask_arr.ctx ? std::make_optional(mlx_array_get_(mask_arr))
                          : std::nullopt),
            (sinks.ctx ? std::make_optional(mlx_array_get_(sinks))
                       : std::nullopt),
            mlx_stream_get_(s)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}
extern "C" int mlx_fast_quantized_scaled_dot_product_attention(
    mlx_array* res,
    const mlx_array queries,
    const mlx_array keys,
    const mlx_array key_scales,
    const mlx_array key_biases /* may be null */,
    const mlx_array values,
    const mlx_array value_scales,
    const mlx_array value_biases /* may be null */,
    float scale,
    const mlx_array mask /* may be null */,
    const mlx_array sinks /* may be null */,
    mlx_optional_int group_size,
    mlx_optional_int bits,
    const char* mode,
    bool causal,
    const mlx_stream s) {
  try {
    mlx_array_set_(
        *res,
        mlx::core::fast::quantized_scaled_dot_product_attention(
            mlx_array_get_(queries),
            mlx_array_get_(keys),
            mlx_array_get_(key_scales),
            (key_biases.ctx ? std::make_optional(mlx_array_get_(key_biases))
                            : std::nullopt),
            mlx_array_get_(values),
            mlx_array_get_(value_scales),
            (value_biases.ctx ? std::make_optional(mlx_array_get_(value_biases))
                              : std::nullopt),
            scale,
            (mask.ctx ? std::make_optional(mlx_array_get_(mask))
                      : std::nullopt),
            (sinks.ctx ? std::make_optional(mlx_array_get_(sinks))
                       : std::nullopt),
            (group_size.has_value ? std::make_optional<int>(group_size.value)
                                  : std::nullopt),
            (bits.has_value ? std::make_optional<int>(bits.value)
                            : std::nullopt),
            std::string(mode),
            causal,
            mlx_stream_get_(s)));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

namespace {

using Clock = std::chrono::steady_clock;

struct SSDMetricsState {
  std::atomic<uint64_t> total_bytes_read{0};
  std::atomic<uint64_t> total_chunks{0};
  std::atomic<uint64_t> total_latency_ns{0};
  std::mutex window_mutex;
  Clock::time_point window_start = Clock::now();
  uint64_t window_bytes = 0;
  double throughput_mb_per_s = 0.0;
};

SSDMetricsState& ssd_metrics_state() {
  static SSDMetricsState state;
  return state;
}

void record_ssd_read(size_t bytes, Clock::duration latency) {
  auto& state = ssd_metrics_state();
  state.total_bytes_read.fetch_add(bytes, std::memory_order_relaxed);
  state.total_chunks.fetch_add(1, std::memory_order_relaxed);
  state.total_latency_ns.fetch_add(
      static_cast<uint64_t>(
          std::chrono::duration_cast<std::chrono::nanoseconds>(latency)
              .count()),
      std::memory_order_relaxed);

  std::lock_guard<std::mutex> lock(state.window_mutex);
  state.window_bytes += bytes;
  auto now = Clock::now();
  auto elapsed = now - state.window_start;
  if (elapsed >= std::chrono::seconds(10)) {
    double seconds = std::chrono::duration<double>(elapsed).count();
    state.throughput_mb_per_s =
        seconds > 0.0
            ? (static_cast<double>(state.window_bytes) / (1024.0 * 1024.0)) /
                  seconds
            : 0.0;
    state.window_bytes = 0;
    state.window_start = now;
  }
}

struct SafetensorsPReadEntry {
  int fd = -1;
  size_t data_start = 0;
  size_t bytes_per_expert = 0;
  size_t expert_count = 0;
};

std::mutex safetensors_pread_cache_mutex;
std::unordered_map<std::string, SafetensorsPReadEntry> safetensors_pread_cache;

SafetensorsPReadEntry get_safetensors_pread_entry(
    const std::string& path,
    const std::string& tensor_name) {
#if defined(_WIN32)
  throw std::runtime_error("safetensors pread is unsupported on this platform");
#else
  std::string key = path + "|" + tensor_name;
  std::lock_guard<std::mutex> lock(safetensors_pread_cache_mutex);
  auto it = safetensors_pread_cache.find(key);
  if (it != safetensors_pread_cache.end()) {
    return it->second;
  }

  int fd = open(path.c_str(), O_RDONLY);
  if (fd < 0) {
    throw std::runtime_error("[pread_into] cannot open: " + path);
  }

  auto close_on_error = [&fd]() {
    if (fd >= 0) {
      close(fd);
      fd = -1;
    }
  };

  uint64_t header_length = 0;
  if (pread(fd, &header_length, sizeof(header_length), 0) !=
      static_cast<ssize_t>(sizeof(header_length))) {
    close_on_error();
    throw std::runtime_error("[pread_into] cannot read safetensors header length");
  }

  std::vector<char> header(header_length);
  if (static_cast<uint64_t>(pread(fd, header.data(), header.size(), 8)) !=
      header_length) {
    close_on_error();
    throw std::runtime_error("[pread_into] cannot read safetensors header JSON");
  }

  auto json = nlohmann::json::parse(header.begin(), header.end());
  auto tensor = json.find(tensor_name);
  if (tensor == json.end()) {
    close_on_error();
    throw std::runtime_error("[pread_into] tensor not found: " + tensor_name);
  }

  auto shape = tensor->at("shape").get<std::vector<size_t>>();
  auto offsets = tensor->at("data_offsets").get<std::vector<size_t>>();
  if (shape.empty()) {
    close_on_error();
    throw std::runtime_error("[pread_into] tensor has no expert dimension: " +
                             tensor_name);
  }
  if (offsets.size() != 2 || offsets[1] < offsets[0]) {
    close_on_error();
    throw std::runtime_error("[pread_into] invalid data_offsets for tensor: " +
                             tensor_name);
  }

  size_t expert_count = shape[0];
  size_t total_bytes = offsets[1] - offsets[0];
  if (expert_count == 0 || total_bytes % expert_count != 0) {
    close_on_error();
    throw std::runtime_error("[pread_into] tensor bytes are not evenly split by experts: " +
                             tensor_name);
  }

  SafetensorsPReadEntry entry{
      fd,
      static_cast<size_t>(8 + header_length + offsets[0]),
      total_bytes / expert_count,
      expert_count};
  safetensors_pread_cache[key] = entry;
  return entry;
#endif
}

void pread_exact(int fd, void* dst, size_t length, size_t offset) {
#if defined(_WIN32)
  throw std::runtime_error("pread is unsupported on this platform");
#else
  if (length > static_cast<size_t>(std::numeric_limits<ssize_t>::max())) {
    throw std::runtime_error("[pread_into] read length exceeds ssize_t max");
  }
  ssize_t result = pread(fd, dst, length, static_cast<off_t>(offset));
  if (result < 0 || static_cast<size_t>(result) != length) {
    throw std::runtime_error(
        "[pread_into] pread failed: got " + std::to_string(result) + " of " +
        std::to_string(length));
  }
#endif
}

} // namespace

extern "C" int mlx_fast_prefault(mlx_array x) {
  try {
    mlx::core::prefault(mlx_array_get_(x));
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

extern "C" int mlx_fast_pread_into(
    mlx_array dst,
    const char* safetensors_path,
    const char* tensor_name,
    uint32_t expert_index) {
  try {
    if (!safetensors_path || !tensor_name) {
      throw std::runtime_error("[pread_into] safetensors path and tensor name are required");
    }

    auto entry = get_safetensors_pread_entry(safetensors_path, tensor_name);
    if (expert_index >= entry.expert_count) {
      throw std::runtime_error("[pread_into] expert index out of bounds");
    }

    auto& arr = mlx_array_get_(dst);
    void* buffer = arr.data<uint8_t>();
    if (!buffer) {
      throw std::runtime_error("[pread_into] dst has no data pointer; evaluate it first");
    }

    size_t dst_bytes = arr.nbytes();
    if (dst_bytes != entry.bytes_per_expert) {
      throw std::runtime_error(
          "[pread_into] dst.nbytes (" + std::to_string(dst_bytes) +
          ") must equal bytes_per_expert (" +
          std::to_string(entry.bytes_per_expert) + ")");
    }

    size_t file_offset =
        entry.data_start + static_cast<size_t>(expert_index) * entry.bytes_per_expert;
    auto start = Clock::now();
    pread_exact(entry.fd, buffer, dst_bytes, file_offset);
    record_ssd_read(dst_bytes, Clock::now() - start);
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

extern "C" int mlx_fast_pread_into_offset(
    mlx_array dst,
    const char* safetensors_path,
    const char* tensor_name,
    uint32_t expert_index,
    size_t dst_offset) {
  try {
    if (!safetensors_path || !tensor_name) {
      throw std::runtime_error(
          "[pread_into_offset] safetensors path and tensor name are required");
    }

    auto entry = get_safetensors_pread_entry(safetensors_path, tensor_name);
    if (expert_index >= entry.expert_count) {
      throw std::runtime_error("[pread_into_offset] expert index out of bounds");
    }

    auto& arr = mlx_array_get_(dst);
    auto* base = arr.data<uint8_t>();
    if (!base) {
      throw std::runtime_error(
          "[pread_into_offset] dst has no data pointer; evaluate it first");
    }

    size_t dst_bytes = arr.nbytes();
    if (dst_offset > dst_bytes ||
        entry.bytes_per_expert > dst_bytes - dst_offset) {
      throw std::runtime_error(
          "[pread_into_offset] dst_offset (" + std::to_string(dst_offset) +
          ") + bytes_per_expert (" +
          std::to_string(entry.bytes_per_expert) + ") exceeds dst.nbytes (" +
          std::to_string(dst_bytes) + ")");
    }

    size_t file_offset =
        entry.data_start + static_cast<size_t>(expert_index) * entry.bytes_per_expert;
    auto start = Clock::now();
    pread_exact(
        entry.fd,
        static_cast<void*>(base + dst_offset),
        entry.bytes_per_expert,
        file_offset);
    record_ssd_read(entry.bytes_per_expert, Clock::now() - start);
  } catch (std::exception& e) {
    mlx_error(e.what());
    return 1;
  }
  return 0;
}

extern "C" void mlx_ssd_metrics_snapshot(MlxSSDMetricsSnapshot* out) {
  if (!out) {
    return;
  }

  auto& state = ssd_metrics_state();
  uint64_t chunks = state.total_chunks.load(std::memory_order_relaxed);
  uint64_t latency_ns = state.total_latency_ns.load(std::memory_order_relaxed);
  double throughput = 0.0;
  {
    std::lock_guard<std::mutex> lock(state.window_mutex);
    throughput = state.throughput_mb_per_s;
  }

  out->throughput_mb_per_s = throughput;
  out->total_bytes_read = state.total_bytes_read.load(std::memory_order_relaxed);
  out->total_chunks = chunks;
  out->avg_chunk_latency_ms =
      chunks > 0 ? (static_cast<double>(latency_ns) / 1'000'000.0) /
                       static_cast<double>(chunks)
                 : 0.0;
}
