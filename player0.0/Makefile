CC=g++ -std=c++11
#In my portatil
ZMQ_PATH=/usr/local
#In university pc
#ZMQ_PATH=/home/utp/zmq
ZMQ_INC=$(ZMQ_PATH)/include
ZMQ_LIB=$(ZMQ_PATH)/lib

all: client server

hwclient: client.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) client.cpp -o client -lzmq -lzmqpp

hwserver: server.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) server.cpp -o server -lzmq -lzmqpp
