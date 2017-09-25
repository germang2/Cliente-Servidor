#include <iostream>
#include "graphreader2.hh"
#include <string>
#include <cassert>
#include <fstream>

using namespace std;
using Mat = vector<vector<int>>;

void saveTime(long elapsedTime){
    ofstream ofs("time0.out", ios_base::app);
    ofs << elapsedTime << "\n" ;
    ofs.close();
}


vector<int> vectorAdds(const Mat &m1, const Mat &m2, int a, int b){
    vector<int> aux;
    aux.resize(m1[0].size());
    int infinite = numeric_limits<int>::max();
    int i = 0;
    for(int c = 0; c < b; c++){
        if(m1[a][c] != infinite && m2[a][c] != infinite)
            aux[i] += m1[a][c] + m2[c][b]; 
        i++;
    }

    return aux;

}

int min(vector<int> vec){
    int m = numeric_limits<int>::max();

    for(int i = 0; i < vec.size(); i++){
        if(vec[i] < m)
            m = vec[i];
    }

    return m;
}

void mult3_0(const Mat &m1, const Mat &m2, Mat &res){
    int i = m1.size();      //number of rows in m1
    int j = m1[0].size();   //number of cols in m1
    int k = m2.size();      //number of rows in m2
    int l = m2[0].size();   //number of cols in m2

    vector<int> vec;
    vec.resize(m1[0].size());

    for(int a = 0; a < i; a++){
        for(int b = 0; b < l; b++){
            vec = vectorAdds(m1, m2, a, b);
            res[a][b] = min(vec);

        }
    }

}

void printMat(const Mat &res){
    for(int i = 0; i < res.size(); i++){
        for(int j = 0; j < res[0].size(); j++){
            cout << res[i][j] << " ";
        }
        cout << "\n";
    }
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
        Timer t("mult3_0");
        mult3_0(g, g, res);
        long elapsedTime = t.elapsed();
        cout << elapsedTime << " ms." << endl;
        saveTime(elapsedTime);        
    }

    printMat(res);

    return 0;
}
