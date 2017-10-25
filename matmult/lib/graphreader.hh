#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
using Mat = vector<vector<int>>;


vector<vector<int>> readGraph(string fileName){
    Mat g;
    ifstream infile(fileName);

    string line;
    while(getline(infile, line)){
        istringstream iss(line);
        if (line[0] == 'p'){
            string s1, s2;
            int nodes;
            iss >> s1 >> s2 >> nodes;
            g.resize(nodes);
            for(int i = 0; i < nodes ; i++){
                g[i].resize(nodes);
            }
            cout << "Graph with " << nodes << " nodes" << endl;
        } else if (line[0] == 'e'){
            char e;
            int s, t, w;
            iss >> e >> s >> t;
            g[ s - 1][t - 1] = 1;
        }
    }
    return g;
}
