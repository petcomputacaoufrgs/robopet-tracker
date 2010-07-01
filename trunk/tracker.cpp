#include "tracker.h"

void Tracker::track() {

	for(unsigned int i = 0; i < dataVision.balls.size(); i++) {
		printf("-Ball (%2d/%2d): CONF=%4.2f POS=<%9.2f,%9.2f> ", i+1, dataVision.balls.size(), dataVision.balls[i].confidence(),
																				dataVision.balls[i].x(),dataVision.balls[i].y());
		if (dataVision.balls[i].has_z()) {
			printf("Z=%7.2f ", dataVision.balls[i].z());
		} else {
			printf("Z=N/A   ");
		}
		printf("RAW=<%8.2f,%8.2f>\n", dataVision.balls[i].pixel_x(), dataVision.balls[i].pixel_y());
	}

	for(unsigned int i = 0; i < dataVision.blueRobots.size(); i++) {
		printf("-Robot(B) (%2d/%2d): ",i+1, dataVision.blueRobots.size());
		printRobotInfo(dataVision.blueRobots[i]);
	}

	for(unsigned int i = 0; i < dataVision.yellowRobots.size(); i++) {
		printf("-Robot(B) (%2d/%2d): ",i+1, dataVision.yellowRobots.size());
		printRobotInfo(dataVision.yellowRobots[i]);
	}
}

void Tracker::simpleTrack() {

	int set_robots_blue[5]= {0,0,0,0,0};
	int set_robots_yellow[5]= {0,0,0,0,0};
	TrackerBall ball;
	TrackerRobot bot_blue;
	TrackerRobot bot_yellow;

	//first ball
	if(dataVision.balls.size()>=1){
		ball.x = dataVision.balls[0].x();
		ball.y = dataVision.balls[0].y();

		setBall(ball);
	}

	//first robot blue ->robot.robot_id();
	//		setBlues(vector<TrackerRobot> blues);
	//		setBlue(TrackerRobot blue, int index);
	for(int i = 0; i < dataVision.blueRobots.size(); i++) {
		if(dataVision.blueRobots[i].has_robot_id()){
			if(!set_robots_blue[dataVision.blueRobots[i].robot_id()]){
				//typedef struct _robot_ {
				//	double x,y,angle;
				//} TrackerRobot;
				bot_blue.x = dataVision.blueRobots[i].x();
				bot_blue.y = dataVision.blueRobots[i].y();
				bot_blue.angle = dataVision.blueRobots[i].orientation();

				setBlue(bot_blue,dataVision.blueRobots[i].robot_id());
				set_robots_blue[dataVision.blueRobots[i].robot_id()]=1;
			}
		}
	}


	for(int i = 0; i < dataVision.yellowRobots.size(); i++) {
		if(dataVision.yellowRobots[i].has_robot_id()){
			if(!set_robots_yellow[dataVision.yellowRobots[i].robot_id()]){
				bot_yellow.x = dataVision.yellowRobots[i].x();
				bot_yellow.y = dataVision.yellowRobots[i].y();
				bot_yellow.angle = dataVision.yellowRobots[i].orientation();

				setYellow(bot_yellow,dataVision.yellowRobots[i].robot_id());
				set_robots_yellow[dataVision.yellowRobots[i].robot_id()]=1;
			}
		}
	}

}


void Tracker::receive() {
	receiveFromSim();
	receiveFromAI();
	receiveFromRadio();
	receiveFromVision();
}

void Tracker::send() {
	sendToAI();
}

void Tracker::receiveFromVision() {

	int balls_n, robots_blue_n, robots_yellow_n;

	SSL_WrapperPacket packet;
	if(visiontotracker->receive(packet) && packet.has_detection()) {
		printf("----------------------------");
		printf("Received Vision-To-TRACKER!\n");
		SSL_DetectionFrame detection = packet.detection();

		balls_n = detection.balls_size();
		robots_blue_n =  detection.robots_blue_size();
		robots_yellow_n =  detection.robots_yellow_size();

		//Ball info:
		for (int i = 0; i < balls_n; i++)
			dataVision.balls.push_back(detection.balls(i));

		//Blue robot info:
		for (int i = 0; i < robots_blue_n; i++)
			dataVision.blueRobots.push_back(detection.robots_blue(i));

		//Yellow robot info:
		for (int i = 0; i < robots_yellow_n; i++)
			dataVision.yellowRobots.push_back(detection.robots_yellow(i));

	}
}

void Tracker::receiveFromRadio()
{
	RoboPET_WrapperPacket packet;
	if (radiototracker->receive(packet) && packet.has_radiototracker()) {
		printf("----------------------------");
		printf("Received Radio-To-TRACKER!\n");
	}
}

void Tracker::receiveFromAI()
{
	RoboPET_WrapperPacket packet;
	if (aitotracker->receive(packet) && packet.has_aitotracker()) {
		printf("----------------------------");
		printf("Received AI-To-TRACKER!\n");
	}
}

void Tracker::receiveFromSim()
{
	RoboPET_WrapperPacket packet;
	if (simtotracker->receive(packet) && packet.has_simtotracker()) {
		printf("----------------------------");
		printf("Received SIM-To-TRACKER!\n");
		dataSim = packet.simtotracker();
	}
}

void Tracker::sendToAI() {
}

//Getters

TrackerBall Tracker::getBall() {
	return _ball;
}
vector<TrackerRobot> Tracker::getBlues() {
	return _blues;
}
vector<TrackerRobot> Tracker::getYellows() {
	return _yellows;
}

TrackerRobot Tracker::getBlue(int index) {
	return _blues[index];
}
TrackerRobot Tracker::getYellow(int index) {
	return _yellows[index];
}

//Setters

void Tracker::setBall(TrackerBall ball) {
	_ball = ball;
}

void Tracker::setBlues(vector<TrackerRobot> blues) {
	_blues = blues;
}

void Tracker::setYellows(vector<TrackerRobot> yellows) {
	_yellows = yellows;
}

void Tracker::setBlue(TrackerRobot blue, int index) {
	_blues[index] = blue;
}

void Tracker::setYellow(TrackerRobot yellow, int index) {
	_yellows[index] = yellow;
}

Tracker::Tracker() {

	simtotracker = new RoboPETClient(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER);
	simtotracker->open(false);

	aitotracker = new RoboPETClient(PORT_AI_TO_TRACKER, IP_AI_TO_TRACKER);
	aitotracker->open(false);

	radiototracker = new RoboPETClient(PORT_RADIO_TO_TRACKER, IP_RADIO_TO_TRACKER);
	radiototracker->open(false);

	visiontotracker = new RoboCupSSLClient();
	visiontotracker->open(false);

	printf("Press <Enter> to open connection with client...\n");
	getchar();
	trackertoai = new RoboPETServer(PORT_TRACKER_TO_AI, IP_TRACKER_TO_AI);
	trackertoai->open();
}

Tracker::~Tracker() {

	trackertoai->close(); 	  delete(trackertoai);
	simtotracker->close(); 	  delete(simtotracker);
	aitotracker->close(); 	  delete(aitotracker);
	radiototracker->close();  delete(radiototracker);
	visiontotracker->close(); delete(visiontotracker);
}


/**********************************************************************/

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
