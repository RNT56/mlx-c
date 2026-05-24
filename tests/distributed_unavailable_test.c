#include <stdbool.h>
#include <stddef.h>

#include "mlx/c/mlx.h"

static void ignore_error(const char* msg, void* data) {
  (void)msg;
  (void)data;
}

int main(void) {
  mlx_set_error_handler(ignore_error, NULL, NULL);

  bool nccl_available = true;
  if (mlx_distributed_is_available_status(&nccl_available, "nccl") !=
      MLX_STATUS_SUCCESS) {
    return 1;
  }
  if (nccl_available) {
    return 77;
  }

  mlx_distributed_group group = mlx_distributed_group_new();
  mlx_status status = mlx_distributed_init(&group, true, "nccl");
  mlx_distributed_group_free(group);
  return status == MLX_STATUS_UNSUPPORTED ? 0 : 2;
}
