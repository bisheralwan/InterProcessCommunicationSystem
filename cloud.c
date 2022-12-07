#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include "device.h"
#include <ctype.h>

int main(int argc, char *argv[]) {
	
	int cloud_fifo_fd; //cloud FIFO integer value
	int read_res; //read value
	int turn = 0; //tracks turns
    	struct actu_info actuator_info; //actuator struct with the boolean
    	
	struct cloud_data cloud; //struct for cloud's data
	cloud.pid = getpid(); //cloud's pid
	
	while(1) {
		//create and open the cloud as Read-Only Blocking
    		mkfifo(CLOUD_FIFO_NAME, 0777); //makes the cloud pipe (FIFO)
    		cloud_fifo_fd = open(CLOUD_FIFO_NAME, O_RDONLY); //open it as read-only and blocking
    		
    		
    		if (cloud_fifo_fd == -1) { //passes if FIFO failed to be created
     	 	  	fprintf(stderr, "Cloud fifo failure\n");
     	 	  	exit(EXIT_FAILURE); //exits process
     		
     		}
     		     		
     		do {
        		read_res = read(cloud_fifo_fd, &actuator_info, sizeof(actuator_info)); //reads actuator struct with the boolean value
        		if (read_res > 0) { //pass if read successfully
        			sleep(1); //sleeps 1 second
        			
        			if (turn%2==0) { //checks turn - even number for ON's turn
        				printf("The threshold has been crossed and the actuator has been activated. The actuator's state is now ON.\n");
        			}
        			else { //checks turn - odd number for OFF's turn
        				printf("The threshold has been crossed and the actuator has been activated. The actuator's state is now OFF.\n");
        			}
        			turn++; //increments turn
        		
        		}
        	}
        	
        	while (read_res > 0); //while
    		close(cloud_fifo_fd); //closes cloud fifo
    		unlink(CLOUD_FIFO_NAME); //unlinks cloud fifo
     	}
     	
     	exit(EXIT_SUCCESS); //exits upon success
	
}
