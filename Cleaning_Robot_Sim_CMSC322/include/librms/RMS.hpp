// #include "RMS.h"
//#include "room.hpp"
#include "../libroom/room.hpp"
#include "../../src/robot.cpp"
#include <unordered_map>
#include <tuple>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <mutex>
#include <random>
#include <thread>

class RobotManagementSystem {
    private:
        std::mutex mtx;
        std::unordered_map<int, std::unique_ptr<Robot>> robots;
        std::unordered_map<int, bool> busyRobot;
        std::unordered_map<int, bool> brokenRobot;
        // std::vector<Room> room_info;
        std::unordered_map<std::string, RoomSize> roomDictionary;
        bool mtxflag = false;
        std::chrono::high_resolution_clock::time_point timer = std::chrono::high_resolution_clock::now();
        // std::unordered_map<int, std::tuple> robots_position;

    public:
        RobotManagementSystem() {}
        // Helper Function
        bool withProbability(double p) {
            std::random_device rd; // random device object
            std::mt19937 gen(rd()); //initializes it with a seed obtained from the random device.
            std::uniform_real_distribution<> dis(0, 1); // declares a uniform real distribution with a range from 0 to 1
            return dis(gen) <= p;
        }

        void setInitTime(int index, int xx, int yy){
            robots[index] -> setX(xx);
            robots[index] -> setY(yy);
        }

        void addRobot(int index, std::unique_ptr<Robot> r) {
            robots[index] = move(r);
        }

        void addRoom(std::string s, RoomSize size){
            roomDictionary[s] = size;
        }

        void dectime(int index){
            int time = robots[index] -> getTime();
            if (time > 0){
                time --;
                robots[index] -> setTime(time);
            }
        }

        void sendtoLoc(int index, std::string loc){
            robots[index] -> setLoc(loc);
        }

        bool checkFinish(int index){
            int time = robots[index] -> getTime();
            if (time == 0){
                return true;
            }
            return false;
        }

        bool checkRoom(std::string s){
            for (auto info : roomDictionary){
                if (info.first == s){
                    return true;
                }
            }
            return false;
        }

        RoomSize getRoomSize(std::string s){
            return roomDictionary[s];
        }

        int get_step(RoomSize r){
            char roomSize;
            if (r == RoomSize::Large){
                roomSize = 'L';
            }
            else if (r == RoomSize::Medium){
                roomSize = 'M';
            }
            else if (r == RoomSize::Medium)
            {
                roomSize = 'S';
            }

            int step;
            if (roomSize == 'L') {
                step = 9;
            } else if (roomSize == 'M') {
                step = 6;
            } else {
                step = 3;
            }
            return step;
        }

        int getRoomIndex(std::string s){
            int count = 0;
            for (auto info : roomDictionary){
                if ((info.first == s) & (info.second == getRoomSize(s))){
                    return count;
                }
                else if (info.second == getRoomSize(s)){
                    count ++;
                }
            }
            return count;
        }

        // test func show robot status
        void showtime(int index){
            std::cout << "robot " << index << " time left " << robots[index] -> getTime() << " loc " << robots[index] -> getLoc() << std::endl;
        }

        void add_busyRobot(int index, int time){
            busyRobot[index] = true;
            robots[index] -> setTime(time);
        }

        // Return an available robot for cleaning
        // Need a func to take in roomsize input
        // int requestCleaning(Size robotSize, Type robotType) {
        //     for (auto& info : robots) {
        //         if (info.second->getTime() == 0){
        //             if (robotSize <= info.second->getSize() & robotType == info.second->getType()){
        //                 return info.first;
        //             }
        //         }
        //     }
        //     return -1;
        // }
        
        int requestCleaning(RoomSize roomSize, Type robotType) {
            Size robotSize;
            if (roomSize == RoomSize::Small){
                robotSize = Size::Small;
                // Search for Small Robots
                for (auto& info : robots) {
                    if (info.second->getTime() == 0){
                        if (robotSize == info.second->getSize() & robotType == info.second->getType()){
                            return info.first;
                        }
                    }
                }
                // Adjust for Larger Robots
                for (auto& info : robots) {
                    if (info.second->getTime() == 0){
                        if (robotSize <= info.second->getSize() & robotType == info.second->getType()){
                            return info.first;
                        }
                    }
                }
            }
            else{
                robotSize = Size::Large;
                for (auto& info : robots) {
                    if (info.second->getTime() == 0){
                        if (robotSize <= info.second->getSize() & robotType == info.second->getType()){
                            return info.first;
                        }
                    }
                }
            }
            return -1;
        }
        

        // Turn the Thread off
        void setmtxlock (bool flag){
            mtxflag = flag;
        }

