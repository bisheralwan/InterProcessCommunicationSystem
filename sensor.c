#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include "device.h"
#include <ctype.h>

int main(int argc, char *argv[]) {

	struct stop_message stopper;
	struct sensor_data sensor; //struct for the sensor
	sensor.pid = getpid(); //sensor's pid
	sensor.name = argv[1]; //sensor's name
	sensor.threshold = atoi(argv[2]); //sensor's threshold
	int read_res;
	
	printf("The sensor's pid is %d\n", sensor.pid);
	printf("The sensor's name is %s\n", sensor.name);
	printf("The sensor's threshold is %d\n", sensor.threshold);
	
	int controller_fifo_fd, sensor_fifo_fd; //controller_fifo_fd value
	
	while(1) {
		sleep(1); //sleeps for 1 second
		sensor.random = rand() % 100; //a number between 0 and 100
		controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_WRONLY); //opens controller to write-only and blocking
		if (controller_fifo_fd != -1) { //passes if we've successfully opened controller
			printf("The random result is %d and the threshold is %d\n", sensor.random, sensor.threshold);
			// The sensor struct is written to the controller.
	    		write(controller_fifo_fd, &sensor, sizeof(sensor)); //writes sensor struct to controller
	    		close(controller_fifo_fd); //closes controller_fifo_fd 
	    	}
	    	
	    	else { //pass if we've unsuccessfully opened controller
	    		fprintf(stderr, "Sorry, no controller server\n"); //let's us know there's no controller server
	    		exit(EXIT_FAILURE); //exits upon program failure
	    	}
	    	
	    	if (sensor.random > sensor.threshold) {
	    	
	    		//make and open the sensor pipe as Read-Only Blocking
    			mkfifo(SENSOR_FIFO_NAME, 0777); //create the pipe (FIFO)
    			sensor_fifo_fd = open(SENSOR_FIFO_NAME, O_RDONLY); 
    
    			if (sensor_fifo_fd == -1) { //passes if FIFO failed to be created
     	   			fprintf(stderr, "Controller fifo failure\n");
     	   			exit(EXIT_FAILURE); //exits upon program failure
     			}
     	
    	
    			read_res = read(sensor_fifo_fd, &stopper, sizeof(stopper)); //reads the sensor struct
    			if (read_res > 0) {
    				if (stopper.stop == 1) {
    					printf("ACK\n");
    					printf("\nThreshold has been crossed, this sensor will now stop!\n");
    					exit(EXIT_SUCCESS);
    				}
    			}
    		}
	    	
	}
	exit(EXIT_SUCCESS); //exits upon program success
}
