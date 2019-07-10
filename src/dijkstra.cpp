#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <queue> // To set up priority queue
#include <functional> // To use std::greater<T> -> This will prove to be useful in picking the minimum weight
#include <climits>

using namespace std;

typedef pair<int, int> Pair; // First = Weight & Second = Vertex
typedef map<int, vector<Pair>> AdjList;

void createAndAddEdge(AdjList &adjList, int u, int weight, int v) {
    adjList[u].push_back(make_pair(weight, v));
}

void loadEdgeList(const char *edge_file, AdjList &adjList) {
    ifstream edge_fs(edge_file);
    if(edge_fs) {
        string line;
        while (getline(edge_fs, line)) {
            stringstream ss(line);
            int u, v, weight = 1;
            ss >> u;
            ss >> v;
            adjList[u].push_back(make_pair(weight, v));
        }
        edge_fs.close();
    }
}

void DijkstrasAlgorithm(AdjList adjList, int source, map<int, int> &ShortestPath) {
    priority_queue<Pair, vector<Pair>, greater<Pair> > PQ; // Set up priority queue
    Pair info;
    int weight;
    set<int> visited;

    ShortestPath[source] = 0; // Set source distance to zero
    int numVertices = adjList.size();
    for (auto v: adjList)
        if (v.first != source)
            ShortestPath[v.first] = INT_MAX; // Initialize everything else to +infinity

    PQ.push(make_pair(0, source)); // Source has weight 0;

    while (!PQ.empty()) {
        info = PQ.top(); // Use to get minimum weight
        PQ.pop(); // Pop before checking for cycles
        source = info.second; // get the vertex
        weight = info.first; // current distance

        if (visited.find(source) != visited.end()) // Check for cycle
            continue; // Already accounted for it, move on

        visited.insert(source); // Else, mark the vertex so that we won't have to visit it again

        for (vector<Pair>::iterator it = adjList[source].begin(); it != adjList[source].end(); it++)
             if ((weight + (it->first)) < ShortestPath[it->second]) { // Check if we can do better
                 ShortestPath[it->second] = weight + (it->first); // Update new distance
                 PQ.push(make_pair(ShortestPath[it->second], it->second)); // Push vertex and weight onto Priority Queue
             } // Update distance
    } // While Priority Queue is not empty
} // DijkstrasAlgorithm

int main(void) {

    int source = 0;
    AdjList adjList;
    map<int, int> ShortestPath;

    // createAndAddEdge(adjList, 0, 5, 1);
    // createAndAddEdge(adjList, 0, 10, 3);
    // createAndAddEdge(adjList, 1, 2, 2);
    // createAndAddEdge(adjList, 1, 10, 5);
    // createAndAddEdge(adjList, 1, 5, 4);
    // createAndAddEdge(adjList, 2, 1, 3);
    // createAndAddEdge(adjList, 2, 5, 4);
    // createAndAddEdge(adjList, 2, 3, 0);
    // createAndAddEdge(adjList, 4, 2, 5);
    loadEdgeList("c:/Users/zhans/Documents/cs-service/sample-data/rmat-10-8.txt", adjList);

    DijkstrasAlgorithm(adjList, source, ShortestPath);

    cout << "Shortest path from source vertex " << source << ": /n";
    for (auto target: ShortestPath) {
        cout << target.first << " " << target.second << endl;
    }
}
