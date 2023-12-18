# Homework 10

I discovered 162 subgraphs. I could definitely have discovered more, but I decided to stop because the algorithm I developed was not as efficient as it should be. I implemented in Python.

As can be seen in the file `code.ipynb`, my code first loads the data into a Pandas Dataframe called edges, before halving the length by doing some filtering. I thought that, in order to be represented in a subgraph with $\text{support} \geq 10 000$, a combination of labels have to occur $\geq 10 000$ times in the edges. That is why I filter the edges that do not satisfy that requirement out, leaving ~150 combinations of labels that can occur. Since this filtering is somewhat demandind, I store them as dictionaries in the folder `preprocessed_edges/`.

I also thought that the total amount of topologies in the subgraphs could be a maximum of 8. However, in my implementation I simply looked at the topology of a series of nodes, shown in the picture below:

![Topology of my discovered subgraphs](hw10/top.png)

Seeing as I only look at the series, I can look at all possible combinations of the label combinations that I filtered earlier. I iterate over the possibilities, and look at whether they occur more than 10 000 times by going through all the edges for those label combinations. As this is a brute-force approach it takes a long time, and I only ran the code for a few minutes until I thought I had a satisfactory number of patterns.

The code can be ran simply using the juputer notebook in `code.ipynb`.
