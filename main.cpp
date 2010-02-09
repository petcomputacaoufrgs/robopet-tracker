#include <stdio.h>

#include "ssl_client.h"
#include "ssl_server.h"

RoboCupSSLServer trackertoai(PORT_TRACKER_TO_AI, IP_TRACKER_TO_AI);
RoboCupSSLClient simtotracker(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER),
				 aitotracker(PORT_AI_TO_TRACKER, IP_AI_TO_TRACKER),
				 radiototracker(PORT_RADIO_TO_TRACKER, IP_RADIO_TO_TRACKER);

int DEBUG = 1, USING_SSL = 0;

SimToTracker data;

void receiveFromSim();
void receiveFromAI();
void receiveFromRadio();
void receiveFromSSL();

void receive()
{
	receiveFromSim();
	receiveFromAI();
	receiveFromRadio();
	receiveFromSSL();
}

void receiveFromSim()
{
	SSL_WrapperPacket packet;
	if (simtotracker.receive(packet) && packet.has_simtotracker()) {
		//printf("----------------------------");
		//printf("Received SIM-To-TRACKER!\n");

		data = packet.simtotracker();
	}
}

void receiveFromAI()
{
	SSL_WrapperPacket packet;
	if (aitotracker.receive(packet) && packet.has_aitotracker()) {
		//printf("----------------------------");
		//printf("Received AI-To-TRACKER!\n");
	}
}

void receiveFromRadio()
{
	SSL_WrapperPacket packet;
	if (radiototracker.receive(packet) && packet.has_radiototracker()) {
		//printf("----------------------------");
		//printf("Received Radio-To-TRACKER!\n");
	}
}

void receiveFromSSL()
{
	//TODO: Vision
}

void send()
{
	if(!USING_SSL) {
		 SSL_WrapperPacket packet;

		 TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
		 BallTracker *b = trackertoaiPacket->mutable_ball();

		//Identity Function Sim -> Tracker -> IA
		 b->set_x(data.ball().x());
		 b->set_y(data.ball().y());

		for(int i = 0; i < data.robots_blue_size(); i++) {
			RobotTracker *r = trackertoaiPacket->add_robots_blue();
			r->set_x(data.robots_blue(i).x());
			r->set_y(data.robots_blue(i).y());
			r->set_theta(data.robots_blue(i).theta());

			printf("cur_pos(%f, %f)\n", r->x(), r->y());
		}

		for(int i = 0; i < data.robots_yellow_size(); i++) {
			RobotTracker *r = trackertoaiPacket->add_robots_yellow();
			r->set_x(data.robots_yellow(i).x());
			r->set_y(data.robots_yellow(i).y());
			r->set_theta(data.robots_yellow(i).theta());

		}

		 trackertoai.send(packet);
		printf("Sent Tracker-To-AI\n");
		printf("ball (%f, %f)\n", data.ball().x(), data.ball().y());
		debug_int(data.robots_blue_size());
		debug_int(data.robots_yellow_size());
	} else {
		//TODO: Vision
	}
}

int main()
{
	printf("Tracker Running!\n");

	simtotracker.open(false);
	aitotracker.open(false);
	radiototracker.open(false);

	printf("Press <Enter> to open connection with client...\n");
	getchar();
	trackertoai.open();

	clrscr();
	while(1) {
		rewindscr();
		receive();
		send();
	}
}
