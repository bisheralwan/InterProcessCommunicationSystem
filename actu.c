#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include "device.h"
#include <ctype.h>

int main(int argc, char *argv[]) {

	struct actuator_data actuator; //struct for the actuator
	actuator.pid = getpid(); //actuator's pid
	actuator.name = argv[1]; //actuator's name
	
	struct actu_info actuator_info; //struct for the actuator's boolean variable
	
	int read_res; //read integer value
	int actuator_fifo_fd; //actuator_fifo_fd value 
	
	printf("The actuator's pid is %d\n", actuator.pid);
	printf("The actuator's name is %s\n", actuator.name);
	
	int turn=0; //will track whether it's time to turn the actuator ON or OFF (they take turns based on even/odd)	
	while(1) {
		mkfifo(ACTUATOR_FIFO_NAME, 0777); //makes actuator fifo
		actuator_fifo_fd = open(ACTUATOR_FIFO_NAME, O_RDONLY); //opens actuator fifo to read-only and blocking
		
		if (actuator_fifo_fd == -1) { //passes if actuator failed to open
			fprintf(stderr, "Sorry, no actuator\n"); //let's us know about the above failure
			exit(EXIT_FAILURE); //exits program upon failure
		}
        
        	do {
        		read_res = read(actuator_fifo_fd, &actuator_info, sizeof(actuator_info)); //reads the actuator_info struct with the bool
        		if (read_res > 0) { //passes if successfully read
        			sleep(1); //sleeps for 1 second
        			if (actuator_info.is_on) { //passes on if the bool passes
        		
        				if (turn%2 == 0) { //checks turn - passes if it's ON's turn
        					printf("The %s turned ON\n", actuator.name);
        				}
        				else { //checks turn - passes if it's OFF's turn
        					printf("The %s turned OFF\n", actuator.name);
        				}
        				turn++; //increments turn
        			}
        		}
        	}
        	while (read_res > 0);
    		close(actuator_fifo_fd); //closes the actuator_fifo_fd
    		unlink(ACTUATOR_FIFO_NAME); //unlinks the ACTUATOR_FIFO_NAME
        }
        exit(EXIT_SUCCESS); //exits upons success
}
