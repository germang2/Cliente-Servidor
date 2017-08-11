#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main(int argc, char** argv) {
	cout << "This is the client\n";

	if (argc != 2) {
		cerr << "Should be called: " << argv[0] << " <op> operand1 operand2\n";
		return 1;
	}

	context ctx;
	socket s(ctx, socket_type::req);//request

	cout << "Connecting to tcp port 5555\n";
	s.connect("tcp://192.168.8.66:5555");

	cout << "Sending  some work!\n";

	message m;
	string operation(argv[1]);

	m << operation;
	s.send(m);

	message answer;
	s.receive(answer);

	string op;
	answer >> op;

	if(op == "list"){
		size_t numSongs; //for save the number of songs with songs.size()
		answer >> numSongs;
		cout<<"Availbable songs: " << songs << endl;
		string song;
		for (int i = 0; i < numSongs; ++i){
			answer >> song;
			cout << song << endl;
		}

	}

    cout << "Finished\n";
	return 0;
}
