#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <glob.h>
#include <vector>
#include <sstream>

using namespace std;
using namespace zmqpp;

vector<char> readFileToBytes(const string& fileName) {
	ifstream ifs(fileName, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	vector<char> result(pos);

	ifs.seekg(0, ios::beg);
	ifs.read(result.data(), pos);

	return result;
}

void fileToMesage(const string& fileName, message& msg) {
	vector<char> bytes = readFileToBytes(fileName);
	msg.add_raw(bytes.data(), bytes.size());
}

vector<string> split(string s, char tok) { // split a string by a token especified
	istringstream ss(s);
  string token;
  vector<string> v;

  while(getline(ss, token, tok)) {
    v.push_back(token);
  }

  return v;
}

unordered_map<string,string> readFilesDirectory(string path){
    glob_t glob_result;
    glob(path.c_str(), GLOB_TILDE, NULL, &glob_result);
    unordered_map<string,string> ans;
    for(unsigned int i = 0; i < glob_result.gl_pathc; ++i){

			string file = string(glob_result.gl_pathv[i]);
			vector<string> splited = split(file.erase(0, path.size() - 1), '.'); // deletes the path of the string
			string fileName = splited[0];
			string fileExtension = splited[1]; // get the extension

			if (fileExtension == "ogg") { // if is ogg
				ans[fileName] = path.substr(0, path.size()-1) + file; // removes the '*' from the path
			}
    }
    globfree(&glob_result);
    return ans;
}

int main(int argc, char** argv) {
  context ctx;
  socket s(ctx, socket_type::rep);
  s.bind("tcp://*:5555");

  string path(argv[1]);
  unordered_map<string,string> songs = readFilesDirectory(path + '*');

	cout << "Reading files in " << path << " : " << endl;

	for (const auto &file : songs) {
		cout << "first : " << file.first << ", second : " << file.second << endl;
	}

  cout << "Start serving requests!" << endl;
  while(true) {
    message m;
    s.receive(m);

    string op;
    m >> op;

    cout << "Action:  " << op << endl;
    if (op == "list") {  // Use case 1: Send the songs
      message n;
      n << "list" << songs.size();
      for(const auto& p : songs)
        n << p.first;
      s.send(n);
    } else if(op == "play") {
      // Use case 2: Send song file
      string songName;
      m >> songName;
      cout << "sending song " << songName
           << " at " << songs[songName] << endl;
			message n;
			n << "file";
			fileToMesage(songs[songName], n);
			s.send(n);
    } else {
      cout << "Invalid operation requested!!" << endl;
    }
  }

  cout << "Finished" << endl;
  return 0;
}