        int getTime(){
            return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - timer).count() + 1;
        }

        // Multi-threading Test Function
        void helloworld(){
            std::unique_lock<std::mutex> lock(mtx);
            while(true){
                if (mtxflag) {
                    mtx.unlock();
                    break;
                }
                std::cout << "hello world" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
                mtx.unlock();
            }
            
        }

        std::pair<int, int> startPosition(RoomSize r, int roomIndex) {
            char roomSize;
            if (r == RoomSize::Large){
                roomSize = 'L';
            }
            else if (r == RoomSize::Medium){
                roomSize = 'M';
            }
            else if (r == RoomSize::Medium)
            {
                roomSize = 'S';
            }
            
            int step = 0;
            int lowerX, lowerY;
            
            if (roomSize == 'L') {
                lowerX = 10;
                if (roomIndex % 2 == 0) {
                    lowerX = 0;
                }
                lowerY = (roomIndex / 2) * 21;
                step = 9;
            } else if (roomSize == 'M') {
                lowerX = 999;
                if (roomIndex % 3 == 1) {
                    lowerX = 7;
                } else if (roomIndex % 3 == 2) {
                    lowerX = 14;
                } else {
                    lowerX = 0;
                }
                lowerY = 10 + (roomIndex / 3) * 21;
                step = 6;
            } else {
                lowerX = 999;
                if (roomIndex % 5 == 1) {
                    lowerX = 4;
                } else if (roomIndex % 5 == 2) {
                    lowerX = 8;
                } else if (roomIndex % 5 == 3) {
                    lowerX = 12;
                } else if (roomIndex % 5 == 4) {
                    lowerX = 16;
                } else {
                    lowerX = 0;
                }
                lowerY = 17 + (roomIndex / 5) * 21;
                step = 3;
            }
            
            return std::make_pair(lowerX, lowerY);
        }

        std::pair<int, int> updateRobotLocation(int xStart, int yStart, int x, int y, int currTime, int step) {
            if (x - xStart <= step) {
                y = y + 1;
                if (currTime % (step + 1) == 0) {
                    x = x + 1;
                    y = yStart;
                }
                return std::make_pair(x, y);
            } else {
                std::cout << "finish cleaning" << std::endl;
                return std::make_pair(50, 35);
            }
        }

        void simulation(){
            std::ofstream outFile("../../output/robot_log.txt");
            std::ofstream csvFile("../../output/simulationInput.csv");
            while (true)
            {   
                if (mtxflag) {
                    mtx.unlock();
                    break;
                }
                std::unique_lock<std::mutex> lock(mtx);
                for (auto rob : busyRobot){
                    if (rob.second == true){
                        if (withProbability(0.01)){
                            // Broken Event
                            busyRobot[rob.first] = false;
                            brokenRobot[rob.first] = true;
                            int fixtime = robots[rob.first] -> fixTime();
                            robots[rob.first] -> setTime(fixtime + 1);
                            robots[rob.first] -> setX(50);
                            robots[rob.first] -> setY(35);
                            robots[rob.first] -> setLoc("Maintenance Station");
                            outFile << "Robot" << rob.first << " is broken at " << "Maintenance Station" << std::endl;
                            //std::cout << "Robot" << rob.first << " is broken at " << "Maintenance Station" << std::endl;
                        }
                        else{
                            dectime(rob.first);
                            // showtime(rob.first);
                            if (checkFinish(rob.first)){
                                busyRobot[rob.first] = false;
                                robots[rob.first] -> setLoc("hub");
                                robots[rob.first] -> setX(0);
                                robots[rob.first] -> setY(0);
                                outFile << "Robot" << rob.first << " is sent back to hub at time " << getTime() << std::endl;
                                //std::cout << "Robot" << rob.first << " is sent back to hub at time " << getTime() << std::endl;
                            }
                            else{
                                std::pair<int, int> init = startPosition(getRoomSize(robots[rob.first] -> getLoc()), getRoomIndex(robots[rob.first] -> getLoc()));
                                //std::cout << init.first << " " << init.second << std::endl;
                                // robots[rob.first] -> setX(init.first);
                                // robots[rob.first] -> setY(init.second);
                                std::pair<int, int> p = updateRobotLocation(init.first - 1, init.second, robots[rob.first] -> getX(), robots[rob.first] -> getY(), robots[rob.first] -> getTime(), get_step(getRoomSize(robots[rob.first] -> getLoc())));
                                // std::cout << p.first << " " << p.second << " " << robots[rob.first] -> getTime() << std::endl;
                                robots[rob.first] -> setX(p.first);
                                robots[rob.first] -> setY(p.second);
                                outFile << "Robot" << rob.first << " is cleaning room " << robots[rob.first] -> getLoc() << " @ location (" << p.first << " " << p.second << ") at time " << getTime() << std::endl;
                                //std::cout << "Robot" << rob.first << " is cleaning room " << robots[rob.first] -> getLoc() << " @ location (" << p.first << " " << p.second << ") at time " << getTime() << std::endl;
                            }
                        }                
                    }
                }
                // time for maintenance
                for (auto rob: brokenRobot){
                    if (rob.second == true){
                        dectime(rob.first);
                        // showtime(rob.first);
                        if (checkFinish(rob.first)){
                            brokenRobot[rob.first] = false;
                            robots[rob.first] -> setLoc("hub");
                            robots[rob.first] -> setX(0);
                            robots[rob.first] -> setY(0);
                            outFile << "Robot" << rob.first << " is fixed and sent back to hub" << std::endl;
                            //std::cout << "Robot" << rob.first << " is fixed and sent back to hub" << std::endl;
                        }
                        else{
                            //outFile << "Robot" << rob.first << " is broken at " << "Maintenance Station" << std::endl;
                           // std::cout << "Robot" << rob.first << " is broken at " << "Maintenance Station" << std::endl;
                        }
                    }
                }

                for (auto &rob: robots){
                    csvFile << rob.second -> getX() << "," << rob.second -> getY() << "," << getTime() << "," << rob.first <<std::endl;
                }
                // csvFile << "-------" << std::endl;
                mtx.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            // outFile.close();
        }

        void debug(){
            for (auto& info : robots) {
                info.second -> print();
            }
        }
        
        int getNumRobots(){
            return robots.size();
        }

        int getNumRooms(){
            return roomDictionary.size();
        }

        int getNumBusy(){
            return busyRobot.size();
        }

        std::string getLocation(int idx){
            return robots[idx] -> getLoc();
        }

        int getRobotTime(int idx){
            return robots[idx] -> getTime();
        }
};
