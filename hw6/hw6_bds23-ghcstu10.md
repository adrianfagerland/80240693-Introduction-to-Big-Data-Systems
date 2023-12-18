# HW 6

## Word Count

The implementation for Word Count is very simple, and done in one line using `flatMap()`, `map()` and `reduceByKey()`. We call `.split()` on the lines to seperate words, then making a tuple for each of the words with value one. Then we use `reduceByKey()` to count the numbers of occurences for each word. That gives a correct implementation of Word Count.

## Page Rank

The Page Rank implementation is slightly more complex. First, we read the file and create a RDD of tuples where the first element is the source node, and the second is an iterable of all the edges out of this node. Here, `distinct()` is used to remove duplicate edges, since the task requested that these were handles as one single edge. We count the number of keys in this to get `num_nodes`. We then initalize the scores for each of the nodes, and partition the RDD to handle each node seperately. We then start the loop where we do the Page Rank iterations and check for convergence.

The Page Rank iteration is implemented by joining the edges with their current scores, then using `flatMap()`, `reduceByKey()`, `mapValues()`. Inside `flatMap()` we pass the scores out using the edges, before summing the result of this and applying the dampening. Since there are no nodes that have no out or in nodes, we don't need to make a pass over these nodes as we would have to do using a normal Page Rank implementation. There are surely ways to speed this up, but since speed was not given as a requirement or evalution point no such efforts were prioritised.

### Results

The top 5 nodes with corresponding scores are:
Table under:
| 263 | 537 | 965 | 243 | 285 |
|----|----|----|---|----|
|0.0020202911815182184|0.0019433415714531497|0.0019254478071662631|0.001852634016241731|0.0018273721700645144|

The algorithm used 2919 seconds for 100 iterations.
