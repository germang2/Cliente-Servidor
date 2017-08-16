#include <iostream>
#include <fstream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <SFML/Audio.hpp>
#include <thread>

using namespace std;
using namespace zmqpp;
using namespace sf;

void messageToFile(const message& msg, const string& fileName) {
	const void *data;
	msg.get(&data, 1);
	size_t size = msg.size(1);

	ofstream ofs(fileName, ios::binary);
	ofs.write((char*)data, size);
}

void playSong(){
	Music music;
	music.openFromFile("song.ogg");
	music.play();
	while(music.getStatus() == SoundSource::Playing){
		cout<<"Play Song"<<endl;
	}
}

int main(int argc, char** argv) {
	cout << "This is the client\n";

	context ctx;
	socket s(ctx, socket_type::req);
	cout << "Connecting to tcp port 5555\n";
	s.connect("tcp://localhost:5555");

	while(true){

		Music music;

		cout << "Sending  some work!\n";

		cout<<"Write operation:\nlist\nplay song\n";

		message m;
		string operation;
		cin>>operation;
		m << operation;


		if(operation == "play") {
			string file;
			cin>>file;
			m << file;
		}

		s.send(m);

		message answer;
		s.receive(answer);

		string result;
		answer >> result;
		if (result == "list") {
			size_t numSongs;
			answer >> numSongs;
			cout << "Available songs: " << numSongs << endl;
			for(int i = 0; i < numSongs; i++) {
				string s;
				answer >> s;
				cout << s << endl;
			}

		} else if (result == "file"){
			messageToFile(answer,"song.ogg");
			//Launch a thread
			thread t1(playSong);
			//Join the thread with the main threadt

		} else {
			cout << "Don't know what to do!!!" << endl;
		}
	}

	return 0;
}
