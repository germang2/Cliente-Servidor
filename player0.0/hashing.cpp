#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <glob.h>
#include <vector>
#include <sstream>

using namespace std;

#define dbg(x) cout << #x << ": " << x << endl

vector<string> split(string s, char tok) { // split a string by a token especified
	istringstream ss(s);
  string token;
  vector<string> v;

  while(getline(ss, token, tok)) {
    v.push_back(token);
  }

  return v;
}

unordered_map<string,int> readFilesDirectory(string path){
	glob_t glob_result;
	glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);
	unordered_map<string,int> ans; // name of the song and its parts
	for(unsigned int i = 0; i < glob_result.gl_pathc; ++i){

		string file = string(glob_result.gl_pathv[i]); // eg: '../music/s1.0.ogg'
		//dbg(file);
		string tmp = file.erase(0, path.size() - 1); // eg: 's1.0.ogg'
		//dbg(tmp);
		vector<string> splited = split(tmp, '_'); // deletes the path of the string:
		//dbg(splited.size());
		string fileName = splited[0];
		//dbg(fileName);
		if (splited.size() == 2) { // splitted songs
			string cropNumber = splited[1];
			//dbg(cropNumber);

			if (ans.count(fileName) >= 1) { // if the song is already in the hash table
				ans[fileName] += 1; // counts how many parts a song has
			} else { // creates the entry for the hash
				ans[fileName] = 1;
			}
		}
	}
	globfree(&glob_result);
	return ans;
}

int main(int argc, char** argv) {

  string path(argv[1]);
  unordered_map<string,int> songs = readFilesDirectory(path + '*');

	cout << "Reading files in " << path << " : " << endl;

	for (const auto &file : songs) {
		cout << "first : " << file.first << ", elements : " << file.second << endl;
	}

  return 0;
}
