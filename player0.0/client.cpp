#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <fstream>
#include <SFML/Audio.hpp>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <SFML/System/Time.hpp>

using namespace std;
using namespace sf;
using namespace zmqpp;

#ifndef SAFE_QUEUE
#define SAFE_QUEUE


// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q()
    , m()
    , c()
  {}

  ~SafeQueue(void)
  {}

  // Add an element to the queue.
  void enqueue(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T dequeue(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      // release lock as long as the wait and reaquire it afterwards.
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    return val;
  }

  bool isEmpty(void) 
  {
    return q.empty();
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};
#endif

void messageToFile(const message& msg, bool part){
	const void *data;
	msg.get(&data, 2); // the first is the name "file", the second is the parts, we don't need it here
	size_t size = msg.size(2);

	if (part) { // append
		ofstream ofs("current_song.ogg", ios::binary | ios_base::app);
		ofs.write((char*)data, size);
	} else { //rewrite
		ofstream ofs("current_song.ogg", ios::binary);
		ofs.write((char*)data, size);
	}
}

void songManager(Music *music, SafeQueue<string> &playList, bool &stop) {
	context ctx;
	socket s(ctx, socket_type::req);
	s.connect("tcp://localhost:5555");
	message m, n;
    string result, current_song = "";
    int parts = 0, current_part = 0;
    Clock clock;
    Time delta = seconds(5), elapsed, elapsed2, current_song_duration;

	while (true) {
		
		stop = false;
		string nextSong = playList.dequeue();
		current_song = nextSong;
		m << "play" << nextSong; // ask for the song
	    s.send(m);
	    s.receive(n);
	    n >> result;
	    n >> parts;


	    if (result == "file") {
	      cout << "nextSong : " << nextSong << endl;
		  messageToFile(n, false);
	      music->openFromFile("current_song.ogg");
	      music->play();
	      current_part = 0;
	      current_song_duration = music->getDuration();
		  elapsed = clock.restart();
		}
		
		while (music->getStatus() == SoundSource::Playing and !stop) {
			elapsed2 = clock.getElapsedTime(); // do not restart the clock
			if((elapsed2 >= current_song_duration - delta) and (current_part < parts - 1)){
				current_part++;
				m << "nextPart" << current_song << current_part;
				s.send(m);
				s.receive(n);
				n >> result;
				n >> parts;
				messageToFile(n, true);

				current_song_duration = music->getDuration();
				clock.restart();
			}
				
		}

	}
	cout << "Finished!" << endl;
}

int main(void) {
	cout << "This is the client" << endl;

	context ctx;
	socket s(ctx, socket_type::req);

	cout << "Connecting to tcp port 5555" << endl;
	s.connect("tcp://localhost:5555");

	//queue<string> playList;
	SafeQueue<string> playList;
	Music music;
	bool stop = false;
	thread t1(songManager, &music, ref(playList), ref(stop));

	while (true) {
		cout << "Enter operation" << endl;
		string operation;
		string songName = "";
		cin >> operation;

		message m;
		m << operation;

		if (operation == "add") {
			cin >> songName;
			m << songName;
		}
		if (operation == "exit") {
			return 0;
		}
		if (operation == "next") {
      if (!playList.isEmpty()) stop = true;
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

		} else if (result == "ok") { // if the song exists
			playList.enqueue(songName);
		}	else {
			cout << result << endl;
		}
	}

	return 0;
}
