#include <SFML/Audio.hpp>
#include <iostream>
#define endl '\n'

using namespace std;
using namespace sf;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Must be called: " << argv[0] << " file.ogg" << endl;
    return 1;
  }
  cout << "Simple player!" << endl;
  string fileToPlay(argv[1]);

  Music music; // creates the object
  if (!music.openFromFile(fileToPlay)) {
    cerr << "File not found or error" << endl;
    return 1;
  }
  music.play();

  int x;
  cin >> x;
  return 0;
}