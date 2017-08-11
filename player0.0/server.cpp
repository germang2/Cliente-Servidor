#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>

using namespace std;
using namespace zmqpp;

int main(int args, char** argv) {
  cout << "This is the server\n";

  context ctx;
  socket s(ctx, socket_type::rep); //rep = replay

  string path(argv[1]);
  unordered_map<string,string> songs; //hashtable with song's list
  songs["s1"] = path + "s1.ogg";
  songs["s2"] = path + "s2.ogg";
  songs["s3"] = path + "s3.ogg";



  cout << "Binding socket to tcp port 5555\n";
  s.bind("tcp://*:5555");

  cout << "Start serving requests!\n";

  while(true) {
    message m;
    cout << "HOLA 11111111111111111" << endl;
    s.receive(m);

    string op; //this is the command to execute
    m >> op;

    cout <<"Action: " << op <<endl;

    if(op == "list"){
      // Use case 1: send song's list
      message n;
      n << "list" << songs.size();
      for (const auto& p : songs){
        n << p.first
      }
      s.send(n);
      
    } else if(op == "play"){
      //  Use case 2: send song file
    }else{
      cout << "Invalid operation requested!" << endl;
    }

    message response;
    response << answer;
    s.send(response);
  }

  cout << "Finished\n";
  return 0;
}
