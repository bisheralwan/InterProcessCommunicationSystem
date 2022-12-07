#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>


//we now define the FIFO names

#define CONTROLLER_FIFO_NAME "/tmp/contr_fifo"
#define SENSOR_FIFO_NAME "/tmp/sens_%d_fifo"
#define ACTUATOR_FIFO_NAME "/tmp/actu_%d_fifo"
#define CLOUD_FIFO_NAME "/tmp/cloud_fifo"

//we define our struct that we'll use for the sensor's information
struct sensor_data {
    pid_t pid;
    char *name;
    int random;
    int threshold;
};

//we define the struct we'll use for the actuator's information
struct actuator_data {
    pid_t pid;
    char *name;
};

//we define the struct that carries the boolean variable for the actuator being activated
struct actu_info {
	_Bool is_on;
};

//we define a struct for the cloud's data
struct cloud_data {
	pid_t pid;
};

struct stop_message {
	int stop;
};
