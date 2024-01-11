# Standup Documentation

This file contains each team member's individual contributions for our biweekly standup meetings.

## Standup 2: November 2, 2023

Jenna: 
+ Created design for input file to our system
    + Added this documentation to the USER_GUIDE.md
+ Began working on the code that will process the input file
+ Created this file for standup documentation

Yili:
+ Modify and specify the overall class diagram
    + Reduce coupling
+ Began working on several pseudo code
    + Room
    + RMS
    + UI
+ If possible having working units for classes mentioned above

Thando:
+ Designed a detailed sequence diagram to show how the datalog is updated based on the occurance of important events like:
    + success and failure of robots
    + Requests from the BM
    + When robot finishes its task

Tony: 
+ come up with a mouse clicking simulation of robot cleanning
+ Add more Robot's attributes in Robot class digram
+ Write Code to similuate the probability of robot failure

## Standup 3: November 16, 2023

Jenna:
+ Continued to work on linking the yaml-cpp library
    + Still getting the 'file not found' error, even when linking running CMake
+ Brainstorming alternate input ideas but planning to go to office hours for help

Thando:
+ Created the first version of the room class
    + This was seperated into header, cpp and main files but I might have to remove the header file.
    + This version allows the user to input the rooms and sizes manually instead of using the yaml files
    + This version does not take room dimenstions. It takes small, medium, and large as room size.

Tony:
+ Working on the function that updates the location of the Robot and the moving flow of Robot
+ Working on the data log file
+ Working on the simulation of the robot cleaning process

Yili:
+ Improvements on Previous Code Base:
    + Restructure Robot Class and RMS Class
        + Merge RMS.h and RMS.cpp
        + Create sub classes of Robot Class: Mop, Vacuum, Sweeper
    + Implement Unique_ptr for RMS Class
+ What's New:
    + Begin Design for UI Work Code and Part of the Simulation
        + UI: Successfully take in inputs
        + Simulation: Hard Code Inputs to test Outputs
+ What's Next:
    + Fetch and merge Location Class
    + Fetch and merge function that updates the location of the Robot


## Standup 4: December 7, 2023

Jenna:
+ Addressed Dr. Martin's comments from the last sprint
+ Cleaned up some old files from the repo
+ Started on Catch2 testing for the RMS class
    + addRoom and addRobot are completed - add getters to RMS.hpp help with testing
    + still need to test for some of the more complex functions

Thando:
+ Built Catch2 testing files for room class.
    + might need to test for more complex functions
+ Built Catch2 testing files for robot class.
    + might need to test for more complex functions
+ Created the CMake Lists file for catch2 testing.
+ Deleted some unnecessary files in the .cache file.
+ I plan to start working on diagrams for the bonuses.

Yili:
+ Improvements on Previous Code Base:
    + Reduce redundancy for Robot Class
        + Delete some unnecessary methods, such as clean()
    + Implement Unique_ptr for RMS Class

Tony:
+ Help integrating log file and all help functions into the main
+ Integrating the Visual Simulation into the whole project
+ improve the data log output

+ What's New:
    + Merge Room Class by Thando into RMS Class
    + Merge YAML input sections by Jenna into the simulation 
    + Merge datalog output feature by Tony work for the simulation
    + Merge Probability methods by Tony into RMS Class work for the simulation
+ What's Next:
    + Create methods to output CSV files for Tony's animation
    + Have coordinates algorithm work for the simulation