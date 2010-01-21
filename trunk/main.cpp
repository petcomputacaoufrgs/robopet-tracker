#include <stdio.h>

#include "ssl_client.h"
#include "ssl_server.h"

RoboCupSSLServer trackertoai(PORT_TRACKER_TO_AI, IP_TRACKER_TO_AI);
RoboCupSSLClient simtotracker(PORT_SIM_TO_TRACKER, IP_SIM_TO_TRACKER);

int DEBUG = 1;

void receive()
{
	SSL_WrapperPacket packet;
	if (simtotracker.receive(packet)) {
		printf("----------------------------\n");
		printf("TestClient Received ");

		if (packet.has_simtotracker())
		{
			printf("SIM-To-TRACKER!\n");
		}

/*
		if (packet.has_trackertoai())
		{
			if(DEBUG)
			{
				printf("Tracker-To-AI!\n");
				printf("Ball: <%d, %d>\n", packet.trackertoai().ball().x(),
				 							packet.trackertoai().ball().y());
			
			
				for(int i=0; i<packet.trackertoai().robots_blue_size(); i++)
					printf("Blue Robot[%d]: <%d, %d>\n", i,
											packet.trackertoai().robots_blue(i).x(),
											packet.trackertoai().robots_blue(i).y());
				for(int i=0; i<packet.trackertoai().robots_yellow_size(); i++)
					printf("Yellow Robot[%d]: <%d, %d>\n", i,
											packet.trackertoai().robots_yellow(i).x(),
											packet.trackertoai().robots_yellow(i).y());
			}
			
			_ball.setCurrentPosition(Point(packet.trackertoai().ball().x(),
				 						   packet.trackertoai().ball().y()));
				 						   
			for(int i=0; i<packet.trackertoai().robots_blue_size(); i++)
				_players[i].setCurrentPosition(Point(packet.trackertoai().robots_blue(i).x(),
												     packet.trackertoai().robots_blue(i).y()));
		}//*/
	}
}

void send()
{
	 SSL_WrapperPacket packet;

	 TrackerToAI *trackertoaiPacket = packet.mutable_trackertoai();
	 BallTracker *b = trackertoaiPacket->mutable_ball();
	 
	 b->set_x(77);
	 b->set_y(666);
	 
	 trackertoai.send(packet);
}

int main()
{
	printf("Tracker Running!\n");
	
	simtotracker.open(false);
	
	printf("Press <Enter> to open connection with client...\n");
	getchar();
	trackertoai.open();
	
	while(1) {
		receive();
		send();
	}
}
