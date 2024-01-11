#include "../include/librobot/robot.h"
#include <iostream>
#include <string>

// Constructor
Robot::Robot(Size robotSize, Type robotType, int processingTime, std::string loc_value, int xx, int yy)
    : size(robotSize), type(robotType), processingTime(processingTime), loc(loc_value), x(xx), y(yy){}

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

int Robot::getX(){
    return x;
}

int Robot::getY(){
    return y;
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

void Robot::setX(int xx){
    x = xx;
}

void Robot::setY(int yy){
    y = yy;
}

// Three Implementations
Mop::Mop(Size robotSize, Type robotType, int processingTime, std::string loc_value, int xx, int yy)
: Robot(robotSize, robotType, processingTime, loc_value, xx, yy) {}

Vacuum::Vacuum(Size robotSize, Type robotType, int processingTime, std::string loc_value, int xx, int yy)
: Robot(robotSize, robotType, processingTime, loc_value, xx, yy) {}

Sweeper::Sweeper(Size robotSize, Type robotType, int processingTime, std::string loc_value, int xx, int yy)
: Robot(robotSize, robotType, processingTime, loc_value, xx, yy) {}

// Test
void Robot::print(){
    std::cout << static_cast<int>(size) << std::endl;
    std::cout << static_cast<int>(type) << std::endl;
    std::cout << processingTime << std::endl;
    std::cout << loc << std::endl;

}