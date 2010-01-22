#include <stdio.h>

#include "ssl_client.h"
#include "ssl_server.h"

RoboCupSSLServer trackertoai(PORT_TRACKER_TO_AI, IP_TRACKER_TO_AI);
RoboCupSSLClient simtotracker(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER),
				 aitotracker(PORT_AI_TO_TRACKER, IP_AI_TO_TRACKER);

int DEBUG = 1;

void receiveFromSim();
void receiveFromAI();

void receive()
{
	receiveFromSim();
	receiveFromAI();
}

void receiveFromSim()
{
	SSL_WrapperPacket packet;
	if (simtotracker.receive(packet) && packet.has_simtotracker()) {
		printf("----------------------------");
		printf("Received SIM-To-TRACKER!\n");
	}
}

void receiveFromAI()
{
	SSL_WrapperPacket packet;
	if (aitotracker.receive(packet) && packet.has_aitotracker()) {
		printf("----------------------------");
		printf("Received AI-To-TRACKER!\n");
	}
}

void send()
{
	 SSL_WrapperPacket packet;

	 TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
	 BallTracker *b = trackertoaiPacket->mutable_ball();

	 b->set_x(77);
	 b->set_y(666);

	 trackertoai.send(packet);
	printf("Sent Tracker-To-AI\n");
}

int main()
{
	printf("Tracker Running!\n");

	simtotracker.open(false);
	aitotracker.open(false);

	printf("Press <Enter> to open connection with client...\n");
	getchar();
	trackertoai.open();

	while(1) {
		receive();
		send();
	}
}
