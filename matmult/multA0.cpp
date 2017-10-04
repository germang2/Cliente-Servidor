#include <iostream>
#include "graphreader2.hh"
#include <string>
#include <cassert>
#include <fstream>
#include <math.h>

using namespace std;
using Mat = vector<vector<int>>;

void saveTime(long elapsedTime){
    ofstream ofs("time0.out", ios_base::app);
    ofs << elapsedTime << "\n" ;
    ofs.close();
}

void multA0(const Mat &m1, const Mat &m2, Mat &res){
    int i = m1.size();      //number of rows in m1
    int j = m1[0].size();   //number of cols in m1
    int k = m2.size();      //number of rows in m2
    int l = m2[0].size();   //number of cols in m2
    int infinite = numeric_limits<int>::max();

    vector<int> vec;
    vec.resize(m1[0].size());

    for(int a = 0; a < i; a++){
        for(int b = 0; b < l; b++){
            for(int c = 0; c < l; c++){
                if(m1[a][c] != infinite && m2[c][b] != infinite){
                    res[a][b] = min(res[a][b], m1[a][c] + m2[c][b]);
                    //cout << m1[a][b] + m2[a][b] << " ";
                }
            }
        }
    }

}

void printMat(const Mat &res){
    int infinite = numeric_limits<int>::max();
    for(int i = 0; i < res.size(); i++){
        for(int j = 0; j < res[0].size(); j++){
            if(res[i][j] == infinite)
                cout <<  "  F ";
            else
                cout <<  "  " << res[i][j] << " ";
        }
        cout << "\n";
    }
}

int main(int argc, char **argv){
    if(argc != 2){
        cerr << "Error!!" << endl;
    }
    int infinite = numeric_limits<int>::max();
    string fileName(argv[1]);
    Mat g = readGraph(fileName);
    //printMat(g);
    Mat res;
    res.resize(g.size());
    for(int i = 0; i < g.size() ; i++){
        res[i].resize(g.size());
    }

    for(int i = 0; i < res.size(); i++)
        for(int j =0; j < res[i].size(); j++)
            res[i][j] = infinite;

    {//lack to take the time of execution
        Timer t("multA0");
        multA0(g, g, res);
        long elapsedTime = t.elapsed();
        cout << elapsedTime << " ms." << endl;
        saveTime(elapsedTime);        
    }

    printMat(res);

    return 0;
}
