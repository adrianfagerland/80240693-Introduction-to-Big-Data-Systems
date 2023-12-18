#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
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
    vector<Vertex> vertices;
    vector<Edge> edges;
};


int hashFunction(VertexId value, int numMachines) {
    return value % numMachines;
}

class Machine {
public:
    unordered_map<VertexId, bool> masterVertices; 
    unordered_map<VertexId, int> mirrorVertices; 
    unordered_map<VertexId, int> edgeCount; 

    void addVertex(VertexId vertexId) {
        masterVertices[vertexId] = true; 
    }

    void addMirrorVertex(VertexId vertexId) {
        mirrorVertices[vertexId]++; 
    }

    void addEdge(VertexId src, VertexId dst) {
        edgeCount[src]++; 
        edgeCount[dst]++; 
    }

    int getMasterVerticesCount() const {
        return masterVertices.size();
    }

    int getTotalVerticesCount() const {
        int totalVertices = masterVertices.size();
        for (const auto& pair : mirrorVertices) {
            totalVertices += pair.second; 
        }
        return totalVertices;
    }

    int getEdgesInPartition() const {
        int edgesInPartition = 0;
        for (const auto& pair : edgeCount) {
            edgesInPartition += pair.second;
        }
        return edgesInPartition / 2; 
    }

    std::string getInfo(int partition) const {
        std::ostringstream oss;
        oss << "Partition " << partition << "\n";
        oss << getMasterVerticesCount() << "\n";
        oss << getTotalVerticesCount() << "\n";
        oss << getEdgesInPartition() << "\n";
        return oss.str();
    }
};

void distributeGraph(Graph& graph, vector<Machine>& machines, int numMachines) {
    for (auto& edge : graph.edges) {
        int machineIndexSrc = hashFunction(edge.src, numMachines);
        int machineIndexDst = hashFunction(edge.dst, numMachines);

        machines[machineIndexSrc].addVertex(edge.src);
        machines[machineIndexDst].addMirrorVertex(edge.src);

        machines[machineIndexSrc].addVertex(edge.dst);
        machines[machineIndexDst].addMirrorVertex(edge.dst);

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
        for (const auto& pair : machines[i].masterVertices) {
            oss << pair.first << "\n";
        }

        oss << "Mirror Vertices:\n";
        for (const auto& pair : machines[i].mirrorVertices) {
            oss << pair.first << "\n";
        }

        oss << "Edges:\n";
        for (const auto& pair : machines[i].edgeCount) {
            oss << pair.first << " -> " << pair.second << "\n";
        }

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

    const int numMachines = 3; 
    vector<Machine> machines(numMachines);
    Graph graph;

    VertexId srcId, dstId;
    while (fin.read(reinterpret_cast<char*>(&srcId), sizeof(srcId))) {
        if (!fin.read(reinterpret_cast<char*>(&dstId), sizeof(dstId))) {
            cerr << "Error reading dstId\n";
            break;
        }

        graph.edges.push_back({srcId, dstId});

        if (graph.edges.size() % 1000000 == 0) {
            distributeGraph(graph, machines, numMachines);
            cout << "Processed " << graph.edges.size() << " edges.\n";
            graph.edges.clear(); 
        }
    }

    fin.close();

    if (!graph.edges.empty()) {
        distributeGraph(graph, machines, numMachines); 
    }

    std::string inputFile = argv[1];
    
    std::ostringstream oss;
    oss << inputFile << "_vertex_cut_" << numMachines << "_output.txt";
    std::string outputFile = oss.str();

    std::ofstream outfile(outputFile);

    for (size_t i = 0; i < machines.size(); i++) {
        outfile << machines[i].getInfo(i);
    }

    // outfile << visualizePartitions(machines) << "\n";

    outfile.close();

    return 0;
}
