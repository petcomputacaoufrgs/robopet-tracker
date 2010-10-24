#include "tracker.h"

void Tracker::track() {

	if(usingSimulator) return;
	
	identityTrack();
}

void Tracker::identityTrack() {

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
				bot_blue.id = dataVision.blueRobots[i].robot_id();

				_blues.push_back(bot_blue);
				set_robots_blue[bot_blue.id]=1;
			}
		}
	}

	for(int i = 0; i < dataVision.yellowRobots.size(); i++) {
		if(dataVision.yellowRobots[i].has_robot_id()){
			if(!set_robots_yellow[dataVision.yellowRobots[i].robot_id()]){
				bot_yellow.x = dataVision.yellowRobots[i].x();
				bot_yellow.y = dataVision.yellowRobots[i].y();
				bot_yellow.angle = dataVision.yellowRobots[i].orientation();
				bot_yellow.id = dataVision.yellowRobots[i].robot_id();

				_yellows.push_back(bot_yellow);
				set_robots_yellow[bot_yellow.id]=1;
			}
		}
	}
	//printMyInfo();
}

void Tracker::simpleTrack() {
}

void Tracker::kalmanFilter() {
}

void Tracker::receive() {
	
	_blues.clear();
	_yellows.clear();
	
	//receiveFromAI(); //not used yet
	//receiveFromRadio(); //not used yet
	if(usingSimulator)
		receiveFromSim();
	else
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
		
		dataVision.balls.clear();
		dataVision.blueRobots.clear();
		dataVision.yellowRobots.clear();

		//Ball info:
		for (int i = 0; i < balls_n; i++)
			dataVision.balls.push_back(detection.balls(i));

		//Blue robot info:
		for (int i = 0; i < robots_blue_n; i++)
			dataVision.blueRobots.push_back(detection.robots_blue(i));

		//Yellow robot info:
		for (int i = 0; i < robots_yellow_n; i++)
			dataVision.yellowRobots.push_back(detection.robots_yellow(i));

		convertCoordinates();
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
	TrackerRobot bot_blue;
	TrackerRobot bot_yellow;
	
	RoboPET_WrapperPacket packet;
	if (simtotracker->receive(packet) && packet.has_simtotracker()) {
		printf("----------------------------");
		printf("Received SIM-To-TRACKER!\n");
		system("clear");
		dataSim = packet.simtotracker();
		
		_ball.x = dataSim.ball().x();
		_ball.y = dataSim.ball().y();
		
		for(int i = 0; i < dataSim.blue_robots_size(); i++) {
			bot_blue.x = dataSim.blue_robots(i).x();
			bot_blue.y = dataSim.blue_robots(i).y();
			bot_blue.angle = dataSim.blue_robots(i).theta();
			bot_blue.id = dataSim.blue_robots(i).id();
			_blues.push_back(bot_blue);
		}

		for(int i = 0; i < dataSim.yellow_robots_size(); i++) {
			bot_yellow.x = dataSim.yellow_robots(i).x();
			bot_yellow.y = dataSim.yellow_robots(i).y();
			bot_yellow.angle = dataSim.yellow_robots(i).theta();
			bot_yellow.id = dataSim.yellow_robots(i).id();
			_yellows.push_back(bot_yellow);
		}
	}
	else
		printf("Didn't receive SIM-To-TRACKER.\n"); 
}

