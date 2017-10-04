#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "timer.hh"

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
                // g[i].push_back(numeric_limits<int>::max());
            }
            for(int i = 0; i< g.size() ; i++)
                for(int j = 0; j < g[i].size(); j++)
                    if(i == j)
                        g[i][j] = 0;
                    else        
                        g[i][j] = numeric_limits<int>::max();
            cout << "Graph with " << nodes << " nodes" << endl;
        } else if (line[0] == 'e'){
            char e;
            int s, t, w;
            iss >> e >> s >> t >> w;
            //cout << s << " - " << t << " : " << w << endl;           
            g[ s - 1][t - 1] = w;
            g[ t - 1][s - 1] = w;                
        }
    }
    return g;
}
