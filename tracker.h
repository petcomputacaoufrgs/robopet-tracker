#ifndef ROBOPET_TRACKER_H
#define ROBOPET_TRACKER_H

#include <stdio.h>
#include <vector>

#include <rp_client.h>
#include <rp_server.h>
#include "robocup_ssl_client.h"

#define ERROR(MSG) printf("ERROR: Call of function %s in file %s, line %i resulted in an error! - %s\n", __FUNCTION__, __FILE__, __LINE__, MSG)

#define LENGTH 6050
#define WIDTH 4050

typedef struct _ball_ {
	double x,y;
} TrackerBall;

typedef struct _robot_ {
	double x,y,angle;
	int id;
} TrackerRobot;

typedef struct _visiontotracker_ {
	int balls_n, robots_blue_n, robots_yellow_n;
	vector<SSL_DetectionBall> balls;
	vector<SSL_DetectionRobot> blueRobots;
	vector<SSL_DetectionRobot> yellowRobots;
	int fieldLength;
	int fieldWidth;
}VisionToTracker;

class Tracker {

	private:

		RoboPETServer 	  *trackertoai;
		RoboPETClient 	  *simtotracker,
						  *aitotracker,
						  *radiototracker;
		RoboCupSSLClient  *visiontotracker;

		VisionToTracker dataVision;
		SimToTracker dataSim;

		TrackerBall _ball;
		vector<TrackerRobot> _blues;
		vector<TrackerRobot> _yellows;		
		
		bool usingSimulator;
		bool receivedMinInfo;

	public:

		Tracker(bool sim = false);
		~Tracker();

		void track();
		
		void identityTrack();
		void simpleTrack();
		void kalmanFilter();
		
		void receive();
		void send();

		// Getters
		TrackerBall getBall();
		vector<TrackerRobot> getBlues();
		vector<TrackerRobot> getYellows();
		TrackerRobot getBlue(int index);
		TrackerRobot getYellow(int index);

		//Setters
		void setBall(TrackerBall ball);
		void setBlues(vector<TrackerRobot> blues);
		void setYellows(vector<TrackerRobot> yellows);
		void setBlue(TrackerRobot blue, int index);
		void setYellow(TrackerRobot yellow, int index);
		
		void printMyInfo();
		void printVisionInfo();
		void printSimInfo();

	private:

		void receiveFromVision();
		void receiveFromRadio();
		void receiveFromAI();
		void receiveFromSim();

		void sendToAI();
		
		void convertCoordinates();
};

/**********************************************************************/
void printRobotInfo(const SSL_DetectionRobot & robot);

#endif
