#include <stdio.h>

#include "ssl_client.h"
#include "ssl_server.h"
#include "vector.h"

#define MAX_ROBOTS 5

RoboCupSSLServer trackertoai(PORT_TRACKER_TO_AI, IP_TRACKER_TO_AI);
RoboCupSSLClient simtotracker(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER),
				 aitotracker(PORT_AI_TO_TRACKER, IP_AI_TO_TRACKER),
				 radiototracker(PORT_RADIO_TO_TRACKER, IP_RADIO_TO_TRACKER),
				 visiontotracker(PORT_VISION_TO_TRACKER, IP_VISION_TO_TRACKER);

int DEBUG = 1, USING_VISION = 1;

SimToTracker dataSim;
String dataVision;

int robot_total[TEAM_TOTAL] = {5, 5}; //FOR VISION COMMUNICATION

Vector robot_last_position[TEAM_TOTAL][MAX_ROBOTS];

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
	if(visiontotracker.receive(packet) && USING_VISION) {
		//printf("----------------------------");
		//printf("Received Vision-To-TRACKER!\n");
		//printf("<|%s|>\n", packet.c_str());

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

		printf("ball (%5i, %5i) --\n", b->x(), b->y());

		for(int i = 0; i < dataSim.robots_blue_size(); i++) {
			RobotTracker *r = trackertoaiPacket->add_robots_blue();
			r->set_x(dataSim.robots_blue(i).x());
			r->set_y(dataSim.robots_blue(i).y());
			r->set_theta(dataSim.robots_blue(i).theta());
			r->set_id(i);

			printf("cur_pos[%2i][%5i](%5i, %5i) --\n", TEAM_BLUE, r->id(), r->x(), r->y());
		}

		for(int i = 0; i < dataSim.robots_yellow_size(); i++) {
			RobotTracker *r = trackertoaiPacket->add_robots_yellow();
			r->set_x(dataSim.robots_yellow(i).x());
			r->set_y(dataSim.robots_yellow(i).y());
			r->set_theta(dataSim.robots_yellow(i).theta());
			r->set_id(i);

			printf("cur_pos[%2i][%5i](%5i, %5i) --\n", TEAM_YELLOW, r->id(), r->x(), r->y());
		}

		 trackertoai.send(packet);
		printf("Sent Tracker-To-AI --\n");

		debug_int(dataSim.robots_blue_size());
		debug_int(dataSim.robots_yellow_size());
	} else {
		//TODO: Vision
	    int tamanho = dataVision.size();
	    int posicoes[50];
	    string stringAux = "";
	    int k = 0;

	    for (int i = 0; i < tamanho; i++)
	    {
		    if (dataVision[i] != '#')
		    {
			    stringAux += dataVision[i];
		    }
		    else
		    {
			    posicoes[k++] = atoi(stringAux.c_str());
			    stringAux = "";
		    }
        }

	    for(int i = 0; i < k - 3; i += 3) {
            ;//printf("jog[%2i](%5i, %5i, %5i) ", i, posicoes[i], posicoes[i + 1], posicoes[i + 2]);
	    }
	    //printf("ball(%5i, %5i)\n", posicoes[k - 2], posicoes[k - 1]);
	    //printf("                       \n                              \n");


		 SSL_WrapperPacket packet;

		 TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
		 BallTracker *b = trackertoaiPacket->mutable_ball();

		 b->set_x(posicoes[(robot_total[TEAM_BLUE] + robot_total[TEAM_YELLOW]) * 3]);
		 b->set_y(posicoes[(robot_total[TEAM_BLUE] + robot_total[TEAM_YELLOW]) * 3 + 1]);

        if(b->x() == -25 && b->y() == -6) {
            b->set_x(0);
            b->set_y(0);
        }
		printf("ball (%5i, %5i) --\n", b->x(), b->y());

		for(int i = 0; i < robot_total[TEAM_BLUE]; i++) {
		    if(posicoes[i * 3] == -25 &&
		       posicoes[i * 3 + 1] == -6 &&
		       posicoes[i * 3 + 2] == -1)
		            continue;

			RobotTracker *r = trackertoaiPacket->add_robots_blue();
			r->set_x(posicoes[i * 3]);
			r->set_y(posicoes[i * 3 + 1]);
			r->set_theta(posicoes[i * 3 + 2]);
			r->set_id(i);

			printf("cur_pos[%2i][%5i](%5i, %5i, %5i) --\n", TEAM_BLUE, r->id(), r->x(), r->y(), r->theta());
		}

		for(int i = 0; i < robot_total[TEAM_YELLOW]; i++) {
		    if(posicoes[i * 3 + robot_total[TEAM_BLUE] * 3] == -25 &&
		       posicoes[i * 3 + 1 + robot_total[TEAM_BLUE] * 3] == -6 &&
		       posicoes[i * 3 + 2 + robot_total[TEAM_BLUE] * 3] == 0)
		            continue;

			RobotTracker *r = trackertoaiPacket->add_robots_yellow();
			r->set_x(posicoes[i * 3 + robot_total[TEAM_BLUE] * 3]);
			r->set_y(posicoes[i * 3 + 1 + robot_total[TEAM_BLUE] * 3]);
			r->set_theta(posicoes[i * 3 + 2 + robot_total[TEAM_BLUE] * 3]);
			r->set_id(i);

			printf("cur_pos[%2i][%5i](%5i, %5i, %5i) --\n", TEAM_YELLOW, r->id(), r->x(), r->y(), r->theta());
		}

		 trackertoai.send(packet);

		for(int i = 1; i < 5; i++) {
		    ;//printf("posicoes[%2i]: %5i\n", k - i, posicoes[k - i]);
		}

		printf("Sent Tracker-To-AI --\n");

		debug_int(robot_total[TEAM_BLUE]);
		debug_int(robot_total[TEAM_YELLOW]);
	}

}

int main(int argc, char **argv)
{
	printf("Tracker Running!\n");

    if(argc > 1) {
        USING_VISION = argv[1][0] == '1';
        printf("USING_VISION set to %i\n", USING_VISION);
    }

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

