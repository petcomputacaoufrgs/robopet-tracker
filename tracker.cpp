#include "tracker.h"
#include <math.h>

#include <iostream>

double scaleFactorLength;
double scaleFactorWidth;


#define GAMBI_RAD_TO_DEGREE(x) ((x) * 180 / M_PI ) 

void Tracker::track() {

	if(usingSimulator) return;

	if(receivedMinInfo)
		identityTrack();
}

void Tracker::identityTrack() {

	int set_robots_blue[5]= {0,0,0,0,0};
	int set_robots_yellow[5]= {0,0,0,0,0};
	TrackerBall ball;
	TrackerRobot bot_blue;
	TrackerRobot bot_yellow;
	
	TrackerBall ballOld[dataVision.balls.size()];
	TrackerBall ballReal;
	double distanceToBallReal[dataVision.balls.size()];
	double nearer;
	int nearerId;
	
	

	//if you see a ball, then it is correct
	if(dataVision.balls.size()==1){
		ball.x = dataVision.balls[0].x();
		ball.y = dataVision.balls[0].y();

		setBall(ball);
	}	
	
	//if you have more than one ball, then picks the one that is closer to the former
	if(dataVision.balls.size()>1)
	{
				
		ballReal = getBall();
		
		nearer = 9999;
		nearerId = 0;
		
		//armazena todas bolas que ta vendo no vetor de bolas
		for(int i=0; i<dataVision.balls.size(); i++){
								
			if(dataVision.balls.size()>=1){
				ball.x = dataVision.balls[i].x();
				ball.y = dataVision.balls[i].y();

				ballOld[i] = ball;
				
				//calcula distancia das bolas vistas, com a bola antiga
				distanceToBallReal[i] = (ballOld[i].x - ballReal.x)*(ballOld[i].x - ballReal.x)
											+ (ballOld[i].y - ballReal.y)*(ballOld[i].y - ballReal.y);
											
	//			printf("Distancia pra bola: %lf", distanceToBallReal[i]);											
											
				if(nearer > distanceToBallReal[i]){
					nearer = distanceToBallReal[i];
					nearerId = i;
				}			
			}
		}	
		setBall(ballOld[nearerId]);
	}
	
	//first robot blue ->robot.robot_id();
	//		setBlues(vector<TrackerRobot> blues);
	//		setBlue(TrackerRobot blue, int index);

	
	for(int i = 0; i < dataVision.blueRobots.size(); i++) {

		//if(dataVision.blueRobots[i].has_robot_id()){ //work only if robot has ID
			if(!set_robots_blue[dataVision.blueRobots[i].robot_id()]){

				bot_blue.x = dataVision.blueRobots[i].x();
				bot_blue.y = dataVision.blueRobots[i].y();
				bot_blue.angle = GAMBI_RAD_TO_DEGREE( dataVision.blueRobots[i].orientation() );
				bot_blue.id = dataVision.blueRobots[i].robot_id();

				//bot_blue.id == 0 ? _blues.insert(_blues.begin(), 1, bot_blue) : _blues.push_back(bot_blue);
				_blues.push_back(bot_blue);
				set_robots_blue[bot_blue.id]=1;

			}
//		}
//		else
//			printf("Received robot with no ID. Ignoring it.\n");
	}
	
	for(int i = 0; i < dataVision.yellowRobots.size(); i++) {
		
		//if(dataVision.yellowRobots[i].has_robot_id()){
			//if(!set_robots_yellow[dataVision.yellowRobots[i].robot_id()]){

				bot_yellow.x = dataVision.yellowRobots[i].x();
				bot_yellow.y = dataVision.yellowRobots[i].y();
				bot_yellow.angle = GAMBI_RAD_TO_DEGREE( dataVision.yellowRobots[i].orientation() );
				bot_yellow.id = dataVision.yellowRobots[i].robot_id();

				//bot_yellow.id == 0 ? _yellows.insert(_yellows.begin(), 1, bot_yellow) : _yellows.push_back(bot_yellow);
				_yellows.push_back(bot_yellow);
				//set_robots_yellow[bot_yellow.id]=1;
			//}
		//}
		//else
			//printf("Received robot with no ID. Ignoring it.\n");
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

	if(receivedMinInfo)
		sendToAI();
	else
		printf("Waiting for minimal necessary information.\n");

}

void Tracker::receiveFromVision() {

	SSL_WrapperPacket packet;
	if(visiontotracker->receive(packet)){
		if(packet.has_detection()){
			printf("----------------------------");
			printf("Received Vision-To-TRACKER!\n");
			SSL_DetectionFrame detection = packet.detection();

			dataVision.balls_n = detection.balls_size();
			dataVision.robots_blue_n =  detection.robots_blue_size();
			dataVision.robots_yellow_n =  detection.robots_yellow_size();

			if(dataVision.balls_n > 0)
				dataVision.balls.clear();
			if(dataVision.robots_blue_n > 0)
				dataVision.blueRobots.clear();
			if(dataVision.robots_yellow_n > 0)
				dataVision.yellowRobots.clear();

			//Ball info:
			for (int i = 0; i < dataVision.balls_n; i++) {
				printf("ball[%i] = %f,%f\n",i, detection.balls(i).x(), detection.balls(i).y());
				dataVision.balls.push_back(detection.balls(i));
			}

			//Blue robot info:
			for (int i = 0; i < dataVision.robots_blue_n; i++) {
				printf("robot[%i] = %f,%f\n",i, detection.robots_blue(i).x(), detection.robots_blue(i).y());
				dataVision.blueRobots.push_back(detection.robots_blue(i));
			}

			//Yellow robot info:
			for (int i = 0; i < dataVision.robots_yellow_n; i++) {
				printf("robot[%i] = %f,%f\n",i, detection.robots_yellow(i).x(), detection.robots_yellow(i).y());
				dataVision.yellowRobots.push_back(detection.robots_yellow(i));
			}

			//printf("balls_n = %i\n", dataVision.balls_n);
			//printf("robots_blue_n = %i\n", dataVision.robots_blue_n);
			//printf("robots_yellow_n = %i\n", dataVision.robots_yellow_n);
		}
		if(packet.has_geometry()){
			printf("TEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\nTEM GEOMETRIA\n");
			//SSL_GeometryFieldSize field = packet.geometry().field();
			const SSL_GeometryData & geom = packet.geometry();
                printf("-[Geometry Data]-------\n");

                const SSL_GeometryFieldSize & field = geom.field();
			
			dataVision.fieldLength = field.field_length();
			dataVision.fieldWidth =  field.field_width();
			
			printf("fieldLength = %i\n", dataVision.fieldLength);
			printf("fieldWidth = %i\n", dataVision.fieldWidth);
			
			scaleFactorLength = (float)LENGTH / dataVision.fieldLength;
			scaleFactorWidth = (float) WIDTH / dataVision.fieldWidth;
			
			this->receivedMinInfo = true;
		}
		else
		{//gambiarra pra ver na CBR2011
			dataVision.fieldLength = 6050;
			dataVision.fieldWidth =  4050;
			
			scaleFactorLength = (float)LENGTH / dataVision.fieldLength;
			scaleFactorWidth = (float) WIDTH / dataVision.fieldWidth;
			
			this->receivedMinInfo = true;
		}
		
		if(receivedMinInfo)
			convertCoordinates();
	}
}

void Tracker::receiveFromRadio()
{
	RoboPET_WrapperPacket packet;
	if (radiototracker->receive(packet) && packet.has_radiototracker()) {
		if(isVerbose){
			printf("----------------------------");
			printf("Received Radio-To-TRACKER!\n");
		}
	}
}

void Tracker::receiveFromAI()
{
	RoboPET_WrapperPacket packet;
	if (aitotracker->receive(packet) && packet.has_aitotracker()) {
		if(isVerbose) {
			printf("----------------------------");
			printf("Received AI-To-TRACKER!\n");
		}
	}
}

void Tracker::receiveFromSim()
{
	TrackerRobot bot_blue;
	TrackerRobot bot_yellow;

	RoboPET_WrapperPacket packet;
	if (simtotracker->receive(packet) && packet.has_simtotracker()) {
		if(isVerbose) {
			printf("----------------------------");
			printf("Received SIM-To-TRACKER!\n");
		}
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
		
		//xunxo:
		scaleFactorLength = 1.;
		scaleFactorWidth = 1.;
		this->receivedMinInfo = true;
	}
	else
		if(isVerbose)
			printf("Didn't receive SIM-To-TRACKER.\n");
}

void Tracker::sendToAI() {

	if(_blues.size()!=0 || _yellows.size()!=0) {
	//	if(true){
			
		RoboPET_WrapperPacket packet;

		TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
		TrackerToAI::Ball *b = trackertoaiPacket->mutable_ball();
		
		b->set_x(_ball.x);
		b->set_y(_ball.y);
		
		if(isVerbose) {
			printf("----------------------------\n");
			printf("Send TRACKER-To-AI!\n");
			
			printf("ball (%5i, %5i) --\n", b->x(), b->y());		

			printf("blue robots sent:\n");
		}
		for(int i = 0; i < _blues.size(); i++) {

			TrackerToAI::Robot *r = trackertoaiPacket->add_blue_robots();
			r->set_x(_blues[i].x);
			r->set_y(_blues[i].y);
			r->set_theta(_blues[i].angle);
			r->set_id(_blues[i].id);
			
			if(isVerbose) {
				printf("%i:\t(%5i, %5i, %5i) --\n", r->id(), r->x(), r->y(), r->theta());
			}
		}

		if(isVerbose) {
			printf("yellow robots sent:\n");
		}
		for(int i = 0; i < _yellows.size(); i++) {

			TrackerToAI::Robot *r = trackertoaiPacket->add_yellow_robots();
			r->set_x(_yellows[i].x);
			r->set_y(_yellows[i].y);
			r->set_theta(_yellows[i].angle);
			r->set_id(_yellows[i].id);
			
			if(isVerbose) {
				printf("%i:\t(%5i, %5i, %5i) --\n", r->id(), r->x(), r->y(), r->theta());
			}
		}

		 trackertoai->send(packet);
	 }
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

	if(dataVision.balls_n > 0)
		for(int i = 0; i < dataVision.balls.size(); i++) {
			dataVision.balls[i].set_x( (dataVision.balls[i].x() * scaleFactorLength) + LENGTH/2. );
			dataVision.balls[i].set_y( (dataVision.balls[i].y() * scaleFactorWidth) + WIDTH/2. );
		}

	if(dataVision.robots_blue_n > 0)
		for(int i = 0; i < dataVision.blueRobots.size(); i++) {
			dataVision.blueRobots[i].set_x( (dataVision.blueRobots[i].x() * scaleFactorLength) + LENGTH/2.);
			dataVision.blueRobots[i].set_y( (dataVision.blueRobots[i].y() * scaleFactorWidth) + WIDTH/2.);
		}

	if(dataVision.robots_yellow_n > 0)
		for(int i = 0; i < dataVision.yellowRobots.size(); i++) {
			dataVision.yellowRobots[i].set_x( (dataVision.yellowRobots[i].x() * scaleFactorLength) + LENGTH/2.);
			dataVision.yellowRobots[i].set_y( (dataVision.yellowRobots[i].y() * scaleFactorWidth) + WIDTH/2.);
		}
}

Tracker::Tracker(bool sim, bool verbose) {

	usingSimulator = sim;
	isVerbose = verbose;
	
	receivedMinInfo = false;

	simtotracker = new RoboPETClient(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER);
	simtotracker->open(false);

	//aitotracker = new RoboPETClient(PORT_AI_TO_TRACKER, "localhost");
	//aitotracker->open(false);

	radiototracker = new RoboPETClient();//PORT_RADIO_TO_TRACKER, IP_RADIO_TO_TRACKER);
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
