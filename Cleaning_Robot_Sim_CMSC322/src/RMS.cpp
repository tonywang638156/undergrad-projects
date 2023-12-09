#include "RMS.h"
// # include "robot.cpp"
#include <iostream>
#include <unordered_map>
#include <string>

RobotManagementSystem::RobotManagementSystem() {
}

// use for initialization
void RobotManagementSystem::addRobot(int index, Robot* r) {
    robots[index] = r;
}

// Robot* getRobot(const int index) {
//     auto it = robots.find(index);
//     if (it != robots.end()) {
//         return &it->second;
//     }
//     return nullptr;
// }

int RobotManagementSystem::requestCleaning(Size robotSize, Type robotType) {
    // check if processtime = 0
    for (auto info : robots) {
        if (info.second->getTime() == 0){
            if (robotSize <= info.second->getSize() & robotType == info.second->getType()){
                return info.first;
            }
        }
    }
    return -1;
}