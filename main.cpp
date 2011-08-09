#include "tracker.h"

int main(int argc, char **argv)
{
	printf("Tracker Running!\n");
	
	bool sim = false;
	bool verbose = true;
	
    if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			string param; 
			param.assign(argv[i]);
			if(param.compare("-sim") == 0) sim = true;
			if(param.compare("-q") == 0) verbose = false;
			
		}
    }
    
    if(sim)
		printf("Tracker will receive from SIM.\n");
	else
		printf("Tracker will receive from VISION.\n");
    
    Tracker tracker(sim, verbose);
    
	while(1) {
	
		tracker.receive();
		tracker.track();
		tracker.send();
		if(verbose)
			system("clear");
		usleep(10000);
	}

    return 0;
}
