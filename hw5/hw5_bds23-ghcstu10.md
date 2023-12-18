# Homework 5

## Task 1

### Q1

One approach to tackle the issue of network traffic and data shuffling in the MapReduce framework is to use Combiners. Combiners are functions that run on the mapper nodes before data is sent to the reducers. They aggregate and perform preliminary processing of the data locally on the mapper nodes, and this local data reduction minimizes the volume of data that needs to be transferred across the network, especially when there is redundant data in the mapper output. Implementing Combiners can in certain cases improve system performance and resource management, but that is dependant on several factors.

### Q2

Speculative execution is a popular approach. Speculative execution is a mechanism that identifies and duplicates tasks that are progressing too slowly, running a backup or speculative task alongside the original one. It entails monitoring tasks, and duplicating them if they are taking longer than expected, and then using the first instance to finish. That should reduce the effect that a slow node has on the overall efficiency.

## Task 2

The `README.md` does not ask for information about the implementation, but I included a short bit here:

The implementation defines a Mapper class that reads each line of the input file, extracts the output node of each edge, and emits a key-value pair with the output node as the key and a value of 1. The code also defines a Reducer class that sums up the values for each key (node) and outputs the node and its out-degree.

It is a very simple solution, and based on a short review of the correct vs output results it seems that the implementation of MapReduce is correct.
