// #include "RMS.h"
//#include "room.hpp"
#include "../libroom/room.hpp"
#include "../../src/robot.cpp"
#include <unordered_map>
#include <tuple>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include <mutex>

class RobotManagementSystem {
    private:
        std::mutex mtx;
        std::unordered_map<int, std::unique_ptr<Robot>> robots;
        std::unordered_map<int, bool> busyRobot;
        std::unordered_map<int, bool> brokenRobot;
        // std::vector<Room> room_info;
        std::unordered_map<std::string, RoomSize> roomDictionary;
        bool mtxflag = false;
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

        void simulation(){
            while (true)
            {   
                if (mtxflag) {
                    mtx.unlock();
                    break;
                }
                std::unique_lock<std::mutex> lock(mtx);
                for (auto rob : busyRobot){
                    if (rob.second == true){
                        if (withProbability(0.05)){
                            // Broken Event
                            busyRobot[rob.first] = false;
                            brokenRobot[rob.first] = true;
                            int fixtime = robots[rob.first] -> fixTime();
                            robots[rob.first] -> setTime(fixtime + 1);
                            robots[rob.first] -> setLoc("Maintenance Station");
                        }
                        else{
                            dectime(rob.first);
                            showtime(rob.first);
                            if (checkFinish(rob.first)){
                                busyRobot[rob.first] = false;
                                robots[rob.first] -> setLoc("hub");
                            }
                        }                
                    }
                }
                // time for maintenance
                for (auto rob: brokenRobot){
                    if (rob.second == true){
                        dectime(rob.first);
                        showtime(rob.first);
                        if (checkFinish(rob.first)){
                            brokenRobot[rob.first] = false;
                            robots[rob.first] -> setLoc("hub");
                        }
                    }
                }
                mtx.unlock();
                std::this_thread::sleep_for(std::chrono::seconds(3));
            }
        }

        void debug(){
            for (auto& info : robots) {
                info.second -> print();
            }
        }

};
