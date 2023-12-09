#include <iostream>
#include "Animator.h"
#include "VehicleBase.h"
#include <fstream>
#include <string>
#include <random>

using namespace std;

vector<vector<VehicleBase*>> moveStraight(VehicleBase* vptr, int currL, 
										  vector<vector<VehicleBase*>> allBounds, int halfSize);
										  
vector<vector<VehicleBase*>> rightTurn(VehicleBase* vptr, int currL, 
									   vector<vector<VehicleBase*>> allBounds, int halfSize);
										  
bool allowToPass(Direction d, bool redEW, int remainingTimeEW, int halfSize, bool redNS, int remainingTimeNS,
			 	 int j, vector<VehicleBase*> waitlist, vector<int> locations, Decision decision);
 
 int main(int argc, char* argv[]) {	
	 //read input
	if (argc != 3) {
        cerr << "Usage: " << argv[0] << " [input text filename] [initial seed] " << endl;
        exit(1);
    }

    ifstream infile(argv[1]);

    if (!infile) {
        cerr << "Error: could not open input file: " << argv[1] << endl;
        exit(1);
    }
    
    int initial_seed;
    initial_seed = stoi(argv[2]);
    
	string line;
    int maxSim;
    int numSectionsBeforeInter;
    int greenVertical;
    int yellowVertical;
    int greenHorizontal;
    int yellowHorizontal;
    double probNewVehicleNorth;
    double probNewVehicleSouth;
    double probNewVehicleEast;
    double probNewVehicleWest;
    double propCars;
    double propSUVs;
    double propRightTurnCars;
    double propLeftTurnCars;
    double propRightTurnSUVs;
    double propLeftTurnSUVs;
    double propRightTurnTrucks;
    double propLeftTurnTrucks;

    //stringstream ss(line);
    while (getline(infile, line)) {
        //stringstream ss(line);
        int strLen = line.length();
        //label is the data tag
        string label = "";
        //data is the int or double associated with the label
        string data = "";
        string dataTemp = "";
        for (int i = 0; i < strLen; i++) {
            if (isspace(line[i])) {
                break;

            }
            else {
                label.push_back(line[i]);
            }
            //lineNum++;
        }
        
        for (int i = strLen - 1; i > 0; i--) {
            if (isspace(line[i])) {
                break;
            }
            dataTemp.push_back(line[i]);
        }
        for (int i = dataTemp.length()-1; i > -1; i--){
            data.push_back(dataTemp[i]);
        }
        if (label == "maximum_simulated_time:") {
            maxSim = stoi(data);
        }
        if (label == "number_of_sections_before_intersection:") {
            numSectionsBeforeInter = stoi(data);
        }
        if (label == "green_north_south:") {
            greenVertical = stoi(data);
        }
        if (label == "yellow_north_south:") {
            yellowVertical = stoi(data);
        }
        if (label == "green_east_west:") {
            greenHorizontal = stoi(data);
        }
        if (label == "yellow_east_west:") {
            yellowHorizontal = stoi(data);
        }
        if (label == "prob_new_vehicle_northbound:") {
            probNewVehicleNorth = stod(data);
        }
        if (label == "prob_new_vehicle_southbound:") {
            probNewVehicleSouth = stod(data);
        }
        if (label == "prob_new_vehicle_eastbound:") {
            probNewVehicleEast = stod(data);
        }
        if (label == "prob_new_vehicle_westbound:") {
            probNewVehicleWest = stod(data);
        }
        if (label == "proportion_of_cars:") {
            propCars = stod(data);
        }
        if (label == "proportion_of_SUVs:") {
            propSUVs = stod(data);
        }
        if (label == "proportion_right_turn_cars:") {
            propRightTurnCars = stod(data);
        }
        if (label == "proportion_left_turn_cars:") {
            propLeftTurnCars = stod(data);
        }
        if (label == "proportion_right_turn_SUVs:") {
            propRightTurnSUVs = stod(data);
        }
        if (label == "proportion_left_turn_SUVs:") {
            propLeftTurnSUVs = stod(data);
        }
        if (label == "proportion_right_turn_trucks:") {
            propRightTurnTrucks = stod(data);
        }
        if (label == "proportion_left_turn_trucks:") {
            propLeftTurnTrucks = stod(data);
        }
    }
    
    propLeftTurnTrucks = 0;
    propLeftTurnSUVs = 0;
    propLeftTurnCars = 0;
    
    Animator::MAX_VEHICLE_COUNT = 999;

    int halfSize = numSectionsBeforeInter;
    Animator anim(halfSize);
    vector<VehicleBase*> eastbound(halfSize * 2 + 2, nullptr);
    vector<VehicleBase*> westbound(halfSize * 2 + 2, nullptr);
    vector<VehicleBase*> northbound(halfSize * 2 + 2, nullptr);
    vector<VehicleBase*> southbound(halfSize * 2 + 2, nullptr);
	
	vector<vector<VehicleBase*>> allBounds{eastbound, westbound, northbound, southbound};
    char dummy;
    
    anim.setLightEastWest(LightColor::green);
    anim.setLightNorthSouth(LightColor::red);
	// at time 0, no vehicle
    anim.setVehiclesNorthbound(northbound);
    anim.setVehiclesWestbound(westbound);
    anim.setVehiclesSouthbound(southbound);
    anim.setVehiclesEastbound(eastbound);
    anim.draw(0);
    cin.get(dummy);
    
    // random number generator to mimic the probability
    mt19937 randomNumberGenerator;
    uniform_real_distribution<double> rand_double(0,1);
    randomNumberGenerator.seed(initial_seed);
    double randNum;
    
    // light switching variables    
    bool redEW = false;
    bool redNS = true;
    int greenDurationEW = greenHorizontal;
    int yellowDurationEW = yellowHorizontal;
    int greenDurationNS = greenVertical;
    int yellowDurationNS = yellowVertical;
    int oneCycle = greenDurationEW + yellowDurationEW + greenDurationNS + yellowDurationNS;
    int remainingTimeEW = greenDurationEW + yellowDurationEW;
    int remainingTimeNS = greenDurationNS + yellowDurationNS + 1;
    
    // variables that holds each vehicle's address, location
    vector<int> locations; // store the location or index for each vehicle
    vector<VehicleBase*> waitlist; // store vehicles in order
    vector<bool> oldVehicles; // checking whether the vehicle is new or already existing

    for (int i = 0; i < maxSim; i++) {
    	//light switching
    	// control east-west yellow light
    	if ((i % oneCycle) == (greenDurationEW - 1)) {    		
    		anim.setLightEastWest(LightColor::yellow);
    	}
    	// control east-west red light
    	if ((i % oneCycle) == (greenDurationEW - 1 + yellowDurationEW)) {
    		anim.setLightEastWest(LightColor::red);
    		anim.setLightNorthSouth(LightColor::green);
    		redEW = true;
    		redNS = false;
    		remainingTimeEW = greenDurationEW + yellowDurationEW + 1;
    	}
    	// control north-south yellow light
    	if ((i % oneCycle) == (greenDurationEW - 1 + yellowDurationEW + greenDurationNS)) {    		
    		anim.setLightNorthSouth(LightColor::yellow);
    	}
    	// control north-south red light
    	if ((i % oneCycle) == (greenDurationEW - 1 + yellowDurationEW + greenDurationNS + yellowDurationNS)) {
    		anim.setLightEastWest(LightColor::green);
    		anim.setLightNorthSouth(LightColor::red);
    		redEW = false;
    		redNS = true;
    		remainingTimeNS = greenDurationNS + yellowDurationNS + 1;
    	}
    	// updating the remaining time for east-west direction
    	if (!redEW) {
    		remainingTimeEW--;
    	} 
    	if (!redNS) {
    		remainingTimeNS--;
    	}
    	
    	VehicleBase* vptr = new VehicleBase();
    	
    	// east
    	randNum = rand_double(randomNumberGenerator);
    	if (randNum < probNewVehicleEast) {
    		randNum = rand_double(randomNumberGenerator);
    		if (randNum < propCars) { // car
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnCars - propLeftTurnCars)) {
    				vptr = new VehicleBase(VehicleType::car, Direction::east, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::car, Direction::east, Decision::right);
    			} 
    		} else if (randNum < propSUVs + propCars) { // suv
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnSUVs - propLeftTurnSUVs)) {
    				vptr = new VehicleBase(VehicleType::suv, Direction::east, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::suv, Direction::east, Decision::right);
    			} 
    		} else { // truck
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnTrucks - propLeftTurnTrucks)) {
    				vptr = new VehicleBase(VehicleType::truck, Direction::east, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::truck, Direction::east, Decision::right);
    			} 
    		}
    		waitlist.push_back(vptr);
            locations.push_back(0);
            oldVehicles.push_back(false);
        } 
        
        // west
        randNum = rand_double(randomNumberGenerator);
        if (randNum < probNewVehicleWest) {
    		randNum = rand_double(randomNumberGenerator);
    		if (randNum < propCars) { // car
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnCars - propLeftTurnCars)) {
    				vptr = new VehicleBase(VehicleType::car, Direction::west, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::car, Direction::west, Decision::right);
    			} 
    		} else if (randNum < propSUVs + propCars) { // suv
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnSUVs - propLeftTurnSUVs)) {
    				vptr = new VehicleBase(VehicleType::suv, Direction::west, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::suv, Direction::west, Decision::right);
    			} 
    		} else { // truck
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnTrucks - propLeftTurnTrucks)) {
    				vptr = new VehicleBase(VehicleType::truck, Direction::west, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::truck, Direction::west, Decision::right);
    			} 
    		}
    		waitlist.push_back(vptr);
            locations.push_back(0);
            oldVehicles.push_back(false);
        }
        
        // north
        randNum = rand_double(randomNumberGenerator);
        if (randNum < probNewVehicleNorth) {
    		randNum = rand_double(randomNumberGenerator);
    		if (randNum < propCars) { // car
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnCars - propLeftTurnCars)) {
    				vptr = new VehicleBase(VehicleType::car, Direction::north, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::car, Direction::north, Decision::right);
    			}            	
    		} else if (randNum < propSUVs + propCars) { // suv
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnSUVs - propLeftTurnSUVs)) {
    				vptr = new VehicleBase(VehicleType::suv, Direction::north, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::suv, Direction::north, Decision::right);
    			} 
    		} else { // truck
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnTrucks - propLeftTurnTrucks)) {
    				vptr = new VehicleBase(VehicleType::truck, Direction::north, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::truck, Direction::north, Decision::right);
    			} 
    		}
    		waitlist.push_back(vptr);
            locations.push_back(0);
            oldVehicles.push_back(false);
        }
        
        // south
        randNum = rand_double(randomNumberGenerator);
        if (randNum < probNewVehicleSouth) {
    		randNum = rand_double(randomNumberGenerator);
    		if (randNum < propCars) { // car
				randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnCars - propLeftTurnCars)) {
    				vptr = new VehicleBase(VehicleType::car, Direction::south, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::car, Direction::south, Decision::right);
    			}
    		} else if (randNum < propSUVs + propCars) { // suv
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnSUVs - propLeftTurnSUVs)) {
    				vptr = new VehicleBase(VehicleType::suv, Direction::south, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::suv, Direction::south, Decision::right);
    			} 
    		} else { // truck
    			randNum = rand_double(randomNumberGenerator);
    			if (randNum < (1 - propRightTurnTrucks - propLeftTurnTrucks)) {
    				vptr = new VehicleBase(VehicleType::truck, Direction::south, Decision::straight);
    			} else {
    				vptr = new VehicleBase(VehicleType::truck, Direction::south, Decision::right);
    			}
    		}
    		waitlist.push_back(vptr);
            locations.push_back(0);
           	oldVehicles.push_back(false);
        }   
    	
    	// process each vehicle and update its location and get rid of the overlapping vehicles
    	for (int j = 0; j < waitlist.size(); j++) {
    		Direction d = waitlist[j]->getVehicleOriginalDirection();
   			if (oldVehicles[j]) { // move the already existing vehicles first
   				if (allowToPass(d, redEW, remainingTimeEW, halfSize, redNS, remainingTimeNS,
			 	 	j, waitlist, locations, Decision::straight) ||
   					allowToPass(d, redEW, remainingTimeEW, halfSize - 1, redNS, remainingTimeNS,
			 	 	j, waitlist, locations, Decision::right) ||
   					((d == Direction::east && allBounds[0][locations[j] + 1] == nullptr ||
					d == Direction::west && allBounds[1][locations[j] + 1] == nullptr ||
					d == Direction::north && allBounds[2][locations[j] + 1] == nullptr ||
					d == Direction::south && allBounds[3][locations[j] + 1] == nullptr) &&
					locations[j] < (halfSize-1))) {
   					locations[j]++;
   					if (waitlist[j]->getVehicleDecision() == Decision::right) {
   						allBounds = rightTurn(waitlist[j], locations[j], allBounds, halfSize);
   					} else if (waitlist[j]->getVehicleDecision() == Decision::straight) {
   						allBounds = moveStraight(waitlist[j], locations[j], allBounds, halfSize);
   					}
   				}
			} else { // getting the new coming vehicles on lanes
				if (d == Direction::east && allBounds[0][0] == nullptr ||
					d == Direction::west && allBounds[1][0] == nullptr ||
					d == Direction::north && allBounds[2][0] == nullptr ||
					d == Direction::south && allBounds[3][0] == nullptr) {
					allBounds = moveStraight(waitlist[j], 0, allBounds, halfSize);
				} else { // ignore vehicles when there are already vehicles on lanes.
					waitlist.erase(waitlist.begin() + j);
           			locations.erase(locations.begin() + j);
           			oldVehicles.erase(oldVehicles.begin() + j);
           			j--;
				}
  			}
    	}    	
    	
    	// set bounds and draw animator
    	anim.setVehiclesEastbound(allBounds[0]);
    	anim.setVehiclesWestbound(allBounds[1]);
    	anim.setVehiclesNorthbound(allBounds[2]);
    	anim.setVehiclesSouthbound(allBounds[3]);
    	anim.draw(i+1);
    	cin.get(dummy);
    	
		// turn the new coming vehicles into already existing vehicles
    	fill(oldVehicles.begin(), oldVehicles.end(), true);
	}
    
 	// reset each bounds
 	southbound.assign(halfSize * 2 + 2, nullptr); 
 	northbound.assign(halfSize * 2 + 2, nullptr);
    eastbound.assign(halfSize * 2 + 2, nullptr);
    westbound.assign(halfSize * 2 + 2, nullptr); 
    // switch the light
    anim.setLightNorthSouth(LightColor::green);
    anim.setLightEastWest(LightColor::red);
}

