#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "mlx/c/mlx.h"

static void ignore_error(const char* msg, void* data) {
  (void)msg;
  (void)data;
}

int main(void) {
#ifndef _WIN32
  unsetenv("MLX_HOSTFILE");
  unsetenv("MLX_RANK");
#endif
  mlx_set_error_handler(ignore_error, NULL, NULL);

  mlx_distributed_group group = mlx_distributed_group_new();
  mlx_status status = mlx_distributed_init(&group, false, "ring");
  if (status != MLX_STATUS_SUCCESS) {
    return 1;
  }

  int rank = -1;
  int size = -1;
  if (mlx_distributed_group_rank_status(&rank, group) != MLX_STATUS_SUCCESS) {
    return 2;
  }
  if (mlx_distributed_group_size_status(&size, group) != MLX_STATUS_SUCCESS) {
    return 3;
  }
  if (rank != 0 || size != 1) {
    return 4;
  }

  mlx_stream stream = mlx_default_cpu_stream_new();
  float data[] = {1.0f, 2.0f, 3.0f, 4.0f};
  int shape[] = {4};
  mlx_array input = mlx_array_new_data(data, shape, 1, MLX_FLOAT32);
  mlx_array output = mlx_array_new();

  status = mlx_distributed_sum_scatter(&output, input, group, stream);
  if (status != MLX_STATUS_SUCCESS) {
    return 5;
  }
  if (mlx_array_eval(output) != 0) {
    return 6;
  }
  if (mlx_array_size(output) != 4) {
    return 7;
  }
  const float* output_data = mlx_array_data_float32(output);
  if (!output_data) {
    return 8;
  }
  for (int i = 0; i < 4; ++i) {
    if (fabsf(output_data[i] - data[i]) > 1e-6f) {
      return 9;
    }
  }

  mlx_array_free(output);
  mlx_array_free(input);
  mlx_stream_free(stream);
  mlx_distributed_group_free(group);
  return 0;
}
