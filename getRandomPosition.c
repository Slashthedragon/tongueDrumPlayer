#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
void setup(){//not used in random
	}

// This generates a random GPIO pin value
int getPosition(){ 
	
	usleep(250000); // 1/4 second delay
	
	int pPins[23] = {12,6,13,21,0,16,26,20,19,0,0,0,0,0,0,0,0,0,0,0,0}; // GPIO outputs with several 0s added to create pauses.
	
	int pin = rand() % 22; 
	printf("%d \n",pPins[pin]);
	return pPins[pin];
	
}