void Tracker::sendToAI() {
	
	RoboPET_WrapperPacket packet;
	
	TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
	TrackerToAI::Ball *b = trackertoaiPacket->mutable_ball();

	b->set_x(_ball.x);
	b->set_y(_ball.y);

	//printf("ball (%5i, %5i) --\n", b->x(), b->y());

	for(int i = 0; i < _blues.size(); i++) {

		TrackerToAI::Robot *r = trackertoaiPacket->add_blue_robots();
		r->set_x(_blues[i].x);
		r->set_y(_blues[i].y);
		r->set_theta(_blues[i].angle);
		r->set_id(_blues[i].id);

		printf("cur_pos[%5i](%5i, %5i, %5i) --\n", r->id(), r->x(), r->y(), r->theta());
	}

	for(int i = 0; i < _yellows.size(); i++) {

		TrackerToAI::Robot *r = trackertoaiPacket->add_yellow_robots();
		r->set_x(_yellows[i].x);
		r->set_y(_yellows[i].y);
		r->set_theta(_yellows[i].angle);
		r->set_id(_blues[i].id);

		printf("cur_pos[%5i](%5i, %5i, %5i) --\n", r->id(), r->x(), r->y(), r->theta());
	}

	 trackertoai->send(packet);
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
	if(_blues.size() > index)
		return _blues[index];
	else
		ERROR("Trying to access out of bound index!");
}

TrackerRobot Tracker::getYellow(int index) {
	if(_yellows.size() > index)
		return _yellows[index];
	else
		ERROR("Trying to access out of bound index!");
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
	if(_blues.size() > index)
		_blues[index] = blue;
	else
		ERROR("Trying to access out of bound index!");
}

void Tracker::setYellow(TrackerRobot yellow, int index) {
	if(_yellows.size() > index)
		_yellows[index] = yellow;
	else
		ERROR("Trying to access out of bound index!");
}

void Tracker::convertCoordinates() {
	
	for(int i = 0; i < dataVision.balls.size(); i++) {
		dataVision.balls[i].set_x(dataVision.balls[i].x() + LENGTH/2.);
		dataVision.balls[i].set_y(dataVision.balls[i].y() + HEIGHT/2.);
	}
	
	for(int i = 0; i < dataVision.blueRobots.size(); i++) {
		dataVision.blueRobots[i].set_x(dataVision.blueRobots[i].x() + LENGTH/2.);
		dataVision.blueRobots[i].set_y(dataVision.blueRobots[i].y() + HEIGHT/2.);
	}
	
	for(int i = 0; i < dataVision.yellowRobots.size(); i++) {
		dataVision.yellowRobots[i].set_x(dataVision.yellowRobots[i].x() + LENGTH/2.);
		dataVision.yellowRobots[i].set_y(dataVision.yellowRobots[i].y() + HEIGHT/2.);
	}
}

Tracker::Tracker(bool sim) {
	
	usingSimulator = sim;

	simtotracker = new RoboPETClient(8100, IP_SIM_TO_TRACKER);
	simtotracker->open(false);

	aitotracker = new RoboPETClient(PORT_AI_TO_TRACKER, "172.26.201.5");
	aitotracker->open(false);

	radiototracker = new RoboPETClient(PORT_RADIO_TO_TRACKER, IP_RADIO_TO_TRACKER);
	radiototracker->open(false);

	visiontotracker = new RoboCupSSLClient();
	visiontotracker->open(false);

	printf("Press <Enter> to open connection with client...\n");
	getchar();
	printf("Ready to receive data.\n");
	
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

void Tracker::printMyInfo(){
	//print ball
	printf("Ball cordinates: <X = %f, Y = %f>", _ball.x,_ball.y);
	printf("\n");

	//print blues
	int i = 0;
	while(i < _blues.size()){
		printf("Blue Robot %f coordinates: <X = %f, Y = %f>", _blues[i].x,_blues[i].y,_blues[i].angle);
		printf("\n");
		i++;
	}

	//print yellows
	i = 0;
	while(i < _yellows.size()){
		printf("Yellow Robot %f coordinates: <X = %f, Y = %f>;", _yellows[i].x,_yellows[i].y,_yellows[i].angle);
		printf("\n");
		i++;
	}
}

void Tracker::printVisionInfo() {

	for(unsigned int i = 0; i < dataVision.balls.size(); i++) {
		printf("-Ball (%2d/%2d): CONF=%4.2f POS=<%9.2f,%9.2f> ", i+1, dataVision.balls.size(), dataVision.balls[i].confidence(), dataVision.balls[i].x(),dataVision.balls[i].y());
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

void Tracker::printSimInfo() {}

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
