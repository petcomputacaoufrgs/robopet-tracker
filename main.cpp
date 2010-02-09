#include <stdio.h>

#include "ssl_client.h"
#include "ssl_server.h"

RoboCupSSLServer trackertoai(PORT_TRACKER_TO_AI, IP_TRACKER_TO_AI);
RoboCupSSLClient simtotracker(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER),
				 aitotracker(PORT_AI_TO_TRACKER, IP_AI_TO_TRACKER),
				 radiototracker(PORT_RADIO_TO_TRACKER, IP_RADIO_TO_TRACKER),
				 visiontotracker(PORT_VISION_TO_TRACKER, IP_VISION_TO_TRACKER);

int DEBUG = 1, USING_VISION = 1;

SimToTracker dataSim;
String dataVision;

void receiveFromSim();
void receiveFromAI();
void receiveFromRadio();
void receiveFromVision();

void receive()
{
	receiveFromSim();
	receiveFromAI();
	receiveFromRadio();
	receiveFromVision();
}

void receiveFromSim()
{
	SSL_WrapperPacket packet;
	if (simtotracker.receive(packet) && packet.has_simtotracker()) {
		//printf("----------------------------");
		//printf("Received SIM-To-TRACKER!\n");

		dataSim = packet.simtotracker();
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

void receiveFromVision()
{
	//TODO: Vision
	String packet;
	if(visiontotracker.receive(packet)) {
		printf("----------------------------");
		printf("Received Vision-To-TRACKER!\n");
		printf("<|%s|>\n", packet.c_str());

		dataVision = packet;
	}
}

void send()
{
	if(!USING_VISION) {
		 SSL_WrapperPacket packet;

		 TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
		 BallTracker *b = trackertoaiPacket->mutable_ball();

		//Identity Function Sim -> Tracker -> IA
		 b->set_x(dataSim.ball().x());
		 b->set_y(dataSim.ball().y());

		printf("ball (%5i, %5i) --\n", dataSim.ball().x(), dataSim.ball().y());

		for(int i = 0; i < dataSim.robots_blue_size(); i++) {
			RobotTracker *r = trackertoaiPacket->add_robots_blue();
			r->set_x(dataSim.robots_blue(i).x());
			r->set_y(dataSim.robots_blue(i).y());
			r->set_theta(dataSim.robots_blue(i).theta());

			printf("cur_pos[%5i](%5i, %5i) --\n", i, r->x(), r->y());
		}

		for(int i = 0; i < dataSim.robots_yellow_size(); i++) {
			RobotTracker *r = trackertoaiPacket->add_robots_yellow();
			r->set_x(dataSim.robots_yellow(i).x());
			r->set_y(dataSim.robots_yellow(i).y());
			r->set_theta(dataSim.robots_yellow(i).theta());

			printf("cur_pos[%5i](%5i, %5i) --\n", i, r->x(), r->y());
		}

		 trackertoai.send(packet);
		printf("Sent Tracker-To-AI --\n");

		debug_int(dataSim.robots_blue_size());
		debug_int(dataSim.robots_yellow_size());
	} else {
		//TODO: Vision
        char *str = dataVision.c_str();

        //é só copiar o código da IA antiga
        //faço isso amanhã
	}
}

int main()
{
	printf("Tracker Running!\n");

	simtotracker.open(false);
	aitotracker.open(false);
	radiototracker.open(false);
	visiontotracker.open(false);

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

