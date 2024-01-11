# User Guide

### Welcome to **Silver Bullet's Cleaning Robot Management System!** We thank you for choosing our service.

Getting started with our system is quite easy, just follow the steps below and you'll be able to manage your fleet with ease in no time!

## Clone the Repo

First step is to clone this repository to your local machine.  
`$ git clone https://github.com/ur-cs-courses/course-project-silver-bullet.git `

## Config File Setup
In order for the system to manage your robot fleet, we must first know what robots you have, as well as the specifications of your building. There are three types of robots -- *sweepers, vacuums, and mops* -- and for each type they can be of two sizes -- *small or large*.

Please make sure the `config.yaml` file in the `input` directory is updated accurately with all the following information:
```
robots:
    large_sweepers: int
    small_sweepers: int
    large_vacuums: int
    small_vacuums: int
    large_mops: int
    small_mops: int
rooms:
    large: ["room_name", ...]
    medium: ["room_name", ...]
    small: ["room_name", ...]
```
Specify the number of robots of each type you have. Specify the names of the rooms in your buildings in their respective lists based on their size. If you have none of a certain type of robot, please enter `0` for that field. If you have no rooms of a certain size, you can leave an empty list as `[]`.
The file is already populated with example values for your reference.

## Running the Program

Once you've set up the configuration file to your specifications, you're ready to run the program! 

A minimum of CMake Version 3.16 is required for our program. We also recommend using VSCode, as you can download the *CMake Tools* extension and build and run with the simple click of a play button.

The `app` directory houses our top-level program, `User_Interface.cpp`. Using CMake, build and run the `user_interface` executable to run the program! Further instructions to use the program will prompt you on the command line.

## Outputs

After completion of the program, you can access the `robot_log.txt` in the `output` directory which will show you a data log of all your interactions with the program, including the assignment and completion of tasks by specific robots, and when a robot breaks and gets fixed.

### Visual Simulation

You will also see two other output files, `robot_count.csv` and `simulationInput.csv`, and the Python file `visual_simulation.py`. The two output files will be used as input for the Python file. This file will allow you to visualize your building and the robots cleaning each room!

 Make sure to navigate to the `output` directory in your terminal and that you have Python downloaded, then you should be able to run `$ python visual_simulation.py` and a web browser will pop up with your visualization!
