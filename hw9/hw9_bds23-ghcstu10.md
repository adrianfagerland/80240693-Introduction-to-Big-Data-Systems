# Homework 9

I implemented Pagerank Delta and K-core with gridgraph. Below is the detailed explanation of each of the implementations.

## Pagerank delta

This code implements the PageRank algorithm, a metric used to gauge the significance of nodes within a graph.

Initially, it parses command-line arguments to determine the graph's data path, the number of iterations for PageRank computation, and allocates memory as per the specified budget. The initialization phase encompasses setting up the graph's structure, along with initializing vertex degrees, PageRank values, sums, and deltas.

Subsequently, the code proceeds to calculate the out-degree for each vertex by tallying the number of outgoing edges, an essential step in the PageRank computation process.

The PageRank algorithm begins with the initialization of PageRank, sum, and delta values for each vertex. PageRank values are initially set as the reciprocal of the vertex's degree, while both sum and delta are initialized to zero.

Within the PageRank iteration loop, the code performs several pivotal tasks. Firstly, it calculates the contribution of each vertex to the PageRank values of its neighboring vertices. Then, it updates delta values based on the contributions received from neighboring vertices. Subsequently, it modifies PageRank values considering the calculated delta, incorporating a propagation threshold to ascertain convergence. These steps are iterated for the specified number of iterations, refining the PageRank values with each iteration.

Finally, the code measures the time taken for the specified number of iterations to converge, providing a measure of the computational efficiency and convergence speed of the PageRank calculation for the given graph.

## K-core

This code focuses on determining the coreness of individual vertices within a graph and identifying the maximum coreness value present.

Initially, it reads command-line arguments to establish the graph data path and allocate memory for computations. The graph structure, alongside vertex degrees and coreness values, are set up as part of the initialization phase.

The algorithm proceeds to compute the out-degree for each vertex by tallying the number of outgoing edges, setting the initial coreness values identical to the degrees of the vertices.

Using an iterative process, the code increments a variable k while assessing alterations in coreness values. It involves a dual-step update process: first, it adjusts vertex degrees to zero if their coreness surpasses the current k value and their degree falls below k. Second, it resets coreness to zero for vertices exhibiting a degree of zero.

Upon completing the iterative process, the code identifies and returns the maximum coreness value attained throughout the computation.

Finally, the algorithm measures the time consumed for computing the coreness values, providing insights into the performance of this coreness calculation method for the given graph.

## Results

### Pagerank Delta

Using 8 GBs of RAM, these are the results optained for the pagerank delta algorithm:

| Grid Size | Iterations | Average Degree Calculation Time (seconds) | Average Total Time (seconds) |
|-----------|------------|-------------------------------------------|------------------------------|
| 4x4       | 20, 40     | (0.22 + 0.21) / 2 = 0.215                 | (6.93 + 10.57) / 2 = 8.75    |
| 400x400   | 20, 40     | (0.31 + 0.30) / 2 = 0.305                 | (8.47 + 13.60) / 2 = 11.035  |

### k-core

For the livejournal dataset, k-core returned that the maxium coreness values is 20293. Using 8 GBs of RAM, the table below shows the time spent for a 4x4 and 400x400 preprocessed grid.

| Grid Size | Degree Calculation (s) | Coreness Calculation (s) |
|-----------|------------------------|------------------------------------|
| 400x400       | 0.30                             | 0.32                               |
| 4x4   | 1.12                             | 1.20                               |

The homework task didn't specify which results were wanted, so that is the reason why the results presented are so basic.

## Code execution

The code can be compiled using the same Makefile as was given in `hw9_GridGraph`, and should be so simple that no further explanation is needed.
