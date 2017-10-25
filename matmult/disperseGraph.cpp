#include <iostream>
#include "graphreader3.hh"

using namespace std;

void printGraph(graph g){
	for (int i = 0; i < g.size(); ++i){
		for (int j = 0; j < g[i].size(); ++j){
			cout << "edge: " << i << "-" << g[i][j].first << ": " <<g[i][j].second << endl;
		}
	}
}

int main(int argc, char const *argv[])
{
	
	if(argc != 2){
		cout << "Miss the graph" << endl;
		exit(-1);
	}	
	string fileName(argv[1]);
	graph g = readGraph(fileName);

	cout << "No hubo errores :3" << endl;
	// print graph

	return 0;
}