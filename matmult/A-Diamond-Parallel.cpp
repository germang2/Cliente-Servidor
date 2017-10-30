#include "lib/SafeGraph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include "lib/thread_pool.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <string.h>
#define INF numeric_limits<int>::max()
#define dbg(x) cout << #x << ": " << x << endl
unsigned const cpu_threads = std::thread::hardware_concurrency();

void dot(SafeGraph &m1, SafeGraph &m2, SafeGraph &res, int &min, int v, const map<int,int> &map, int nodes) {
  int value;
  for (int i = 0; i <= nodes; i++) { // cols m2
    int min = INF;
    for (auto& neighs : map) { // cols m1
      if (m2.m[i][neighs.first]) {
        value = m1.getValue(v, neighs.first) + m2.getValue(neighs.first, i);
        // dbg(value);
        if (value < min) min = value;
      }
    }
    if (min != INF) res.insert(v, i, min);
  }
}

int parallelMult(int p, SafeGraph &original, SafeGraph &current, SafeGraph &res) {
  const int nodes = p;
  int min = INF, workers = 0;
  thread_pool pool;
  while (p > 0) {
    dbg(p);
    cout << "--------------" << endl;
    if (p % 2 == 0) {
      for (auto& v : current.m) { // rows m1
        pool.submit( //
        [&current, &res, &min, &v, nodes]() { dot(ref(current), ref(current), ref(res), ref(min), v.first, cref(v.second), nodes); });
      }
    } else {
      for (auto& v : original.m) { // rows m1
        pool.submit( //
        [&original, &current, &res, &min, &v, nodes]() { dot(ref(original), ref(current), ref(res), ref(min), v.first, cref(v.second), nodes); });
      }
    }
    pool.barrier(nodes);
    workers += pool.getWorkersCount();
    current.m = res.m;
    cout << "Current" << endl;
    current.print();
    res.clear();
    p /= 2;
  }
  return 8;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Error!!" << endl;
    return 1;
  }
  string fileName(argv[1]);
  string fileNameTime(argv[2]);
  SafeGraph g, r, original;
  int workers;
  original.readGraph(fileName);
  original.fillDiagonals(original.getNodes());
  original.print(); cout << endl;
  g.readGraph(fileName);
  g.fillDiagonals(g.getNodes());

  cout << "---------Initial Graph----------" << endl;
  g.print();

  Timer t("mult6");

  // Parallel implementation
  workers = parallelMult(g.getNodes(), original, g, r);
  dbg(workers);
  cout << "---------Final Graph (Parallel)----------" << endl;
  g.print();

  saveTime(t.elapsed(), fileNameTime, workers);
  return 0;
}
