#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include "device.h"
#include <ctype.h>

struct actu_info actuator_info1; //globally defined struct for the actuator struct with a boolean variable

void S1_handler(int sig) //handler for SIGUSR1
{
	printf("In S1_handler: Threshold has been crossed and will send info to cloud\n");
	actuator_info1.is_on = true; //since threshold was crossed, the boolean becomes true
	
	//we're gonna send the info to cloud here
	int cloud_fifo_fd; //cloud_fifo_fd integer variable
	cloud_fifo_fd = open(CLOUD_FIFO_NAME, O_WRONLY); //opens actuator FIFO with blocking and to write
    	
    if (cloud_fifo_fd != -1) { //passes if successfully opened cloud fifo
		write(cloud_fifo_fd, &actuator_info1, sizeof(actuator_info1)); //writes the struct to the cloud fifo
		close(cloud_fifo_fd); //closes cloud fifo
	}
	
	//exit(EXIT_SUCCESS);
}

void S2_handler(int sig) //handler for SIGUSR2
{
	printf("In S2_handler: Threshold has not been crossed and will not send\n");
	
	//since threshold was not crossed - we do nothing, simply print that it hasn't crossed
}

int main()
{
    struct stop_message stopper; 
    stopper.stop = 0; //(stop signal) so when stop = 1, the controller should stop

    int controller_fifo_fd, actuator_fifo_fd, sensor_fifo_fd; //controller and actuator fifo values
    struct sensor_data sensor; //sensor struct
    struct actu_info actuator_info; //actuator struct with boolean
    actuator_info.is_on = false; //boolean is set to false as we haven't done anything
    int read_res; //read_res integer
    char actuator_fifo[256]; //actuator fifo info will be stored
    signal(SIGUSR1, S1_handler); //signal for SUGUSR1
    signal(SIGUSR2, S2_handler); //signal for SIGUSR2
    pid_t pid; //pid variable
    pid = fork(); //forks the process and returns 0 to pid
    
    switch(pid)
    {
    case -1: //if pid failed
    	perror("fork failed");
	exit(1);
    case 0: //if pid is successful
    	    
    	while(1) {
    
    		//create and open the controller as Read-Only Blocking
    		mkfifo(CONTROLLER_FIFO_NAME, 0777); //create the pipe (FIFO)
    		controller_fifo_fd = open(CONTROLLER_FIFO_NAME, O_RDONLY); //open it as read-only
    
    		if (controller_fifo_fd == -1) { //passes if FIFO failed to be created
     	   		fprintf(stderr, "Controller fifo failure\n");
     	   		exit(EXIT_FAILURE); //exits upon program failure
     		}
     	
    		sleep(1); //sleeps 1 second
    	
    		do {
       			read_res = read(controller_fifo_fd, &sensor, sizeof(sensor)); //reads the sensor struct
       			if (read_res > 0) {
// In this next stage, we perform some processing on the data just read from the sensor.
// We ccheck if the random variable is bigger than the threshold

				printf("Threshold: %d\n", sensor.threshold);
								
				if (sensor.random > sensor.threshold) { //passes if the random variable surpasses threshold
					actuator_info.is_on = true; //sets boolean to true
					printf("%d sends: random %d crossed Threshold %d\n", sensor.pid, sensor.random, sensor.threshold);
				
					actuator_fifo_fd = open(ACTUATOR_FIFO_NAME, O_WRONLY); //opens actuator FIFO to write
					
					if (actuator_fifo_fd == -1) { //passes if actuator_fifo_fd fails to open
						fprintf(stderr, "Sorry, no actuator client\n");
						exit(EXIT_FAILURE); //exits upon failure
					}
					
					//now we write the struct with the boolean to the actuator
					write(actuator_fifo_fd, &actuator_info, sizeof(actuator_info));
					close(actuator_fifo_fd); //close actuator_fifo_fd FIFO
					
					//now that we have written to actuator, let's send a signal to the parent process
					stopper.stop = 1; //stop is now set to 1 so to signal to the parent that it should stop
					sensor_fifo_fd = open(SENSOR_FIFO_NAME, O_WRONLY); //opens sensor FIFO to write
					
					if (sensor_fifo_fd == -1) { //passes if sensor_fifo_fd fails to open
						fprintf(stderr, "Sorry, no sensor\n");
						exit(EXIT_FAILURE); //exits upon failure
					}
					
					//now we write the struct with the stop value to the sensor
					write(sensor_fifo_fd, &stopper, sizeof(stopper));
					close(sensor_fifo_fd); //close sensor_fifo_fd FIFO
					
					kill(getppid(), SIGUSR1); //sends a signal to the parent process
				}
			
				else {
					printf("%d sends: Random %d has not crossed threshold %d\n", sensor.pid, sensor.random, sensor.threshold); //threshold has not been crossed, so we print the information
					actuator_info.is_on = false; //boolean becomes false since it hasn't crossed
					kill(getppid(), SIGUSR2); //sends signal to parent process
				}
				
          		printf("Controller PID: %d, Sensor PID: %d\n\n", getpid(), sensor.pid);
          		
			}
            	}
            
            	while (read_res > 0);
            	close(controller_fifo_fd); //closes controller FIFO
            	unlink(CONTROLLER_FIFO_NAME); //unlinks controller FIFO
            }
    }
    pause(); //pauses parent process to wait for kill() signal
    while (1) { //kill signal comes, so parent process continues and enters while loop
   		struct sigaction act1, act2;
   		
   	//sets sigaction for SIGUSR1
    	act1.sa_flags = 0;
    	act1.sa_handler = &S1_handler;
    	sigaction(SIGUSR1, &act1, 0);
    	
    	//sets sigaction for SIGUSR2
    	act2.sa_flags = 0;
    	act2.sa_handler = &S2_handler;
    	sigaction(SIGUSR2, &act2, 0);
    	
    	    	
    	pause(); //pause again before re-entering while loop
    	
    			 //so  basically now we only re-enter the while loop if another kill() signal comes, which means it came from another threshold cross (another loop)
    }
    
    exit(EXIT_SUCCESS); //exits upon success
}

