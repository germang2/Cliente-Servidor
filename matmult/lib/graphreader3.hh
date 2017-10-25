#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "timer.hh"

using namespace std;
using graph = vector<vector<pair<int,int>>>;
//using Mat = vector<vector<int>>;


vector<vector<pair<int,int>>> readGraph(string fileName){
    graph g;
    ifstream infile(fileName);

    string line;
    while(getline(infile, line)){
        istringstream iss(line);
        // Resize graph por n nodes, this resize is only for the first vector
        // that allocate all the nodes
        if (line[0] == 'p'){
            string s1, s2;
            int nodes;
            iss >> s1 >> s2 >> nodes;
            g.resize(nodes);
            cout << "Graph with " << nodes << " nodes" << endl;
        } else if (line[0] == 'a'){ //For all edges
            char e;
            int s, t, w;
            pair <int,int> edge;
            // s is the first node, t the second and w the weight
            if(s == t)
                continue;
            iss >> e >> s >> t >> w;
            edge.first = t - 1;
            edge.second = w;
            g[ s - 1].push_back(edge);
        }
    }
    return g;
}
