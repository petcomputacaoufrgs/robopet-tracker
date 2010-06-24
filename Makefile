CC = colorgcc
FLAGS = -Wall -I$(ROBOPET_PATH) -I$(COMMUNICATION_H) -I$(SOCKETS_PATH) -lstdc++ -lprotobuf

#AI_PATH = ../IA/lib
ROBOPET_PATH = ../lib
ROBOPET_LIB = $(ROBOPET_PATH)/robopet.a #$(ROBOPET_PATH)/point.o $(AI_PATH)/vector.o $(AI_PATH)/movingObject.o

#PROTOBUF_PATH = ../Communication/SSL
COMMUNICATION_PATH = ../robopet-communication/
COMMUNICATION_H = $(COMMUNICATION_PATH)/packets
COMMUNICATION_LIB = $(COMMUNICATION_PATH)/communication.a

VISION_SOCKETS_PATH = ./socket
VISION_PROTO_PATH = ./proto
INC_PATHS = -I$(VISION_SOCKETS_PATH) -I$(VISION_PROTO_PATH) -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I../robopet-communication/socket

OBJECTS = $(VISION_SOCKETS_PATH)/robocup_ssl_client.o ../robopet-communication/udp_netraw.o

PROTOBUF_FILES_H = $(VISION_PROTO_PATH)/messages_robocup_ssl_detection.pb.h \
				$(VISION_PROTO_PATH)/messages_robocup_ssl_geometry.pb.h \
				$(VISION_PROTO_PATH)/messages_robocup_ssl_refbox_log.pb.h \
				$(VISION_PROTO_PATH)/messages_robocup_ssl_wrapper.pb.h

PROTOBUF_FILES_O = $(PROTOBUF_FILES_H:.h=.o)
PROTOBUF_FILES_CC = $(PROTOBUF_FILES_H:.h=.cc)

SOCKETS_PATH = $(COMMUNICATION_PATH)/socket

all: tracker

tracker: main.cpp $(ROBOPET_LIB) $(PROTOBUF_FILES_O) $(OBJECTS) $(COMMUNICATION_LIB)
	@echo $@
	@$(CC) $(INC_PATHS) -o $@ $^ $(FLAGS) -lQtCore -lpthread `pkg-config --cflags --libs protobuf`

$(VISION_SOCKETS_PATH)/robocup_ssl_client.o: $(VISION_SOCKETS_PATH)/robocup_ssl_client.cpp $(VISION_SOCKETS_PATH)/robocup_ssl_client.h
	$(CC) $(INC_PATHS) -c -o $@ $< $(CFLAGS) $(LFLAGS)

### PROTOBUF ###
$(VISION_PROTO_PATH)/%.pb.o:
	$(CC) $(INC_PATHS) -c $(VISION_PROTO_PATH)/$*.pb.cc $(CFLAGS) $(LFLAGS)
