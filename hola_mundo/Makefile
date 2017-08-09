CC=g++ -std=c++11
ZMQ_PATH=/home/utp/zmq
ZMQ_INC=$(ZMQ_PATH)/include
ZMQ_LIB=$(ZMQ_PATH)/lib

all: hwclient hwserver

hwclient: hwclient.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) hwclient.cpp -o hwclient -lzmq -lzmqpp

hwserver: hwserver.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) hwserver.cpp -o hwserver -lzmq -lzmqpp
