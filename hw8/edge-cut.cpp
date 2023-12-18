#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>

using namespace std;

typedef int VertexId;

class Vertex {
public:
    VertexId id;
};

class Edge {
public:
    VertexId src, dst;
};

class Graph {
public:
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
};

int hashFunction(VertexId value, int numMachines) {
    return value % numMachines;
}

class Machine {
public:
    std::unordered_map<VertexId, std::vector<VertexId>> vertices;
    std::unordered_map<VertexId, int> edgeCount;

    void addVertex(VertexId vertexId) {
        vertices[vertexId]; 
    }

    void addEdge(VertexId src, VertexId dst) {
        vertices[src].push_back(dst);
        edgeCount[src]++;
    }

    int getTotalVerticesCount() const {
        int totalVertices = vertices.size();
        for (const auto& pair : vertices) {
            totalVertices += pair.second.size(); 
        }
        return totalVertices;
    }

    std::string getInfo(int partition) {
        std::ostringstream oss;
        oss << "Partition " << partition << "\n";
        oss << vertices.size() << "\n";
        oss << getTotalVerticesCount() << "\n";
        int replicatedEdges = 0;
        for (const auto& pair : edgeCount) {
            if (pair.second > 1) replicatedEdges += pair.second;
        }
        oss << replicatedEdges << "\n";
        oss << edgeCount.size() << "\n";
        return oss.str();
    }
};

void distributeGraph(Graph& graph, std::vector<Machine>& machines) {
    for (auto& edge : graph.edges) {
        int machineIndexSrc = hashFunction(edge.src, machines.size());
        int machineIndexDst = hashFunction(edge.dst, machines.size());

        machines[machineIndexSrc].addVertex(edge.src);
        machines[machineIndexDst].addVertex(edge.dst);

        machines[machineIndexSrc].addEdge(edge.src, edge.dst);
        machines[machineIndexDst].addEdge(edge.dst, edge.src);
    }
}

std::string visualizePartitions(const vector<Machine>& machines) {
    std::ostringstream oss;
    oss << "-----------------\n";
    for (size_t i = 0; i < machines.size(); ++i) {
        oss << "Partition " << i << ":\n";
        oss << "Master Vertices:\n";
        for (const auto& pair : machines[i].vertices) {
            oss << pair.first << "\n";
        }

        oss << "Edges:\n";
        int replicatedEdges = 0;
        for (const auto& pair : machines[i].edgeCount) {
            if (pair.second > 1) {
                replicatedEdges += pair.second;
                oss << pair.first << " -> " << pair.second << " (Replicated)\n";
            } else {
                oss << pair.first << " -> " << pair.second << "\n";
            }
        }
        oss << "Number of Replicated Edges in Partition " << i << ": " << replicatedEdges << "\n";

        oss << "-----------------\n";
    }
    return oss.str();
}


int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [input]\n";
        exit(-1);
    }

    ifstream fin(argv[1], ios::binary);
    if (!fin) {
        cerr << "Error opening file " << argv[1] << endl;
        exit(-1);
    }

    const int batchSize = 1000000; 
    Graph graph;
    int edgeCount = 0;

    const int numMachines = 3; 
    std::vector<Machine> machines(numMachines);

    while (true) {
        Edge edge;
        if (!fin.read(reinterpret_cast<char*>(&edge.src), sizeof(edge.src))) break;
        if (!fin.read(reinterpret_cast<char*>(&edge.dst), sizeof(edge.dst))) break;

        graph.edges.push_back(edge);
        edgeCount++;

        if (edgeCount == batchSize) {
            distributeGraph(graph, machines);
            graph.edges.clear();
            edgeCount = 0;

            for (auto& machine : machines) {
                machine.vertices.clear();
                machine.edgeCount.clear();
            }

            cout << "Processed " << batchSize << " edges..." << endl;
        }

    }

    fin.close();

    if (!graph.edges.empty()) {
        distributeGraph(graph, machines); 
    }

    std::string inputFile = argv[1];
    
    std::ostringstream oss;
    oss << inputFile << "_edge_cut_" << numMachines << "_output.txt";
    std::string outputFile = oss.str();

    std::ofstream outfile(outputFile);

    for (size_t i = 0; i < machines.size(); i++) {
        outfile << machines[i].getInfo(i);
    }

    // outfile << visualizePartitions(machines) << "\n";

    outfile.close();

    return 0;
}
