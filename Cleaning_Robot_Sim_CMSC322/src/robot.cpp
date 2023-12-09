#include "../include/librobot/robot.h"
#include <iostream>
#include <string>

// Constructor
Robot::Robot(Size robotSize, Type robotType, int processingTime, std::string loc_value)
    : size(robotSize), type(robotType), processingTime(processingTime), loc(loc_value) {}

Robot::Robot(){}

// Getter methods
Size Robot::getSize(){
    return size;
}

Type Robot::getType(){
    return type;
}

int Robot::getTime(){
    return processingTime;
}

std::string Robot::getLoc(){
    return loc;
}

int Robot::fixTime(){
    if (size == Size::Small){
        return 10;
    }
    else if (size == Size::Large){
        return 20;
    }
    return 0;
}

// Setter methods
void Robot::setSize(std::string input_size) {
    if(input_size == "small"){
        size = Size::Small;
    }
    else if (input_size == "large"){
        size = Size::Large;
    }
}

void Robot::setType(std::string input_type) {
    if(input_type == "mop"){
        type = Type::Mop;
    }
    else if (input_type == "sweeper"){
        type = Type::Sweeper;
    }
    else if (input_type == "vacuum"){
        type = Type::Vacuum;
    }
}

void Robot::setTime(int time) {
    processingTime = time;
}

void Robot::setLoc(std::string input_loc){
    loc = input_loc;
}

// Three Implementations
Mop::Mop(Size robotSize, Type robotType, int processingTime, std::string loc_value)
: Robot(robotSize, robotType, processingTime, loc_value) {}

Vacuum::Vacuum(Size robotSize, Type robotType, int processingTime, std::string loc_value)
: Robot(robotSize, robotType, processingTime, loc_value) {}

Sweeper::Sweeper(Size robotSize, Type robotType, int processingTime, std::string loc_value)
: Robot(robotSize, robotType, processingTime, loc_value) {}

// Clean Functions ** WAITING TO BE CHANGED
int Robot::clean(){
    return 0;
}

int Mop::clean(){
    return 0;
}

int Vacuum::clean(){
    return 0;
}

int Sweeper::clean(){
    return 0;
}

// Test
void Robot::print(){
    std::cout << static_cast<int>(size) << std::endl;
    std::cout << static_cast<int>(type) << std::endl;
    std::cout << processingTime << std::endl;
    std::cout << loc << std::endl;

}