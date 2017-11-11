#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include <string>

using namespace std;
using namespace zmqpp;

class node
{
private:
	int myid;
	int sucessor;
	int left; //the range goes since left to myid 
public:
	node(int id){
		myid = id;
	}
	
};
