#include "mlx/c/mlx.h"

static void ignore_mlx_error(const char* msg, void* data) {
  (void)msg;
  (void)data;
}

int main(void) {
  mlx_set_error_handler(ignore_mlx_error, NULL, NULL);
  mlx_stream stream = mlx_default_cpu_stream_new();

  int q_shape[] = {1, 4, 1, 64};
  float q_data[256] = {0.0f};
  mlx_array queries = mlx_array_new_data(q_data, q_shape, 4, MLX_FLOAT32);

  int packed_shape[] = {1, 1, 1, 1, 8};
  uint32_t packed_data[8] = {0};
  mlx_array key_packed =
      mlx_array_new_data(packed_data, packed_shape, 5, MLX_UINT32);
  mlx_array value_packed =
      mlx_array_new_data(packed_data, packed_shape, 5, MLX_UINT32);

  int bitset_shape[] = {1, 1, 1, 1, 2};
  uint32_t bitset_data[2] = {0};
  mlx_array key_signs =
      mlx_array_new_data(bitset_data, bitset_shape, 5, MLX_UINT32);
  mlx_array key_high_mask =
      mlx_array_new_data(bitset_data, bitset_shape, 5, MLX_UINT32);
  uint32_t compact_data[1] = {0};
  int compact_shape[] = {1};
  mlx_array compact =
      mlx_array_new_data(compact_data, compact_shape, 1, MLX_UINT32);

  int key_scales_shape[] = {1, 1, 1, 1, 3};
  float key_scales_data[3] = {1.0f, 0.0f, 0.0f};
  mlx_array key_scales =
      mlx_array_new_data(key_scales_data, key_scales_shape, 5, MLX_FLOAT32);
  int value_scales_shape[] = {1, 1, 1, 1, 2};
  float value_scales_data[2] = {1.0f, 0.0f};
  mlx_array value_scales =
      mlx_array_new_data(value_scales_data, value_scales_shape, 5, MLX_FLOAT32);
  int key_page_summary_shape[] = {1, 1, 1, 1};
  float key_page_summary_data[1] = {1.0f};
  mlx_array key_page_summary = mlx_array_new_data(
      key_page_summary_data, key_page_summary_shape, 4, MLX_FLOAT32);
  int key_candidate_sketch_shape[] = {1, 1, 1, 64};
  float key_candidate_sketch_data[64] = {0.0f};
  mlx_array key_candidate_sketch = mlx_array_new_data(
      key_candidate_sketch_data, key_candidate_sketch_shape, 4, MLX_FLOAT32);

  mlx_fast_turbo_quant_attention_layout_descriptor layout = {
      .layout_version = 6,
      .batch_size = 1,
      .kv_head_count = 1,
      .capacity = 1,
      .logical_length = 1,
      .ring_offset = 0,
      .pinned_prefix_length = 0,
      .head_dimension = 64,
      .groups_per_vector = 1,
      .magnitude_words_per_group = 8,
      .bitset_words_per_group = 2};
  mlx_fast_turbo_quant_precision_policy_descriptor precision = {
      .preset = 35,
      .group_size = 64,
      .key_base_bits = 3,
      .key_high_bits = 4,
      .high_precision_numerator = 500,
      .high_precision_denominator = 1000,
      .value_bits = 4,
      .key_scales_per_group = 3,
      .value_scales_per_group = 2,
      .value_magnitude_words_per_group = 8,
      .key_seed = 1,
      .value_seed = 2};
  mlx_fast_turbo_quant_attention_options options = {
      .scale = 0.125f,
      .causal = true,
      .split_k_blocks = 0,
      .sparse_v_threshold = 0.0f,
      .diagnostics = false,
      .backend_version = 3};

  mlx_fast_turbo_quant_segmented_attention_backend backend =
      MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_NATIVE_FUSED;
  mlx_status probe_status = mlx_fast_turbo_quant_segmented_attention_get_backend(
      &backend, false, stream);
  if (probe_status != MLX_STATUS_SUCCESS ||
      backend != MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_UNAVAILABLE) {
    return 1;
  }

  bool available = true;
  probe_status = mlx_fast_turbo_quant_segmented_attention_is_available(
      &available, false, stream);
  if (probe_status != MLX_STATUS_SUCCESS || available) {
    return 2;
  }

  backend = MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_NATIVE_FUSED;
  probe_status =
      mlx_fast_turbo_quant_segmented_attention_get_backend_for_codec(
          &backend,
          MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_CODEC_POLAR_QJL,
          false,
          stream);
  if (probe_status != MLX_STATUS_SUCCESS ||
      backend != MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_UNAVAILABLE) {
    return 21;
  }

  backend = MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_NATIVE_FUSED;
  probe_status =
      mlx_fast_turbo_quant_segmented_attention_get_backend_for_codec(
          &backend,
          MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_CODEC_POLAR_WHT,
          true,
          stream);
  if (probe_status != MLX_STATUS_SUCCESS ||
      backend != MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_UNAVAILABLE) {
    return 22;
  }

  available = true;
  probe_status =
      mlx_fast_turbo_quant_segmented_attention_is_available_for_codec(
          &available,
          MLX_FAST_TURBO_QUANT_SEGMENTED_ATTENTION_CODEC_HYBRID_K8_POLAR_WHT_VALUE,
          true,
          stream);
  if (probe_status != MLX_STATUS_SUCCESS || available) {
    return 23;
  }

  mlx_array output = mlx_array_new();
  mlx_status status = mlx_fast_turbo_quant_segmented_attention(
      &output, queries, key_packed, key_signs, key_high_mask, compact,
      key_scales, value_packed, compact, compact, compact, value_scales,
      layout, precision, options, stream);
  if (status != MLX_STATUS_UNSUPPORTED) {
    return 3;
  }

  mlx_vector_array outputs = mlx_vector_array_new();
  options.diagnostics = true;
  status = mlx_fast_turbo_quant_segmented_attention_with_diagnostics(
      &outputs, queries, key_packed, key_signs, key_high_mask, compact,
      key_scales, value_packed, compact, compact, compact, value_scales,
      layout, precision, options, stream);
  if (status != MLX_STATUS_UNSUPPORTED) {
    return 4;
  }

  options.diagnostics = false;
  options.sparse_v_selection_mode = 6;
  options.sparse_v_top_k = 1;
  status = mlx_fast_turbo_quant_segmented_attention_with_page_summaries(
      &output, queries, key_packed, key_signs, key_high_mask, compact,
      key_scales, value_packed, compact, compact, compact, value_scales,
      key_page_summary, layout, precision, options, stream);
  if (status != MLX_STATUS_UNSUPPORTED) {
    return 5;
  }

  mlx_vector_array_free(outputs);
  outputs = mlx_vector_array_new();
  options.diagnostics = true;
  status =
      mlx_fast_turbo_quant_segmented_attention_with_page_summaries_and_diagnostics(
          &outputs, queries, key_packed, key_signs, key_high_mask, compact,
          key_scales, value_packed, compact, compact, compact, value_scales,
          key_page_summary, layout, precision, options, stream);
  if (status != MLX_STATUS_UNSUPPORTED) {
    return 6;
  }

  mlx_vector_array_free(outputs);
  outputs = mlx_vector_array_new();
  options.diagnostics = false;
  options.sparse_v_selection_mode = 7;
  options.sparse_v_top_k = 1;
  options.sparse_v_recent_tokens = 1;
  options.sparse_v_candidate_pages = 1;
  status = mlx_fast_turbo_quant_segmented_attention_with_candidate_sketches(
      &output, queries, key_packed, key_signs, key_high_mask, compact,
      key_scales, value_packed, compact, compact, compact, value_scales,
      key_candidate_sketch, layout, precision, options, stream);
  if (status != MLX_STATUS_UNSUPPORTED) {
    return 7;
  }

  options.diagnostics = true;
  status =
      mlx_fast_turbo_quant_segmented_attention_with_candidate_sketches_and_diagnostics(
          &outputs, queries, key_packed, key_signs, key_high_mask, compact,
          key_scales, value_packed, compact, compact, compact, value_scales,
          key_candidate_sketch, layout, precision, options, stream);
  if (status != MLX_STATUS_UNSUPPORTED) {
    return 8;
  }

  mlx_vector_array_free(outputs);
  mlx_array_free(output);
  mlx_array_free(key_candidate_sketch);
  mlx_array_free(key_page_summary);
  mlx_array_free(value_scales);
  mlx_array_free(key_scales);
  mlx_array_free(compact);
  mlx_array_free(key_high_mask);
  mlx_array_free(key_signs);
  mlx_array_free(value_packed);
  mlx_array_free(key_packed);
  mlx_array_free(queries);
  mlx_stream_free(stream);
  return 0;
}
