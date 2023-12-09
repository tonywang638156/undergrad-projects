#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Robot {
    int robotID;
    int x;
    int y;
};

pair<int, int> startPosition(char roomSize, int roomIndex) {
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
    
    return make_pair(lowerX, lowerY);
}

pair<int, int> updateRobotLocation(int xStart, int yStart, int x, int y, int currTime, int step) {
    if (x - xStart <= step) {
        y = y + 1;
        if (currTime % (step + 1) == 0) {
            x = x + 1;
            y = yStart;
        }
        return make_pair(x, y);
    } else {
        cout << "finish cleaning" << endl;
        return make_pair(50, 35);
    }
}

int main() {
    int currTime = 0;
    int robotID = 88888;
    char roomSize = 'L';
    int roomIndex = 0;
    pair<int, int> startPositionResult = startPosition(roomSize, roomIndex);
    int initX = startPositionResult.first - 1;
    int initY = startPositionResult.second;
    int step = 0;
    int x = initX;
    int y = initY;
    
    if (roomSize == 'L') {
        step = 9;
    } else if (roomSize == 'M') {
        step = 6;
    } else {
        step = 3;
    }
    
    int totalIterations = 120;
    vector<int> xList;
    vector<int> yList;
    vector<int> tList;
    
    ofstream outFile("robot_log.txt");
    
    while (currTime <= totalIterations) {
        pair<int, int> updatedLocation = updateRobotLocation(initX, initY, x, y, currTime, step);
        x = updatedLocation.first;
        y = updatedLocation.second;
        
        outFile << "Robot" << robotID << " is cleaning room " << roomSize << roomIndex << " @ location (" << x << " " << y << ") at time " << currTime << endl;
        cout << "Robot" << robotID << " is cleaning room " << roomSize << roomIndex << " @ location (" << x << " " << y << ") at time " << currTime << endl;
        
        xList.push_back(x);
        yList.push_back(y);
        tList.push_back(currTime);
        
        currTime = currTime + 1;
    }
    
    outFile.close();
    
    ofstream csvFile("simulationInput.csv");
    csvFile << "x,y,t" << endl;
    
    for (int i = 0; i < xList.size(); i++) {
        csvFile << xList[i] << "," << yList[i] << "," << tList[i] << endl;
    }
    
    csvFile.close();
    
    return 0;
}