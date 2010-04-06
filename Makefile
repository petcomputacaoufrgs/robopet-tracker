CC = colorgcc
FLAGS = -Wall -I$(ROBOPET_PATH) -I$(COMMUNICATION_H) -I$(SOCKETS_PATH) -lstdc++ -lprotobuf

#AI_PATH = ../IA/lib
ROBOPET_PATH = ../lib
ROBOPET_LIB = $(ROBOPET_PATH)/robopet.a #$(ROBOPET_PATH)/point.o $(AI_PATH)/vector.o $(AI_PATH)/movingObject.o

#PROTOBUF_PATH = ../Communication/SSL
COMMUNICATION_PATH = ../robopet-communication/
COMMUNICATION_H = $(COMMUNICATION_PATH)/packets
COMMUNICATION_LIB = $(COMMUNICATION_PATH)/communication.a

SOCKETS_PATH = $(COMMUNICATION_PATH)/socket

all: tracker

tracker: main.cpp $(ROBOPET_LIB) $(COMMUNICATION_LIB)
	@echo $@
	@$(CC) -o $@ $^ $(FLAGS) `pkg-config --cflags --libs protobuf`
