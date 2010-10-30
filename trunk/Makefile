CC = colorgcc
FLAGS = -Wall -I$(ROBOPET_PATH) -I$(COMMUNICATION_H) -I$(SOCKETS_PATH) -lstdc++ -lprotobuf -lQtCore -lpthread

#AI_PATH = ../IA/lib
ROBOPET_PATH = ../lib
ROBOPET_LIB = $(ROBOPET_PATH)/robopet.a

#PROTOBUF_PATH = ../Communication/SSL
COMMUNICATION_PATH = ../communication
COMMUNICATION_H = $(COMMUNICATION_PATH)/packets
COMMUNICATION_LIB = $(COMMUNICATION_PATH)/communication.a

VISION_SOCKETS_PATH = ./socket
VISION_PROTO_PATH = ./proto
VISION_PACKETS_PATH = ./packets
INC_PATHS = -I$(VISION_SOCKETS_PATH) -I$(VISION_PACKETS_PATH) -I/usr/include/qt4 -I/usr/include/qt4/QtCore -I$(COMMUNICATION_PATH)/socket -I$(COMMUNICATION_PATH)/packets

PROTOBUF_FILES_H = $(VISION_PACKETS_PATH)/messages_robocup_ssl_detection.pb.h \
				$(VISION_PACKETS_PATH)/messages_robocup_ssl_geometry.pb.h \
				$(VISION_PACKETS_PATH)/messages_robocup_ssl_refbox_log.pb.h \
				$(VISION_PACKETS_PATH)/messages_robocup_ssl_wrapper.pb.h

PROTOBUF_FILES_O = $(PROTOBUF_FILES_H:.h=.o)
PROTOBUF_FILES_CC = $(PROTOBUF_FILES_H:.h=.cc)

OBJECTS = $(PROTOBUF_FILES_O) $(VISION_SOCKETS_PATH)/robocup_ssl_client.o

SOCKETS_PATH = $(COMMUNICATION_PATH)/socket

all: $(OBJECTS) tracker

tracker: main.cpp tracker.o $(OBJECTS) $(ROBOPET_LIB) $(COMMUNICATION_LIB)
	@echo $@
	@$(CC) $(INC_PATHS) -o $@ $^ $(FLAGS) `pkg-config --cflags --libs protobuf`

tracker.o: tracker.cpp tracker.h
	@echo $@
	$(CC) $(INC_PATHS) -c -o $@ $< $(CFLAGS) $(LFLAGS)

$(VISION_SOCKETS_PATH)/robocup_ssl_client.o: $(VISION_SOCKETS_PATH)/robocup_ssl_client.cpp $(VISION_SOCKETS_PATH)/robocup_ssl_client.h
	$(CC) $(INC_PATHS) -c -o $@ $< $(CFLAGS) $(LFLAGS)

### PROTOBUF ###
$(VISION_PACKETS_PATH)/%.pb.o: $(VISION_PROTO_PATH)/%.proto
	protoc -I=$(VISION_PROTO_PATH) --cpp_out=$(VISION_PACKETS_PATH) $(VISION_PROTO_PATH)/$*.proto
	$(CC) $(INC_PATHS) -c -o $@ $(VISION_PACKETS_PATH)/$*.pb.cc $(CFLAGS) $(LFLAGS)
