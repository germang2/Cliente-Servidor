#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <condition_variable>
#include <mutex>

using namespace std;

struct SafeGraph {

  unordered_map<int, map<int, int>> m;
  int nodes;
  recursive_mutex mut;
  condition_variable data_cond;

  SafeGraph() {
    nodes = 0;
  }

  SafeGraph(unordered_map<int, map<int, int>> another) {
    m = another;
  }

  void insert(int u, int v, int w) {
    lock_guard<recursive_mutex> lk(mut);
    if (exists(u)) m[u][v] = w;
    else {
      map<int,int> m2;
      m2[v] = w;
      m[u] = m2;
    }
    data_cond.notify_one();
  }

  void setNodes(int n) {
    nodes = n;
  }

  int getNodes() {
    lock_guard<recursive_mutex> lk(mut);
    return nodes;
  }

  map<int,int> getMap(int key) {
    lock_guard<recursive_mutex> lk(mut);
    return m[key];
  }

  void readGraph(string fileName) {
    ifstream infile(fileName);
    string line;
    while (getline(infile, line)) {
      istringstream iss(line);
      if (line[0] == 'p') {
        string s1, s2;
        int n, edges;
        iss >> s1 >> s2 >> n >> edges;
        setNodes(n);
        cout << "Graph with " << nodes << " nodes" << endl;
      } else if (line[0] == 'a') {
        char e;
        int u, v, w;
        iss >> e >> u >> v >> w;
        insert(u - 1, v - 1, w);
      }
    }
  }

  void fillDiagonals(int nodes) {
    for (int i = 0; i < nodes; i++) insert(i, i, 0);
  }

  bool exists(int key) {
    lock_guard<recursive_mutex> lk(mut);
    return (m.count(key) > 0);
  }

  bool exists(int u_key, int v_key) {
    lock_guard<recursive_mutex> lk(mut);
    if (exists(u_key)) return (m[u_key].count(v_key) > 0);
    else return false;
  }

  void clear() {
    // for (int i = 0; i < m.size(); i++) {
    //   m[i].clear();
    // }
    m.clear();
  }

  int getValue(int u_key, int v_key) {
    lock_guard<recursive_mutex> lk(mut);
    return m[u_key][v_key];
  }

  int size() {
    lock_guard<recursive_mutex> lk(mut);
    return m.size();
  }

  int size(int key) {
    lock_guard<recursive_mutex> lk(mut);
    return m[key].size();
  }

  void print() {
    for (auto& u : m) {
      cout << u.first << " -> ";
      for (auto& edge : u.second) {
        cout << " [" << edge.first << " : " << edge.second << "]";
      }
      cout << endl;
    }
  }
};
