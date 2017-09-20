#include <iostream>
#include "graphreader.hh"
#include <string>
#include <cassert>
#include <thread>
#include <vector>

using namespace std;
using Mat = vector<vector<int>>;

void saveTime(long elapsedTime){
    ofstream ofs("time1.out", ios_base::app);
    ofs << elapsedTime << "\n" ;
    ofs.close();
}

void dot(const Mat &m1, const Mat &m2, int a, int b, int &res){
    int j = m1[0].size();   //number of cols in m1
    for(int c = 0; c < j; c++)
        res += m1[a][c] * m2[c][b];
}

void mult1(const Mat &m1, const Mat &m2, Mat &res){
    int i = m1.size();      //number of rows in m1
    int j = m1[0].size();   //number of cols in m1
    int k = m2.size();      //number of rows in m2
    int l = m2[0].size();   //number of cols in m2

    assert(j == k);

    vector<thread> ts;

    for(int a = 0; a < i; a++){
        for(int b = 0; b < l; b++){
            ts.push_back(thread(dot, cref(m1), cref(m2), a, b, ref(res[a][b])));
        }
    }
    for(thread &t : ts)
        t.join();
}

int main(int argc, char **argv){
    if(argc != 2){
        cerr << "Error!!" << endl;
    }
    string fileName(argv[1]);
    Mat g = readGraph(fileName);
    Mat res;
    res.resize(g.size());
    for(int i = 0; i < g.size() ; i++){
        res[i].resize(g[0].size());
    }

    {//lack to take the time of execution
        Timer t("mult1");
        mult1(g, g, res);
        long elapsedTime = t.elapsed();
        cout << elapsedTime << " ms." << endl;
        saveTime(elapsedTime);        
    }

    return 0;
}
