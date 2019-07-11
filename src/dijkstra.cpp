#include <iostream>
#include <fstream>
#include <sstream> // for parsing edge list file
#include <string>
#include <map>
#include <set>
#include <vector>
#include <queue> // To set up priority queue
#include <functional> // To use std::greater<T> -> This will prove to be useful in picking the minimum weight
#include <algorithm> // To use reverse
#include <climits> // defines INT_MAX

using namespace std;

typedef pair<int, int> Pair; // First = Weight & Second = Vertex
typedef map<int, vector<Pair>> AdjList; // Vertex ID as key, outgoing edges in vector as value

void createAndAddEdge(AdjList &adjList, int u, int weight, int v) { // add edges manually
    adjList[u].push_back(make_pair(weight, v));
}

void loadEdgeList(const string edge_file, bool directed, AdjList &adjList) { // add edges from text file
    ifstream edge_fs(edge_file);
    if(edge_fs) {
        string line;
        while (getline(edge_fs, line)) {
            stringstream ss(line);
            int u, v, weight = 1;
            ss >> u;
            ss >> v;
            adjList[u].push_back(make_pair(weight, v));
            if(directed) {
                auto itr = adjList.find(v);
                if (itr == adjList.end())
                    adjList[v] = vector<Pair>();
            }
            else
                adjList[v].push_back(make_pair(weight, u));
        }
        edge_fs.close();
    }
}

void DijkstrasAlgorithm(AdjList adjList, int source, map<int, int> &predecessor, map<int, int> &shortestPath) {
    priority_queue<Pair, vector<Pair>, greater<Pair> > PQ; // Set up priority queue
    Pair info;
    int weight;
    set<int> visited;

    shortestPath[source] = 0; // Set source distance to zero
    int numVertices = adjList.size();
    for (auto v: adjList) {
        predecessor[v.first] = INT_MAX; // initialize all predessor to +infinity
        if (v.first != source)
            shortestPath[v.first] = INT_MAX; // Initialize everything else to +infinity
    }

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
            if ((weight + (it->first)) < shortestPath[it->second]) { // Check if we can do better
                shortestPath[it->second] = weight + (it->first); // Update new distance
                predecessor[it->second] = source; // Update new predecessor
                PQ.push(make_pair(shortestPath[it->second], it->second)); // Push vertex and weight onto Priority Queue
            } // Update distance
    } // While Priority Queue is not empty
} // DijkstrasAlgorithm

void FindPath(int source, int target, map<int, int> predecessor, vector<int> &path) {
    int v = target;
    while (v != source) {
        path.push_back(v);
        v = predecessor[v];
    }
    path.push_back(v);
    reverse(path.begin(), path.end());
}

int main(int argc, char* argv[]) {
    string edge_file = "c:/Users/zhans/Documents/cs-service/sample-data/rmat-10-8.txt";
    int source = 0, target = 938;
    AdjList adjList;
    map<int, int> predecessor;
    map<int, int> shortestPath;
    vector<int> path;

    loadEdgeList(edge_file, true, adjList);

    DijkstrasAlgorithm(adjList, source, predecessor, shortestPath);

    FindPath(source, target, predecessor, path);

    cout << "Shortest path from source vertex " << source << ":\n";
    for (auto target: shortestPath)
        cout << target.first << " " << target.second << endl;

    cout << "Path to vertex " << target << ": ";
    for (auto v: path)
        cout << v << " ";
    cout << endl;
}
