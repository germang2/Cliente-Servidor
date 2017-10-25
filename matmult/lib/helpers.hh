#include <iostream>
#include <vector>

using namespace std;

using Mat = vector<vector<int>>;

void write(const Mat &M, const char* fileName) {
  int rows = M.size();
  int cols = M[0].size();

  FILE *stream;
  int i, j;
  stream = fopen(fileName, "w");
  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++) {
      if (j + 1 == cols) fprintf(stream, "%d", M[i][j]);
      else fprintf(stream, "%d,", M[i][j]);
    }
    fprintf(stream, "%s\n","");
  }
  fclose(stream);
}

void print(const Mat &M) {
  for(int i = 0; i < M.size(); i++) {
    for (int j = 0; j < M[0].size(); j++) {
      cout << M[i][j] << " ";
    }
    cout << endl;
  }
}

void saveTime(long elapsedTime, string fileName){
  ofstream ofs(fileName, ios_base::app);
  ofs << elapsedTime << "\n" ;
  ofs.close();
}
