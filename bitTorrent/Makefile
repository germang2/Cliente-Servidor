CC=g++ -std=c++11
#In portatil
ZMQ_PATH=/usr/local
#In university pc
#ZMQ_PATH=/home/utp/zmq
ZMQ_INC=$(ZMQ_PATH)/include
ZMQ_LIB=$(ZMQ_PATH)/lib
LIBS=-lzmq -lzmqpp

all: ring

ring: ring.cpp
	$(CC) -L$(ZMQ_LIB) -I$(ZMQ_INC) ring.cpp -o ring.out $(LIBS)