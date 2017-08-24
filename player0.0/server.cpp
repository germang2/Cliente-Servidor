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
  cout << "DEBUG 1, fileName:" << fileName <<endl;
  ifstream ifs("music/" + fileName, ios::binary | ios::ate);
  if(ifs.is_open())
    cout << "DEBUG 2" << endl;
  else
    cout << "ERROR OPENING FILE" << endl;

  ifstream::pos_type pos = ifs.tellg();

  cout << "DEBUG 3: " << pos <<endl;
  
  vector<char> result(pos);
  cout << "DEBUG 4" << endl;

  ifs.seekg(0, ios::beg);
  ifs.read(result.data(), pos);

  ifs.close();

  return result;
}

void fileToMesage(const string& fileName, message& msg, int part) {
  vector<char> bytes = readFileToBytes(fileName + "_0" + to_string(part));
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

//Reads the songs and counts how many parts have
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
  context ctx;
  socket s(ctx, socket_type::rep);
  s.bind("tcp://*:5555");

  string path(argv[1]);
  unordered_map<string,int> songs = readFilesDirectory(path + '*');

  cout << "Reading files in " << path << " : " << endl;

  for (const auto &file : songs) {
    cout << "first : " << file.first << ", parts : " << file.second << endl;
  }


  cout << "Start serving requests!" << endl;
  while(true) {
    message m;
    message n; // answer from the client
    s.receive(m);

    string op;
    m >> op;

    cout << "Action:  " << op << endl;
    if (op == "list") {  // Use case 1: Send the songs
      n << "list" << songs.size();
      for(const auto& p : songs)
        n << p.first;

    } else if(op == "play") { // Use case 2: Send song file
      string songName;
      m >> songName;
      cout << "sending song " << songName << endl; //" at " << songs[songName] << endl;
      n << "file";
      fileToMesage(songName, n, 0);
    } else if (op == "add") {
      string songName;
      m >> songName;

      if (songs.count(songName) != 1) { // if doesn't exists
        cout << "No song with the name " + songName << endl;
        n << "No song with the name " + songName;
      } else {
        n << "ok";
      }
    } else if (op == "next") {
      n << "next";
    } else {
      n << "Invalid operation requested!!";
    }
    s.send(n);
  }

  cout << "Finished" << endl;
  return 0;
}
