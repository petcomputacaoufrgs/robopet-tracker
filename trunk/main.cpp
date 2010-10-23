#include "tracker.h"

int main(int argc, char **argv)
{
	printf("Tracker Running!\n");
	
	bool sim = false;
	
    if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			string param; 
			param.assign(argv[i]);
			if(param.compare("-sim") == 0) sim = true;
			
		}
    }
    
    if(sim)
		printf("Tracker will receive from SIM.\n");
	else
		printf("Tracker will receive from VISION.\n");
    
    Tracker tracker(sim);

	while(1) {
	
		tracker.receive();
		tracker.track();
		tracker.send();
	}

    return 0;
}

/*
#include "vector.h"

using std::vector;

#define MAX_ROBOTS 5

int DEBUG = 1, USING_VISION = 0;

String dataVision;

int robot_total[TEAM_TOTAL] = {5, 5}; //FOR VISION COMMUNICATION

Vector robot_last_position[TEAM_TOTAL][MAX_ROBOTS];

void send()
{
	if(!USING_VISION) {
		 RoboPET_WrapperPacket packet;

		 TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
		 TrackerToAI::Ball *b = trackertoaiPacket->mutable_ball();

		//Identity Function Sim -> Tracker -> IA
		 b->set_x(dataSim.ball().x());
		 b->set_y(dataSim.ball().y());

		printf("ball (%5i, %5i) --\n", b->x(), b->y());

		for(int i = 0; i < dataSim.blue_robots_size(); i++) {
			TrackerToAI::Robot *r = trackertoaiPacket->add_blue_robots();
			r->set_x(dataSim.blue_robots(i).x());
			r->set_y(dataSim.blue_robots(i).y());
			r->set_theta(dataSim.blue_robots(i).theta());
			r->set_id(i);

			printf("cur_pos[%2i][%5i](%5i, %5i) --\n", TEAM_BLUE, r->id(), r->x(), r->y());
		}

		for(int i = 0; i < dataSim.yellow_robots_size(); i++) {
			TrackerToAI::Robot *r = trackertoaiPacket->add_yellow_robots();
			r->set_x(dataSim.yellow_robots(i).x());
			r->set_y(dataSim.yellow_robots(i).y());
			r->set_theta(dataSim.yellow_robots(i).theta());
			r->set_id(i);

			printf("cur_pos[%2i][%5i](%5i, %5i) --\n", TEAM_YELLOW, r->id(), r->x(), r->y());
		}

		trackertoai.send(packet);
		printf("Sent Tracker-To-AI --\n");

		debug_int(dataSim.blue_robots_size());
		debug_int(dataSim.yellow_robots_size());
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


		RoboPET_WrapperPacket packet;

		TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
		TrackerToAI::Ball *b = trackertoaiPacket->mutable_ball();

		b->set_x(posicoes[(robot_total[TEAM_BLUE] + robot_total[TEAM_YELLOW]) * 3]);
		b->set_y(posicoes[(robot_total[TEAM_BLUE] + robot_total[TEAM_YELLOW]) * 3 + 1]);

        if(b->x() == -25 && b->y() == -6) {
            b->set_x(0);
            b->set_y(0);
            printf("x");
        } else {
            printf(".");
        }
		//printf("ball (%5i, %5i) --\n", b->x(), b->y());



		for(int i = 0; i < robot_total[TEAM_BLUE]; i++) {
		    if(posicoes[i * 3] == -25 &&
		       posicoes[i * 3 + 1] == -6 &&
		       posicoes[i * 3 + 2] == -1)
		            continue;

			TrackerToAI::Robot *r = trackertoaiPacket->add_blue_robots();
			r->set_x(posicoes[i * 3]);
			r->set_y(posicoes[i * 3 + 1]);
			r->set_theta(posicoes[i * 3 + 2]);
			r->set_id(i);

			//printf("cur_pos[%2i][%5i](%5i, %5i, %5i) --\n", TEAM_BLUE, r->id(), r->x(), r->y(), r->theta());
		}

		for(int i = 0; i < robot_total[TEAM_YELLOW]; i++) {
		    if(posicoes[i * 3 + robot_total[TEAM_BLUE] * 3] == -25 &&
		       posicoes[i * 3 + 1 + robot_total[TEAM_BLUE] * 3] == -6 &&
		       posicoes[i * 3 + 2 + robot_total[TEAM_BLUE] * 3] == 0)
		            continue;

			TrackerToAI::Robot *r = trackertoaiPacket->add_yellow_robots();
			r->set_x(posicoes[i * 3 + robot_total[TEAM_BLUE] * 3]);
			r->set_y(posicoes[i * 3 + 1 + robot_total[TEAM_BLUE] * 3]);
			r->set_theta(posicoes[i * 3 + 2 + robot_total[TEAM_BLUE] * 3]);
			r->set_id(i);

			//printf("cur_pos[%2i][%5i](%5i, %5i, %5i) --\n", TEAM_YELLOW, r->id(), r->x(), r->y(), r->theta());
		}

		 trackertoai.send(packet);

		for(int i = 1; i < 5; i++) {
		    ;//printf("posicoes[%2i]: %5i\n", k - i, posicoes[k - i]);
		}

		//printf("Sent Tracker-To-AI --\n");

		debug_int(robot_total[TEAM_BLUE]);
		debug_int(robot_total[TEAM_YELLOW]);
	}

}
*/

/*
clrscr();
int scrCount = 0;

scrCount++;
if(scrCount == SCR_CLEAR_DELAY) {
	scrCount = 0;
	clrscr();
}
rewindscr();
*/
