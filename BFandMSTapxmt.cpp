#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <algorithm> 
#include <limits>

using namespace std;



const double INF = numeric_limits<double>::max();
double minDist = 10000000.0000;

vector<int> shortestPath;
double graph[13][13];

void readfile(string& filename)
{
    ifstream inputFile(filename);
    if (inputFile.is_open())
    {
        string line;
        
        for (int i = 0; i < 13; ++i)
        {
            getline(inputFile, line);
            stringstream ss(line);

            string source;
            getline(ss, source, ' ');
            int idx = stoi(source);
            
            for (int j = 0; j < 12; ++j)
            {
                string target;
                getline(ss, target, ' ');
                string distance;
                getline(ss, distance, ' ');
                graph[idx][stoi(target)] = stod(distance);
            }
        }
        inputFile.close();
    }
    
    for (auto & i : graph)
    {
        for (double & j : i)
        {
            if (j == 0)
            {
                j = INF;
            }
        }
    }
}

void bruteForceTSP(vector<int>&tmpPath, int start, double curDist, bool mark[])
{

    if (tmpPath.size() == 13)
    {
        curDist += graph[tmpPath[12]][tmpPath[0]];
    }
    
    if (curDist >= minDist)
    {
        return;
    }
    
    else
    {
        if (tmpPath.size() == 13)
        {
            shortestPath.assign(tmpPath.begin(), tmpPath.end());
            minDist = curDist;
        }
        for (int i = 0; i < 13; ++i)
        {
            if (!mark[i])
            {
                mark[i] = true;
                tmpPath.push_back(i);
                bruteForceTSP(tmpPath, i, curDist + graph[start][i], mark);
                tmpPath.pop_back();
                mark[i] = false;
            }
        }
    }
}

struct Edge
{
    int from, to;
    double weight;

    Edge(int f, int t, double w) : from(f), to(t), weight(w) {}

    
    bool operator<(const Edge& other) const
    {
        return weight < other.weight;
    }
};

int findRoot(int node, vector<int>& parent)
{
    if (parent[node] == -1)
    {
        return node;
    }
    return findRoot(parent[node], parent);
}

void unionSets(int root1, int root2, vector<int>& parent)
{
    parent[root1] = root2;
}


vector<Edge> kruskalMST(const vector<Edge>& edges, int numVertices)
{
    vector<Edge> mst;
    
    vector<int> parent(numVertices, -1);

    vector<Edge> sortedEdges = edges;

    sort(sortedEdges.begin(), sortedEdges.end());

    for (const Edge& edge : sortedEdges)
    {
        int root1 = findRoot(edge.from ,   parent);
        int root2 = findRoot(edge.to   ,   parent);

        if (root1 != root2)
        {
            mst.push_back(edge);
            unionSets(root1, root2, parent);
        }
    }
    return mst;
}

void preorderTraversal(int node, const vector<vector<int>>& adjList, vector<bool>& visited, vector<int>& tour)
{
    visited[node] = true;
    tour.push_back(node);

    for (int neighbor : adjList[node])
    {
        if (!visited[neighbor])
        {
            preorderTraversal(neighbor, adjList, visited, tour);
        }
    }
}

void tspMSTApproximation()
{
    clock_t start,end;
    start = clock();
    int numVertices = 13;

    vector<Edge> edges;
    for (int i = 0; i < numVertices; ++i)
    {
        for (int j = i + 1; j < numVertices; ++j)
        {
            if (graph[i][j] != 0)
            {
                //edges.push_back(i, j, graph[i][j]);
                edges.emplace_back(i, j, graph[i][j]);
            }
        }
    }

    vector<Edge> mst = kruskalMST(edges, numVertices);

    
    vector<vector<int>> adjList(numVertices);
    cout << "mst: " << endl;
    
    for (const Edge& edge : mst)
    {
        adjList[edge.from].push_back(edge.to);
        adjList[edge.to].push_back(edge.from);
        cout << edge.from << "---" << edge.to << endl;
    }

    vector<bool> visited(numVertices, false);
    vector<int> tour;
    preorderTraversal(0, adjList, visited, tour);
    end = clock();

    cout << "------MST approximations result------" << endl;
    double totalDist = 0;
    for (int i = 0; i < tour.size() - 1; ++i)
    {
        totalDist += graph[tour[i]][tour[i + 1]];
    }
    
    totalDist += graph[tour[12]][tour[0]];
    for (int node : tour)
    {
        cout << node << " ";
    }
    cout << tour[0] << endl;
    cout << "time used: " << double(end-start)/CLOCKS_PER_SEC<<"s; " << " total distance: " <<  totalDist << endl;
}








int main() {
    // Replace "your_graph_file.txt" with the actual filename
    string filename = "graph7.txt";

    // Read the graph from the file
    readfile(filename);

    clock_t start,end;
    start = clock();
    vector<int> tmpPath;
    tmpPath.push_back(0);
    bool mark[13] = {};
    mark[0] = true;
    bruteForceTSP(tmpPath, 0, 0, mark);
    end = clock();
    cout << "------brute force result------" << endl;
    for (const auto & item: shortestPath)
    {
        cout << item << " " ;
    }
    cout << shortestPath[0] << endl;

    cout << "time used: " << double(end-start)/CLOCKS_PER_SEC<<"s; " << " minimum total distance: " <<  minDist << endl;

    tspMSTApproximation();

    return 0;
}