vector<vector<VehicleBase*>> moveStraight(VehicleBase* vptr, int currL,
										  vector<vector<VehicleBase*>> allBounds, int halfSize) {
	// vehicles move into the lanes
	Direction d = vptr->getVehicleOriginalDirection();
	if (currL < vptr->getLength()) {
		if (d == Direction::east && allBounds[0][currL] == nullptr) { // check whether it has spaces 
			allBounds[0][currL] = vptr;
   		} else if (d == Direction::west && allBounds[1][currL] == nullptr) {
			allBounds[1][currL] = vptr;
   		} else if (d == Direction::north && allBounds[2][currL] == nullptr) {
			allBounds[2][currL] = vptr;
   		} else if (d == Direction::south && allBounds[3][currL] == nullptr) {
			allBounds[3][currL] = vptr;    		
    	}
    } else if (currL <= (2 * halfSize + 1)) { // vehicles move on the lanes
    	if (d == Direction::east && allBounds[0][currL] == nullptr) {
        	allBounds[0][currL] = vptr;
    		allBounds[0][currL-vptr->getLength()] = nullptr;
    	} else if (d == Direction::west && allBounds[1][currL] == nullptr) {
        	allBounds[1][currL] = vptr;
    		allBounds[1][currL-vptr->getLength()] = nullptr;
   		} else if (d == Direction::north && allBounds[2][currL] == nullptr) {
        	allBounds[2][currL] = vptr;
    		allBounds[2][currL-vptr->getLength()] = nullptr;
		} else if (d == Direction::south && allBounds[3][currL] == nullptr) {
        	allBounds[3][currL] = vptr;
    		allBounds[3][currL-vptr->getLength()] = nullptr;
   		}
    } else { // vehicles move out of the lanes
    	if (currL < (2 * halfSize + 2) + vptr->getLength()) { // 18represents the size of the lane
    		if (d == Direction::east) {
    			allBounds[0][currL - vptr->getLength()] = nullptr;
    		} else if (d == Direction::west) {
   				allBounds[1][currL - vptr->getLength()] = nullptr;
   			} else if (d == Direction::north) {
   				allBounds[2][currL - vptr->getLength()] = nullptr;
   			} else if (d == Direction::south) {
   			    allBounds[3][currL - vptr->getLength()] = nullptr;
    		}
    	}
    }
    // return the changing bounds
	return allBounds;
}

