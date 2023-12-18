# Homework 12

Two different scheduling strategies are showcased, demonstrating optimizations for parallelism and vectorization. The first strategy divides the computation into tiles and parallelizes them, effectively utilizing vectorized operations for enhanced performance. Meanwhile, the second strategy applies a more intricate tiling scheme, splitting the computation across different dimensions and interleaving vectorization and parallelism

## Scheduling primitives

Following is an overview of which scheduling primitives I used in my two scheduling approaches, along with why I used each of them:

- Tile
  - Used to partition the computation space into smaller tiles. Two different tile sizes are applied: 4x4 and 8x8. Tiling helps improve data locality, enabling better cache utilization and reducing memory access overhead. It enhances memory access patterns and locality, reducing cache misses.
- Fuse
  - It merges loop variables (x, y, x_outer, y_outer, tile_index) to enable efficient parallelization across these dimensions. Fusing x and y into tile_index is to parallelize the computation effectively. Additionally, fusing x_outer, y_outer, and tile_index contributes to optimizing the parallel execution strategy, potentially reducing loop overhead and improving performance.
- Vectorize
  - Vectorize is applied to the inner loops (x_inner and co_inner) with a vector size of 4. This technique performs operations on multiple data elements simultaneously.
- Parallel
  - This introduces parallelism by executing loop iterations concurrently. Parallelization is applied to the fused loop (tile_index) to maximize hardware parallelism and optimize performance. This technique allows multiple independent operations to run simultaneously, utilizing multicore architectures efficiently.
- Compute Root
  - It declares the output function to be computed at the outermost level. This choice aids in better control over computation and optimization strategies, optimizing the overall computation process.
- Reorder (approach 2, commented out currently)
  - Specifies a different order of computation for update steps. Although not applied in the final code, reordering loop variables (r.x, r.y, r.z, x_inner, y_inner, co_inner, x_outer, y_outer, co_outer) might potentially optimize memory access patterns and computation sequence.

## Results

Table 1 shows the results for different `dilation` values. As can be seen by the table, my implementation performs considerably slower than the oneDNN method, although this is still a lot faster than the starter code supplied.

| Dilation | Halide Time (ms) | Halide GFLOP/s | oneDNN Time (ms) | oneDNN GFLOP/s |
|----------|------------------|----------------|------------------|----------------|
| 63       | 855.286556       | 13.792430      | 13.172219        | 895.557516     |
| 31       | 908.412530       | 12.985818      | 12.470659        | 945.938790     |
| 15       | 928.042881       | 12.711137      | 11.509352        | 1024.947380    |
| 0        | 980.113672       | 12.035829      | 10.994474        | 1072.946337    |

## Running the code

The file `dilated_conv.cpp` can be put directly into the folder with the starter code supplied, and compiled with the starter Makefile to create the binary.
