# Part 1

`edge-cut.cpp` contains my implementation of balanced p-way edge-cut, and `vertex-cut` contains my implementation of balanced p-way vertex-cut. Use the included `Makefile` to compile the programs if the binaries don't work.

The `data/` folder contains the output for several tests of both edge-cut and vertex-cut for the `roadNet-PA` and `synthesized-1b` graphs.

The visualisations for edge-cut and vertex-cut on the small-5 graph is included in the files `data/small-5.graph_edge_cut_3_output_with_visualisation.txt` and `data/small-5.graph_vertex_cut_3_output_with_visualisation.txt`, respectively.

Edge-cut splits based on edges, tracking vertices and replicated edges. Vertex-cut uses master and mirror vertices, distributing vertices and counting edges differently.

**Edge-Cut**: More balanced edge distribution but less direct control over vertex placement.

*Best for*: Graphs with uniform or random edge distribution. It excels when edges are evenly spread across vertices without distinct clusters.

*Worst for*: Graphs with dense clusters or where edges are highly concentrated between specific subsets of vertices. In such cases, it might lead to uneven distribution, causing some machines to handle significantly more edges.
Vertex-Cut with Mirroring (Second Algorithm)

**Vertex-Cut**: Closer placement of related vertices but introduces mirror vertices, impacting storage and access.

*Best for*: Graphs with strong vertex-centric relationships or clustered structures. When certain vertices have dense connections within clusters, this method might efficiently group related vertices together.

*Worst for*: Graphs with evenly spread edges and weak vertex-to-vertex relationships. It might create unnecessary mirror vertices, leading to increased storage overhead without significant communication benefits.

To find which one works best in specific use cases, you can assess based on communication efficiency, storage impact, and load balancing. Both methods have strengths and weaknesses, so the choice depends on specific graph characteristics and distribution requirements.

The score for the twitter graph is given in the files `data/twitter-2010.graph_edge_cut_3_output.txt` and `data/twitter-2010.graph_vertex_cut_3_output.txt`

## Detailed description of edge-cut implementation

**Classes**: Define graph elements (*Vertex*, *Edge*, *Graph*) and manage machine storage (*Machine*).

**Functions**:

- `hashFunction`: Determines which machine should store a vertex based on a simple hash.
- `distributeGraph`: Assigns edges to machines using the hash, distributing them across the machines based on their source and destination vertices.

**Main Functionality**:

- Reads edge data from an input file in batches.
- Distributes edges among machines according to their source and destination vertices using the hash function.
- Clears data and resets machines after processing each batch.
- Generates an output file containing information about how edges are distributed across machines.

In essence, the core of this code revolves around the `distributeGraph` function, which uses a hash to allocate edges to machines based on their vertices, ensuring that each machine handles a portion of the graph's edges.

## Detailed description of vertex-cut implementation

**Classes**: Define graph elements (*Vertex*, *Edge*, *Graph*) and manage machine storage (*Machine*).

**Functions**:
`hashFunction`: Determines which machine should store a vertex using a simple hash.
`distributeGraph`: Allocates edges based on their vertices to different machines, tracking both master and mirror vertices.
Various get functions in Machine retrieve counts and information about vertices and edges in a partition.

**Main Functionality**:

- Reads edge data from an input file.
- Distributes edges among machines, updating master and mirror vertices along with edge counts.
- Clears data and resets machines after processing a certain number of edges.
- Generates an output file containing information about vertices and edges in each partition.
-

The distributeGraph function is also here important by assigning edges to machines based on the vertices they connect, creating a partitioning scheme that separates master and mirror vertices, and tracks edge counts for each partition.
