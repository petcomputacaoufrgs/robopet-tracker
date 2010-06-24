#include <stdio.h>

#include "robocup_ssl_client.h"
#include "vector.h"
#include <rp_client.h>
#include <rp_server.h>

#define MAX_ROBOTS 5

RoboPETServer trackertoai(PORT_TRACKER_TO_AI, IP_TRACKER_TO_AI);
RoboPETClient simtotracker(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER),
			  aitotracker(PORT_AI_TO_TRACKER, IP_AI_TO_TRACKER),
			  radiototracker(PORT_RADIO_TO_TRACKER, IP_RADIO_TO_TRACKER);

int DEBUG = 1, USING_VISION = 0;

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
	RoboPET_WrapperPacket packet;
	if (simtotracker.receive(packet) && packet.has_simtotracker()) {
		printf("----------------------------");
		printf("Received SIM-To-TRACKER!\n");

		dataSim = packet.simtotracker();
	}
}

void receiveFromAI()
{
	RoboPET_WrapperPacket packet;
	if (aitotracker.receive(packet) && packet.has_aitotracker()) {
		printf("----------------------------");
		printf("Received AI-To-TRACKER!\n");
	}
}

void receiveFromRadio()
{
	RoboPET_WrapperPacket packet;
	if (radiototracker.receive(packet) && packet.has_radiototracker()) {
		printf("----------------------------");
		printf("Received Radio-To-TRACKER!\n");
	}
}

void receiveFromVision()
{
/*
	//TODO: Vision
	String packet;
	if(visiontotracker.receive(packet) && USING_VISION) {
		//printf("----------------------------");
		//printf("Received Vision-To-TRACKER!\n");
		//printf("<|%s|>\n", packet.c_str());

		dataVision = packet;
	}
*/
}

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


