#include <climits>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <limits.h>
#include <math.h>
#include <float.h>

//warning: This program doesn't work on VSC but works on VS which I don't understand why, they produce totally different output.

using namespace std;

#define V 13

vector<string> Stringsplit(string str, const char split)
{
    vector<string> strList;
    istringstream iss(str);
    string token;
    while (getline(iss, token, split))
    {
        strList.push_back(token);

    }
    return strList;
}

 map<int, map<int, double>> getTourDistanceMap(string txtName){
    map<int, map<int, double>> result;
    ifstream file(txtName);
    string line;
    if (file.is_open()) {
        cout << "file open" << endl;
        while (getline(file, line)) {
            vector<string> strList;
            strList = Stringsplit(line, ' ');
            map<int, double> m;
            const int lSize = strList.size();
            strList.pop_back();
            for (int i = 0; i < (lSize-1)/2; i++)
            {
                string a = strList.back();
                strList.pop_back();
                string b = strList.back();
                strList.pop_back();
                int ib = stoi(b);
                double ia = stod(a);
                m[ib] = ia;
            }
            string c = strList.front();
            int ic = stoi(c);
            result[ic] = m;
        }
        file.close();
    }
    return result;
 }

double getMill(int a, int b, map<int, map<int, double>> result){
    map<int, double> vm = result[a];
    double value = vm[b];
    return value;
}

double getMillFromList(vector<int> array, map<int, map<int, double>> tdMap){
    double result = 0;
    for (int i = 0; i < array.size() - 1; i++)
    {
        result += getMill(array[i],array[i+1],tdMap);
    }
    result += getMill(array[0],array[array.size() - 1],tdMap);
    return result;
}

int minKey(double key[], bool mstSet[])
{
    double min = INT_MAX;
    int min_index;

    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

void printMST(int parent[], double graph[V][V])
{
    double distance = 0;
    cout << "Edge  \tWeight\n";
    for (int i = 1; i < V; i++){
        distance  +=  graph[i][parent[i]];
        cout << parent[i] << " - " << i << " \t" << graph[i][parent[i]] << " \n";
    }
    cout<<"distance: "<<distance;
}

void primMST(double graph[V][V])
{
    int parent[V];
    double key[V];
    bool mstSet[V];

    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minKey(key, mstSet);

        mstSet[u] = true;

        for (int v = 0; v < V; v++)
            if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    printMST(parent, graph);
}

void getGraphFromMap(double graph[V][V], map<int, map<int, double>> tdMap){
    for (int i = 0; i < tdMap.size(); i++)
    {
        map<int,double> inMap = tdMap[i];
        for (int j = 0; j < inMap.size(); j++)
        {
            cout<<inMap[j]<<" ";
            graph[i][j] = inMap[j];
        }
        cout<<endl;
    }
}

int main()
{

    // double graph[V][V] = {{0, 2, 0, 6, 0},
    //                       {2, 0, 3, 8, 5},
    //                       {0, 3, 0, 0, 7},
    //                       {6, 8, 0, 0, 9},
    //                       {0, 5, 7, 9, 0}};
    cout<<"plase enter fileName!"<<endl;
    string fileName;
    cin>>fileName;
    map<int, map<int, double>> tourDistanceMap = getTourDistanceMap(fileName);
    double graph[V][V];
    getGraphFromMap(graph,tourDistanceMap);

    primMST(graph);

    return 0;
}