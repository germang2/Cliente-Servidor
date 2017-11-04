#include "lib/SafeGraph.hh"
#include "lib/timer.hh"
#include "lib/helpers.hh"
#include "lib/thread_pool.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <map>
#include <string.h>
#define INF numeric_limits<int>::max()
unsigned const cpu_threads = std::thread::hardware_concurrency();
#define dbg(x) cout << #x << ": " << x << endl

using namespace std;

void dot(SafeGraph &m1, SafeGraph &m2, SafeGraph &res, int v, const int nodes) {
  int value;
  for (int i = 0; i <= nodes; i++) { // cols m2
    int min = INF;
    for (auto& neighs : m1.getMap(v)) { // cols m1
      if (m2.exists(i, neighs.first)) {
        value = m1.getValue(v, neighs.first) + m2.getValue(neighs.first, i);
        // dbg(value);
        if (value < min) min = value;
      }
    }
    if (min != INF) res.insert(v, i, min);
  }
}

int parallelMult2(int p, SafeGraph &original, SafeGraph &current, SafeGraph &res) {
  const int nodes = p;
  int chunks = 0, cpu = 0, index = 0;
  bool flag = false;
  dbg(nodes);
  if (nodes < cpu_threads) {
    chunks = 1;
    cpu = nodes;
  }
  else {
    cpu = cpu_threads;
    chunks = nodes / cpu_threads;
    if (nodes % cpu_threads != 0) flag = true;
  }
  dbg(chunks);
  dbg(cpu);
  while (p > 0) {
    vector<thread> ts;
    dbg(p);
    cout << "--------------" << endl;
    int i = 0;

    for (; i < chunks; i++) {
      dbg(i); dbg(chunks);
      for (int k = 0; k < cpu; k++) {
        dbg(k);
        index = k + cpu*i;
        dbg(index);
        if (p % 2 == 0) ts.push_back(thread(dot, ref(current), ref(current), ref(res), index, nodes));
        else ts.push_back(thread(dot, ref(original), ref(current), ref(res), index, nodes));
      }
      for (thread &t : ts) t.join();
    }
    if (flag) {
      dbg(flag);
      chunks = nodes % cpu_threads;
      dbg(chunks);
      for (int l = 0; l < chunks; l++) {
        index += l;
        dbg(index);
        if (p % 2 == 0) ts.push_back(thread(dot, ref(current), ref(current), ref(res), index, nodes));
        else ts.push_back(thread(dot, ref(original), ref(current), ref(res), index, nodes));
      }
      for (thread &t : ts) t.join();
    }

    current.m = res.m;
    cout << "Current" << endl;
    current.print();
    res.clear();
    p /= 2;
    // return -1;
  }
  return (nodes < cpu_threads ? nodes : cpu_threads);
}

int parallelMult(int p, SafeGraph &original, SafeGraph &current, SafeGraph &res) {
  const int nodes = p;
  int workers = 0;
  thread_pool pool;
  while (p > 0) {
    dbg(p);
    cout << "--------------" << endl;

    for (auto& v : current.m) { // rows m1
      if (p % 2 == 0) pool.submit( //
      [&current, &res, &v, nodes]() { dot(ref(current), ref(current), ref(res), v.first, nodes); });
      else pool.submit( //
      [&original, &current, &res, &v, nodes]() { dot(ref(original), ref(current), ref(res), v.first, nodes); });
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

  g.readGraph(fileName);
  g.fillDiagonals(g.getNodes());

  cout << "---------Initial Graph----------" << endl;
  original.print();

  Timer t("mult6");

  // Parallel implementation
  workers = parallelMult2(g.getNodes(), original, g, r);
  dbg(workers);
  cout << "---------Final Graph (Parallel)----------" << endl;
  g.print();

  saveTime(t.elapsed(), fileNameTime, workers);
  return 0;
}
