#include <math.h>
#include <stddef.h>

#include "mlx/c/mlx.h"

static int get_quantized_component(
    mlx_array* out,
    const mlx_vector_array quantized,
    size_t index) {
  if (mlx_vector_array_get(out, quantized, index) != 0) {
    return 1;
  }
  if (out->ctx == NULL) {
    return 2;
  }
  return 0;
}

int main(void) {
  mlx_stream stream = mlx_default_cpu_stream_new();
  mlx_optional_int group_size = {.value = 64, .has_value = true};
  mlx_optional_int bits = {.value = 4, .has_value = true};

  int shape[] = {1, 1, 2, 64};
  float queries_data[128] = {0.0f};
  float keys_data[128] = {0.0f};
  float values_data[128];
  for (int i = 0; i < 64; ++i) {
    values_data[i] = 1.0f;
    values_data[64 + i] = 9.0f;
  }

  mlx_array queries = mlx_array_new_data(queries_data, shape, 4, MLX_FLOAT32);
  mlx_array keys = mlx_array_new_data(keys_data, shape, 4, MLX_FLOAT32);
  mlx_array values = mlx_array_new_data(values_data, shape, 4, MLX_FLOAT32);

  mlx_vector_array quantized_keys = mlx_vector_array_new();
  mlx_vector_array quantized_values = mlx_vector_array_new();
  if (mlx_quantize(
          &quantized_keys, keys, group_size, bits, "affine", (mlx_array){NULL},
          stream) != 0) {
    return 1;
  }
  if (mlx_quantize(
          &quantized_values, values, group_size, bits, "affine",
          (mlx_array){NULL}, stream) != 0) {
    return 2;
  }

  mlx_array key_weights = mlx_array_new();
  mlx_array key_scales = mlx_array_new();
  mlx_array key_biases = mlx_array_new();
  mlx_array value_weights = mlx_array_new();
  mlx_array value_scales = mlx_array_new();
  mlx_array value_biases = mlx_array_new();
  if (get_quantized_component(&key_weights, quantized_keys, 0) != 0 ||
      get_quantized_component(&key_scales, quantized_keys, 1) != 0 ||
      get_quantized_component(&key_biases, quantized_keys, 2) != 0 ||
      get_quantized_component(&value_weights, quantized_values, 0) != 0 ||
      get_quantized_component(&value_scales, quantized_values, 1) != 0 ||
      get_quantized_component(&value_biases, quantized_values, 2) != 0) {
    return 3;
  }

  mlx_array output = mlx_array_new();
  if (mlx_fast_quantized_scaled_dot_product_attention(
          &output, queries, key_weights, key_scales, key_biases, value_weights,
          value_scales, value_biases, 1.0f, (mlx_array){NULL},
          (mlx_array){NULL}, group_size, bits, "affine", true, stream) != 0) {
    return 4;
  }
  if (mlx_array_eval(output) != 0) {
    return 5;
  }

  const float* output_data = mlx_array_data_float32(output);
  if (!output_data) {
    return 6;
  }
  for (int i = 0; i < 64; ++i) {
    if (fabsf(output_data[i] - 1.0f) > 1e-5f) {
      return 7;
    }
    if (fabsf(output_data[64 + i] - 5.0f) > 1e-5f) {
      return 8;
    }
  }

  mlx_array_free(output);
  mlx_array_free(value_biases);
  mlx_array_free(value_scales);
  mlx_array_free(value_weights);
  mlx_array_free(key_biases);
  mlx_array_free(key_scales);
  mlx_array_free(key_weights);
  mlx_vector_array_free(quantized_values);
  mlx_vector_array_free(quantized_keys);
  mlx_array_free(values);
  mlx_array_free(keys);
  mlx_array_free(queries);
  mlx_stream_free(stream);
  return 0;
}
