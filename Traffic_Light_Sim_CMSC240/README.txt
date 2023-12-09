Design Decision:
We use each row in degree 2 vector (matrix) to represent each bound so that we can update all vehicles at once.
By using our own methods (moveStraight(), rightTurn()), given different decision, we are able to place all the vehicle onto corresponding lanes and make correct decision before enter the intersection.
The logic behind updating the locations of the vehicle is that everytime we increment the time click, we only move the tail of each vehicle to the front of the head and keep the rest part of the vehicle unchanged. For new vehicle coming into the lane, we increment the length of the vehicle. For vehicles moving out of the lane, we decrement the length of each vehicles.
And each time when we contruct a vehicle, we use vectors, (waitlist, locations, oldVehicles), to store the information of each vehicle, (address, location, and whether it's existing vechile or not).
Also, we use our own method allowToPass() to check whether each vehicle can successfully pass the intersection by the light turning red. The logic behind this is to using the variable remainingTime to store the number of time clicks we have left for each vehicle to make decision.


The way to compile and run:
To compile our program, anytime the FinalProject.cpp, VehicleBase.cpp, VehicleBase.h, Animator.cpp, or Animator.h is changed, use the makefile that we provide.
To run our program, use  ./simulation [input file] [initial seed]  as the format of the command line arguements.
Our executable is named simulation.