vector<vector<VehicleBase*>> rightTurn(VehicleBase* vptr, int currL,
									   vector<vector<VehicleBase*>> allBounds, int halfSize) {
	// vehicles move into the lanes
	Direction d = vptr->getVehicleOriginalDirection();
	if (currL < vptr->getLength()) {
		if (d == Direction::east && allBounds[0][currL] == nullptr) { // check whether it has spaces 
			allBounds[0][currL] = vptr;
   		} else if (d == Direction::west && allBounds[1][currL] == nullptr) {
			allBounds[1][currL] = vptr;
   		} else if (d == Direction::north && allBounds[2][currL] == nullptr) {
			allBounds[2][currL] = vptr;
   		} else if (d == Direction::south && allBounds[3][currL] == nullptr) {
			allBounds[3][currL] = vptr;    		
    	}
    } else if (currL < halfSize) { // vehicles move on the lanes
    	if (d == Direction::east && allBounds[0][currL] == nullptr) {
        	allBounds[0][currL] = vptr;
    		allBounds[0][currL-vptr->getLength()] = nullptr;
    	} else if (d == Direction::west && allBounds[1][currL] == nullptr) {
        	allBounds[1][currL] = vptr;
    		allBounds[1][currL-vptr->getLength()] = nullptr;
   		} else if (d == Direction::north && allBounds[2][currL] == nullptr) {
        	allBounds[2][currL] = vptr;
    		allBounds[2][currL-vptr->getLength()] = nullptr;
		} else if (d == Direction::south && allBounds[3][currL] == nullptr) {
        	allBounds[3][currL] = vptr;
    		allBounds[3][currL-vptr->getLength()] = nullptr;
   		}
    } else if (currL < halfSize + vptr->getLength()) {
    	if (d == Direction::east) {
    		allBounds[0][currL - vptr->getLength()] = nullptr; // vehicles on east decrements in length
    		allBounds[3][currL + 1] = vptr; //turn right and show up on the south lane
    	} else if (d == Direction::west) {
   			allBounds[1][currL - vptr->getLength()] = nullptr;
   			allBounds[2][currL + 1] = vptr;
   		} else if (d == Direction::north) {
   			allBounds[2][currL - vptr->getLength()] = nullptr;
   			allBounds[0][currL + 1] = vptr;
   		} else if (d == Direction::south) {
   			allBounds[3][currL - vptr->getLength()] = nullptr;
   			allBounds[1][currL + 1] = vptr;
    	}    	
    } else if (currL <= (2 * halfSize + 1)) { // vehicles move on the lanes
    	if (d == Direction::east) {
        	allBounds[3][currL + 1] = vptr;
    		allBounds[3][currL-vptr->getLength() + 1] = nullptr;
    	} else if (d == Direction::west && allBounds[1][currL] == nullptr) {
        	allBounds[2][currL + 1] = vptr;
    		allBounds[2][currL-vptr->getLength() + 1] = nullptr;
   		} else if (d == Direction::north && allBounds[2][currL] == nullptr) {
        	allBounds[0][currL + 1] = vptr;
    		allBounds[0][currL-vptr->getLength() + 1] = nullptr;
		} else if (d == Direction::south && allBounds[3][currL] == nullptr) {
        	allBounds[1][currL + 1] = vptr;
    		allBounds[1][currL-vptr->getLength() + 1] = nullptr;
   		}
    } else { // vehicles move out of the lanes
    	if (currL < (2 * halfSize + 2) + vptr->getLength()) { // 18represents the size of the lane
    		if (d == Direction::east) {
    			allBounds[3][currL - vptr->getLength() + 1] = nullptr;
    		} else if (d == Direction::west) {
   				allBounds[2][currL - vptr->getLength() + 1] = nullptr;
   			} else if (d == Direction::north) {
   				allBounds[0][currL - vptr->getLength() + 1] = nullptr;
   			} else if (d == Direction::south) {
   			    allBounds[1][currL - vptr->getLength() + 1] = nullptr;
    		}
    	}
    }
    // return the changing bounds
	return allBounds;
}

// check whether the remaining time is enough for vehicle to pass the intersection or not
bool allowToPass(Direction d, bool redEW, int remainingTimeEW, int halfSize, bool redNS, int remainingTimeNS,
			 	 int j, vector<VehicleBase*> waitlist, vector<int> locations, Decision decision) {
	return (((((d == Direction::east || d == Direction::west) && !redEW) && 
   		(waitlist[j]->getLength() + halfSize) <=  (remainingTimeEW + locations[j])) || 
   		(((d == Direction::north || d == Direction::south) && !redNS) && 
   		(waitlist[j]->getLength() + halfSize) <=  (remainingTimeNS + locations[j])) ||
   		(locations[j] >= (waitlist[j]->getLength() + halfSize))) && 
   		waitlist[j]->getVehicleDecision() == decision);
}