add_executable(mlx-c-distributed-compile distributed_compile_test.c)
target_link_libraries(mlx-c-distributed-compile PRIVATE mlxc)
add_test(NAME mlx-c-distributed-compile
         COMMAND mlx-c-distributed-compile)

add_executable(mlx-c-distributed-runtime distributed_runtime_test.c)
target_link_libraries(mlx-c-distributed-runtime PRIVATE mlxc)
add_test(NAME mlx-c-distributed-runtime
         COMMAND mlx-c-distributed-runtime)

add_executable(mlx-c-distributed-unavailable distributed_unavailable_test.c)
target_link_libraries(mlx-c-distributed-unavailable PRIVATE mlxc)
add_test(NAME mlx-c-distributed-unavailable
         COMMAND mlx-c-distributed-unavailable)
set_tests_properties(mlx-c-distributed-unavailable
                     PROPERTIES SKIP_RETURN_CODE 77)

add_executable(mlx-c-fast-quantized-sdpa fast_quantized_sdpa_test.c)
target_link_libraries(mlx-c-fast-quantized-sdpa PRIVATE mlxc)
add_test(NAME mlx-c-fast-quantized-sdpa
         COMMAND mlx-c-fast-quantized-sdpa)

add_executable(mlx-c-fast-turbo-quant-sdpa fast_turbo_quant_sdpa_test.c)
target_link_libraries(mlx-c-fast-turbo-quant-sdpa PRIVATE mlxc)
add_test(NAME mlx-c-fast-turbo-quant-sdpa
         COMMAND mlx-c-fast-turbo-quant-sdpa)