void printRobotInfo(const SSL_DetectionRobot & robot) {
    printf("CONF=%4.2f ", robot.confidence());
    if (robot.has_robot_id()) {
        printf("ID=%3d ",robot.robot_id());
    } else {
        printf("ID=N/A ");
    }
    printf(" HEIGHT=%6.2f POS=<%9.2f,%9.2f> ",robot.height(),robot.x(),robot.y());
    if (robot.has_orientation()) {
        printf("ANGLE=%6.3f ",robot.orientation());
    } else {
        printf("ANGLE=N/A    ");
    }
    printf("RAW=<%8.2f,%8.2f>\n",robot.pixel_x(),robot.pixel_y());
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

	//printf("Press <Enter> to open connection with client...\n");
	//getchar();
	//trackertoai.open();

	//clrscr();
	//int scrCount = 0;
	while(1) {
	    //scrCount++;
	    //if(scrCount == SCR_CLEAR_DELAY) {
	    //    scrCount = 0;
	    //    clrscr();
	    //}
		//rewindscr();
		receive();
		//send();
	}
	
    RoboCupSSLClient client;
    client.open(true);
    SSL_WrapperPacket packet;

    while(true) {
        if (client.receive(packet)) {
            printf("-----Received Wrapper Packet---------------------------------------------\n");
            //see if the packet contains a robot detection frame:
            if (packet.has_detection()) {
                SSL_DetectionFrame detection = packet.detection();
                //Display the contents of the robot detection results:
                double t_now = 1;//GetTimeSec();

                printf("-[Detection Data]-------\n");
                //Frame info:
                printf("Camera ID=%d FRAME=%d T_CAPTURE=%.4f\n",detection.camera_id(),detection.frame_number(),detection.t_capture());

                printf("SSL-Vision Processing Latency                   %7.3fms\n",(detection.t_sent()-detection.t_capture())*1000.0);
                printf("Network Latency (assuming synched system clock) %7.3fms\n",(t_now-detection.t_sent())*1000.0);
                printf("Total Latency   (assuming synched system clock) %7.3fms\n",(t_now-detection.t_capture())*1000.0);
                int balls_n = detection.balls_size();
                int robots_blue_n =  detection.robots_blue_size();
                int robots_yellow_n =  detection.robots_yellow_size();

                //Ball info:
                for (int i = 0; i < balls_n; i++) {
                    SSL_DetectionBall ball = detection.balls(i);
                    printf("-Ball (%2d/%2d): CONF=%4.2f POS=<%9.2f,%9.2f> ", i+1, balls_n, ball.confidence(),ball.x(),ball.y());
                    if (ball.has_z()) {
                        printf("Z=%7.2f ",ball.z());
                    } else {
                        printf("Z=N/A   ");
                    }
                    printf("RAW=<%8.2f,%8.2f>\n",ball.pixel_x(),ball.pixel_y());
                }

                //Blue robot info:
                for (int i = 0; i < robots_blue_n; i++) {
                    SSL_DetectionRobot robot = detection.robots_blue(i);
                    printf("-Robot(B) (%2d/%2d): ",i+1, robots_blue_n);
                    printRobotInfo(robot);
                }

                //Yellow robot info:
                for (int i = 0; i < robots_yellow_n; i++) {
                    SSL_DetectionRobot robot = detection.robots_yellow(i);
                    printf("-Robot(Y) (%2d/%2d): ",i+1, robots_yellow_n);
                    printRobotInfo(robot);
                }

            }

            //see if packet contains geometry data:
            if (packet.has_geometry()) {
                const SSL_GeometryData & geom = packet.geometry();
                printf("-[Geometry Data]-------\n");

                const SSL_GeometryFieldSize & field = geom.field();
                printf("Field Dimensions:\n");
                printf("  -line_width=%d (mm)\n",field.line_width());
                printf("  -field_length=%d (mm)\n",field.field_length());
                printf("  -field_width=%d (mm)\n",field.field_width());
                printf("  -boundary_width=%d (mm)\n",field.boundary_width());
                printf("  -referee_width=%d (mm)\n",field.referee_width());
                printf("  -goal_width=%d (mm)\n",field.goal_width());
                printf("  -goal_depth=%d (mm)\n",field.goal_depth());
                printf("  -goal_wall_width=%d (mm)\n",field.goal_wall_width());
                printf("  -center_circle_radius=%d (mm)\n",field.center_circle_radius());
                printf("  -defense_radius=%d (mm)\n",field.defense_radius());
                printf("  -defense_stretch=%d (mm)\n",field.defense_stretch());
                printf("  -free_kick_from_defense_dist=%d (mm)\n",field.free_kick_from_defense_dist());
                printf("  -penalty_spot_from_field_line_dist=%d (mm)\n",field.penalty_spot_from_field_line_dist());
                printf("  -penalty_line_from_spot_dist=%d (mm)\n",field.penalty_line_from_spot_dist());

                int calib_n = geom.calib_size();
                for (int i=0; i< calib_n; i++) {
                    const SSL_GeometryCameraCalibration & calib = geom.calib(i);
                    printf("Camera Geometry for Camera ID %d:\n", calib.camera_id());
                    printf("  -focal_length=%.2f\n",calib.focal_length());
                    printf("  -principal_point_x=%.2f\n",calib.principal_point_x());
                    printf("  -principal_point_y=%.2f\n",calib.principal_point_y());
                    printf("  -distortion=%.2f\n",calib.distortion());
                    printf("  -q0=%.2f\n",calib.q0());
                    printf("  -q1=%.2f\n",calib.q1());
                    printf("  -q2=%.2f\n",calib.q2());
                    printf("  -q3=%.2f\n",calib.q3());
                    printf("  -tx=%.2f\n",calib.tx());
                    printf("  -ty=%.2f\n",calib.ty());
                    printf("  -tz=%.2f\n",calib.tz());

                    if (calib.has_derived_camera_world_tx() && calib.has_derived_camera_world_ty() && calib.has_derived_camera_world_tz()) {
                      printf("  -derived_camera_world_tx=%.f\n",calib.derived_camera_world_tx());
                      printf("  -derived_camera_world_ty=%.f\n",calib.derived_camera_world_ty());
                      printf("  -derived_camera_world_tz=%.f\n",calib.derived_camera_world_tz());
                    }

                }
            }
        }
    }

    return 0;
}
