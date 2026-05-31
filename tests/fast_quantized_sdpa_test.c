#include <math.h>
#include <stddef.h>
#include <stdlib.h>

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

static int run_affine_int4_decode_gqa4_head256(mlx_stream stream, int group_size_value) {
  mlx_optional_int group_size = {.value = group_size_value, .has_value = true};
  mlx_optional_int bits = {.value = 4, .has_value = true};
  int q_shape[] = {1, 16, 1, 256};
  int kv_shape[] = {1, 4, 128, 256};
  size_t q_count = 1 * 16 * 1 * 256;
  size_t kv_count = 1 * 4 * 128 * 256;
  float* queries_data = (float*)calloc(q_count, sizeof(float));
  float* keys_data = (float*)calloc(kv_count, sizeof(float));
  float* values_data = (float*)calloc(kv_count, sizeof(float));
  if (!queries_data || !keys_data || !values_data) {
    free(queries_data);
    free(keys_data);
    free(values_data);
    return 20;
  }
  for (size_t i = 0; i < kv_count; ++i) {
    values_data[i] = 0.125f;
  }

  mlx_array queries = mlx_array_new_data(queries_data, q_shape, 4, MLX_FLOAT32);
  mlx_array keys = mlx_array_new_data(keys_data, kv_shape, 4, MLX_FLOAT32);
  mlx_array values = mlx_array_new_data(values_data, kv_shape, 4, MLX_FLOAT32);
  mlx_vector_array quantized_keys = mlx_vector_array_new();
  mlx_vector_array quantized_values = mlx_vector_array_new();
  int status = 0;
  if (mlx_quantize(
          &quantized_keys, keys, group_size, bits, "affine", (mlx_array){NULL},
          stream) != 0 ||
      mlx_quantize(
          &quantized_values, values, group_size, bits, "affine",
          (mlx_array){NULL}, stream) != 0) {
    status = 21;
    goto cleanup_arrays;
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
    status = 22;
    goto cleanup_components;
  }

  mlx_array output = mlx_array_new();
  if (mlx_fast_quantized_scaled_dot_product_attention(
          &output, queries, key_weights, key_scales, key_biases, value_weights,
          value_scales, value_biases, 0.0625f, (mlx_array){NULL},
          (mlx_array){NULL}, group_size, bits, "affine", true, stream) != 0 ||
      mlx_array_eval(output) != 0) {
    status = 23;
    mlx_array_free(output);
    goto cleanup_components;
  }
  const int* shape = mlx_array_shape(output);
  if (!shape || shape[0] != 1 || shape[1] != 16 || shape[2] != 1 || shape[3] != 256) {
    status = 24;
  }
  mlx_array_free(output);

cleanup_components:
  mlx_array_free(value_biases);
  mlx_array_free(value_scales);
  mlx_array_free(value_weights);
  mlx_array_free(key_biases);
  mlx_array_free(key_scales);
  mlx_array_free(key_weights);
cleanup_arrays:
  mlx_vector_array_free(quantized_values);
  mlx_vector_array_free(quantized_keys);
  mlx_array_free(values);
  mlx_array_free(keys);
  mlx_array_free(queries);
  free(values_data);
  free(keys_data);
  free(queries_data);
  return status;
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

  int gqa32 = run_affine_int4_decode_gqa4_head256(stream, 32);
  if (gqa32 != 0) {
    return gqa32;
  }
  int gqa64 = run_affine_int4_decode_gqa4_head256(stream, 64);
  if (gqa64 != 0) {
    return gqa64;
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
