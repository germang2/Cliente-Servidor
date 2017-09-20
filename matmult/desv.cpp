#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <math.h>

using namespace std;

long double variance(vector<long> data) {
  long long ans;
  long long sum = 0;
  long double average;
  for (auto v : data ) {
    sum += v;
  }
  average = sum / data.size();
  cout << "size: " << data.size() << endl;
  cout << "sum: " << sum << endl;
  cout << "average: " << average << endl;

  sum = 0;
  for (auto v : data ) {
    sum += pow((v - average),2);
  }
  ans = sum / data.size();
  return ans;
}

long toLong(string &s) {
  stringstream ss;
  ss << s;
  long out;
  ss >> out;
  return out;
}

vector<long> read(string fileName) {
  vector<long> data;
  string line;
  ifstream myfile(fileName);
  if (myfile.is_open()) {
    while ( getline(myfile,line) ) {
      data.push_back(toLong(line));
    }
    myfile.close();
  }
  return data;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: fileName" << endl;
  }
  string fileName(argv[1]);
  vector<long> data = read(fileName);

  for (auto v : data ) {
    cout << v << endl;
  }

  long double var = variance(data);
  long double desvest = sqrt(var);
  cout << "Desvest: " << desvest << endl;
  return 0;
}
