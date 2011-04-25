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
