#ifndef ROBOPET_TRACKER_H
#define ROBOPET_TRACKER_H

#include <stdio.h>
#include <vector>

#include <rp_client.h>
#include <rp_server.h>
#include "robocup_ssl_client.h"

typedef struct _ball_ {
	double x,y;
} TrackerBall;

typedef struct _robot_ {
	double x,y,angle;
} TrackerRobot;

typedef struct _visiontotracker_ {
	vector<SSL_DetectionBall> balls;
	vector<SSL_DetectionRobot> blueRobots;
	vector<SSL_DetectionRobot> yellowRobots;
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


	public:

		Tracker();
		~Tracker();

		void track();
		void simpleTrack();
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

	private:

		void receiveFromVision();
		void receiveFromRadio();
		void receiveFromAI();
		void receiveFromSim();

		void sendToAI();
};

/**********************************************************************/
void printRobotInfo(const SSL_DetectionRobot & robot);

#endif
