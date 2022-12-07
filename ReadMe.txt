Brief Overview:

The sensors generate random values that are then compared to their thresholds. It sends this information along with their PIDs to the controller.
The controller then evaluates whether these random results cross the threshold, if they do, it will display that information, change the
actuator's state, and send a message to the cloud that the state has been changed along with the current now state of the actuator, and finally the 
sensor will stop. However, if they don't, then the controller will display that the threshold was not crossed, and will therefore not chnage the 
actuator's state, and the sensor will keep going until the threshold crosses. Sensors are tracked through their PIDs throughout their communication 
with the controller to understand which sensor the information is coming from, so it knows how to deal with the informatiom accordingly, allowing 
for multiple sensors to be used at once.

The cloud will display the changes made to the actuator's state through print statements. The actuator will display the actuator's behavior 
changes through print statements. The controller displays the sensor's PID to track which sensor the random result is coming from, the sensor's
threshold and whether the threshold was crossed, the controller's PID, and if the controller will be sending info to the cloud or not. The sensors 
will display their PID, name, threshold, and the random result.

----------------------------------------------------------------------------------------------------------------------------------------------------------

In order to run the code, please follow these Stages:


STAGE 1: Compiling the programs

1. Save all files in your directory
2. Open the terminal 
3. Type in the chmod command and hit enter: chmod +x sercli
4. Run the script and hit enter by typing: ./sercli
5. Now you have ran the script, this will compile the processes and 5 xterm terminals will pop up - 1 for each process


STAGE 2: Setting up the programs

1. Use 2 xterm terminals for your 2 sensors
2. Use 1 xterm terminal for your controller
3. Use 1 xterm terminal for your actuator
4. Use 1 xterm terminal for your cloud


STAGE 3: Running the programs

1. Run the cloud by typing in: ./cloud
2. Run the actuator with an actuator name such as "button" by typing in: ./actuator button
   Note: You can name the actuator whatever you like, as long as it's one word, or 2+ words surrounded by single quotation (' *argument* ')
3. Run the controller by typing in: ./controller
4. Run the first sensor with a name and a threshold such as "light 25" by typing in: ./sensor light 25
   Note: You can name the sensor whatever you like, as long as it's one word, or 2+ words surrounded by single quotation (' *argument* ')
5. Run the second sensor with a name and a threshold such as "AC 12" by typing in: ./sensor AC 12
   Note: You can name the sensor whatever you like, as long as it's one word, or 2+ words surrounded by single quotation (' *argument* ')

----------------------------------------------------------------------------------------------------------------------------------------------------------

THANK YOU!
