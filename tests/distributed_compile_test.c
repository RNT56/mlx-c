#include <stdbool.h>
#include <stddef.h>

#include "mlx/c/mlx.h"

static void ignore_error(const char* msg, void* data) {
  (void)msg;
  (void)data;
}

int main(void) {
  mlx_set_error_handler(ignore_error, NULL, NULL);

  mlx_distributed_group group = mlx_distributed_group_new();
  mlx_distributed_group split = mlx_distributed_group_new();
  mlx_array input = mlx_array_new();
  mlx_array output = mlx_array_new();
  mlx_stream stream = mlx_default_cpu_stream_new();
  int shape[] = {1};
  int value = 1;
  bool available = false;
  int rank = -1;
  int size = -1;

  input = mlx_array_new_data(&value, shape, 1, MLX_INT32);
  (void)mlx_status_string(MLX_STATUS_SUCCESS);
  (void)mlx_distributed_is_available_status(&available, "ring");
  (void)mlx_distributed_init(&group, false, "ring");
  (void)mlx_distributed_group_rank_status(&rank, group);
  (void)mlx_distributed_group_size_status(&size, group);
  (void)mlx_distributed_group_split(&split, group, 0, -1);
  (void)mlx_distributed_all_sum(&output, input, group, stream);
  (void)mlx_distributed_all_max(&output, input, group, stream);
  (void)mlx_distributed_all_min(&output, input, group, stream);
  (void)mlx_distributed_all_gather(&output, input, group, stream);
  (void)mlx_distributed_sum_scatter(&output, input, group, stream);
  (void)mlx_distributed_send(&output, input, 0, group, stream);
  (void)mlx_distributed_recv(&output, shape, 1, MLX_INT32, 0, group, stream);
  (void)mlx_distributed_recv_like(&output, input, 0, group, stream);

  mlx_stream_free(stream);
  mlx_array_free(output);
  mlx_array_free(input);
  mlx_distributed_group_free(split);
  mlx_distributed_group_free(group);
  return 0;
}
