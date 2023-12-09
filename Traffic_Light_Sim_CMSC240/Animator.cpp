#ifndef __ANIMATOR_CPP__
#define __ANIMATOR_CPP__

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "Animator.h"

// used for drawing width -- can be defined by user
int Animator::MAX_VEHICLE_COUNT = 9999;

// computed in constructor since user can redefine count above
int Animator::DIGITS_TO_DRAW = 0;
// repeated strings of '-' or ' ' based on the digit-width MAX_VEHCILE_COUNT;
// both computed i n constructor
std::string Animator::SECTION_BOUNDARY_EW = "";
std::string Animator::EMPTY_SECTION = "";

// see https://unix.stackexchange.com/questions/124407/what-color-codes-can-i-use-in-my-ps1-prompt
const std::string Animator::COLOR_RED_FG    = "\033[1;31m";
const std::string Animator::COLOR_GREEN_FG  = "\033[1;32m";
const std::string Animator::COLOR_BLUE_FG   = "\033[1;34m";
const std::string Animator::COLOR_RED_BG    = "\033[41m\033[1;37m";
const std::string Animator::COLOR_GREEN_BG  = "\033[42m\033[1;37m";
const std::string Animator::COLOR_BLUE_BG   = "\033[44m\033[1;37m";
const std::string Animator::COLOR_YELLOW_BG = "\033[43m\033[1;37m";
const std::string Animator::COLOR_RESET     = "\033[0m";

std::string Animator::GREEN_LIGHT = "";
std::string Animator::YELLOW_LIGHT = "";
std::string Animator::RED_LIGHT = "";

const std::string Animator::SECTION_BOUNDARY_NS = "|";
const std::string Animator::ERROR_MSG =
    "Error in Animator::draw: must call all four Animator::setVehicles* methods prior to calling Animator::draw";

//======================================================================
//* Animator::Animator(int numSectionsBeforeIntersection)
//======================================================================
Animator::Animator(int numSectionsBeforeIntersection)
{
    // redo here in case the user set MAX_VEHCILE_COUNT differently
    Animator::DIGITS_TO_DRAW = Animator::MAX_VEHICLE_COUNT <= 1 ? 
        2 : static_cast<int>(log10(Animator::MAX_VEHICLE_COUNT)) + 1;
    Animator::SECTION_BOUNDARY_EW = std::string(Animator::DIGITS_TO_DRAW, '-');
    Animator::EMPTY_SECTION = std::string(Animator::DIGITS_TO_DRAW, ' ');

    // these will use the redone DIGITS TO DRAW above
    Animator::GREEN_LIGHT  = createLight(LightColor::green);
    Animator::YELLOW_LIGHT = createLight(LightColor::yellow);
    Animator::RED_LIGHT    = createLight(LightColor::red);

    numSectionsBefore = numSectionsBeforeIntersection;

    // each lane will be twice the number of sections provided (before and 
    // after the intersection) plus the two intersection sections
    eastToWest.resize(numSectionsBefore * 2 + 2);
    westToEast.resize(numSectionsBefore * 2 + 2);
    northToSouth.resize(numSectionsBefore * 2 + 2);
    southToNorth.resize(numSectionsBefore * 2 + 2);

    // this will set the values in the vector to 0 (false), indicating that
    // the user must set the vehicles in each of the four directions using the
    // setVehicles* functions
    vehiclesAreSet.resize(4);
}

//======================================================================
//* Animator::~Animator()
//======================================================================
Animator::~Animator() {}

//======================================================================
//* std::string Animator::createLight(LightColor color)
//======================================================================
std::string Animator::createLight(LightColor color)
{
    std::string light = "";

    if (color == LightColor::green)  light += Animator::COLOR_GREEN_BG;
    if (color == LightColor::yellow) light += Animator::COLOR_YELLOW_BG;
    if (color == LightColor::red)    light += Animator::COLOR_RED_BG;

    light += std::string(2, ' ') + Animator::COLOR_RESET;

    return light;
}

//======================================================================
//* Animator::draw(int time)
//======================================================================
void Animator::draw(int time)
{
    // ensure all four setVehicles* methods have been called prior
    std::vector<bool>::iterator it = vehiclesAreSet.begin();
    for (; it != vehiclesAreSet.end(); it++)
        if (*it == false) throw std::runtime_error(Animator::ERROR_MSG.c_str());

    std::cout << "\x1B[2J\x1B[H";  // clears the screen

    drawNorthPortion(time);
    drawWestbound();
    drawEastbound();
    drawSouthPortion();

    // reset the values (to false) in the boolean vector, indicating that the 
    // user must set the vehicles in each of the four directions using the
    // setVehicles* functions
    vehiclesAreSet.clear();
    vehiclesAreSet.resize(4);
}

//======================================================================
//* std::string getVehicleColor(VehicleBase* vptr)
//======================================================================
std::string Animator::getVehicleColor(VehicleBase* vptr)
{
    Direction dir = vptr->getVehicleOriginalDirection();
    switch (vptr->getVehicleType())
    {
        case VehicleType::car: 
            if (dir == Direction::east || dir == Direction::west)
                return Animator::COLOR_RED_BG;
            return Animator::COLOR_RED_FG;
        case VehicleType::suv:
            if (dir == Direction::east || dir == Direction::west)
                return Animator::COLOR_BLUE_BG;
            return Animator::COLOR_BLUE_FG;
        case VehicleType::truck:
            if (dir == Direction::east || dir == Direction::west)
                return Animator::COLOR_GREEN_BG;
            return Animator::COLOR_GREEN_FG;
    }
    return Animator::COLOR_RESET;
}

//======================================================================
//* ::string Animator::getTrafficLight(Direction direction)
//======================================================================
std::string Animator::getTrafficLight(Direction direction)
{
    LightColor color = eastWestLightColor;
    if (direction == Direction::north || direction == Direction::south)
        color = northSouthLightColor;

    std::string light = "";
    
    // when odd DIGITS_TO_DRAW, want the extra padding on left of lights
    // for the southbound and eastbound traffice
    if (direction == Direction::south || direction == Direction::east)
        light += Animator::DIGITS_TO_DRAW % 2 == 0 ?
            std::string((Animator::DIGITS_TO_DRAW - 2) / 2, ' ') :
            std::string((Animator::DIGITS_TO_DRAW - 1) / 2, ' ');
    else
        light += std::string((Animator::DIGITS_TO_DRAW - 2) / 2, ' ');

    switch (color)
    {
        case LightColor::green:  light += Animator::GREEN_LIGHT;  break;
        case LightColor::yellow: light += Animator::YELLOW_LIGHT; break;
        case LightColor::red:    light += Animator::RED_LIGHT;    break;
    }
    
    // when odd DIGITS_TO_DRAW, want the extra padding on right of lights
    // for the northbound and westbound traffice
    if (direction == Direction::south || direction == Direction::east)
        light += std::string((Animator::DIGITS_TO_DRAW - 2) / 2, ' ');
    else
        light += Animator::DIGITS_TO_DRAW % 2 == 0 ?
            std::string((Animator::DIGITS_TO_DRAW - 2) / 2, ' ') :
            std::string((Animator::DIGITS_TO_DRAW - 1) / 2, ' ');

    return light;
}

//======================================================================
//* Animator::drawNorthPortion(int time)
//======================================================================
void Animator::drawNorthPortion(int time)
{
    for (int s = 0; s < numSectionsBefore; s++)
    {
        // draw empty spaces to account for E/W lanes to left of intersection
        for (int i = 0; i < numSectionsBefore; i++) 
            if (s == numSectionsBefore - 1 && i == s)
                std::cout << (i > 0 ? " " : "") 
                          << getTrafficLight(Direction::south); // or north
            else
                std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) southbound vehicle if present, 
        // or an empty section
        if (northToSouth[s] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(northToSouth[s])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << northToSouth[s]->getVehicleID()
                      << Animator::COLOR_RESET;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) northbound vehicle if present, 
        // or an empty section
        int section = southToNorth.size() - s - 1;
        if (southToNorth[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(southToNorth[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << southToNorth[section]->getVehicleID()
                      << Animator::COLOR_RESET;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        if (s == numSectionsBefore - 1)
            std::cout << getTrafficLight(Direction::west);  // or east

        std::cout << std::endl;

        if (s < numSectionsBefore - 1)  // last will be drawn by westbound method
        {
            // draw empty spaces to account for E/W lanes to left of intersection
            for (int i = 0; i < numSectionsBefore; i++) 
                std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;
            std::cout << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW
                      << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW 
                      << Animator::SECTION_BOUNDARY_NS;
        }

        // draw the time halfway down, on right
        if (s == numSectionsBefore / 2) 
            std::cout << std::setfill(' ') 
                << std::setw((numSectionsBefore / 2) * Animator::DIGITS_TO_DRAW)
                << "time: " << time;

        if (s < numSectionsBefore - 1) std::cout << std::endl;
    }
}

//======================================================================
//* Animator::drawEastWestBoundary()
//======================================================================
void Animator::drawEastWestBoundary()
{
    for (int s = 0; s < numSectionsBefore; s++)
        std::cout << Animator::SECTION_BOUNDARY_EW 
            << (s == numSectionsBefore-1 ? Animator::SECTION_BOUNDARY_NS : " ");
    std::cout << Animator::SECTION_BOUNDARY_EW << Animator::SECTION_BOUNDARY_NS;
    std::cout << Animator::SECTION_BOUNDARY_EW << Animator::SECTION_BOUNDARY_NS;
    for (int s = 0; s < numSectionsBefore; s++)
        std::cout << Animator::SECTION_BOUNDARY_EW << " ";
    std::cout << std::endl;
}

//======================================================================
//* Animator::drawEastbound()
//======================================================================
void Animator::drawEastbound()
{
    drawEastWestBoundary();

    // handle all the west-to-east sections before the intersection
    for (int s = 0; s < numSectionsBefore; s++)
    {
        int section = s;
        if (westToEast[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION << "|";
        else
            std::cout << getVehicleColor(westToEast[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << westToEast[section]->getVehicleID()
                      << Animator::COLOR_RESET << "|";
    }

    // now handle the intersection; the first spot in the west to east lane
    // could be occupied by a vehicle in the W2E lane or in the N2S lane
    VehicleBase* vptr = (westToEast[numSectionsBefore] != nullptr ?
            westToEast[numSectionsBefore] : northToSouth[numSectionsBefore + 1]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_RESET << "|";

    // and the second spot in the west to east lane could be occupied by a
    // vehicle in the W2E lane or in the S2N lane
    vptr = (westToEast[numSectionsBefore + 1] != nullptr ?
            westToEast[numSectionsBefore + 1] : southToNorth[numSectionsBefore]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_RESET << "|";

    // and now handle all the west-to-east sections after the intersection
    for (int s = numSectionsBefore + 2; s < static_cast<int>(westToEast.size()); s++)
    {
        int section = s;
        if (westToEast[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(westToEast[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << westToEast[section]->getVehicleID()
                      << Animator::COLOR_RESET;
        std::cout << (s < static_cast<int>(westToEast.size()) - 1 ? "|" :  "");
    }
    std::cout << std::endl;

    drawEastWestBoundary();
}

//======================================================================
//* Animator::drawWestbound()
//======================================================================
void Animator::drawWestbound()
{
    drawEastWestBoundary();

    // handle all the east-to-west sections before the intersection
    // (drawing in reverse order of the vector)
    for (int s = 0; s < numSectionsBefore; s++)
    {
        int section = eastToWest.size() - s - 1;
        if (eastToWest[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION << "|";
        else
            std::cout << getVehicleColor(eastToWest[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << eastToWest[section]->getVehicleID()
                      << Animator::COLOR_RESET << "|";
    }

    // now handle the intersection; the first spot encountered L to R in the
    // east to west lane could be occupied by a vehicle in the E2W lane or in
    // the N2S lane
    VehicleBase* vptr = (eastToWest[numSectionsBefore + 1] != nullptr ?
            eastToWest[numSectionsBefore + 1] : northToSouth[numSectionsBefore]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_RESET << "|";

    // and the second spot encountered L to R in the east to west lane could be
    // occupied by a vehicle in the E2W lane or in the S2N lane
    vptr = (eastToWest[numSectionsBefore] != nullptr ?
            eastToWest[numSectionsBefore] : southToNorth[numSectionsBefore + 1]);
    if (vptr == nullptr)
        std::cout << Animator::EMPTY_SECTION << "|";
    else
        std::cout << getVehicleColor(vptr)
                  << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                  << vptr->getVehicleID()
                  << Animator::COLOR_RESET << "|";

    // and now handle all the east-to-west sections after the intersection
    // (drawing in reverse order of the vector)
    for (int s = numSectionsBefore + 2; s < static_cast<int>(eastToWest.size()); s++)
    {
        int section = eastToWest.size() - s - 1;
        if (eastToWest[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(eastToWest[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << eastToWest[section]->getVehicleID()
                      << Animator::COLOR_RESET;
        std::cout << (s < static_cast<int>(eastToWest.size()) - 1 ? "|" :  "");
    }
    std::cout << std::endl;

}

//======================================================================
//* Animator::drawSouthPortion()
//======================================================================
void Animator::drawSouthPortion()
{
    for (int s = 0; s < numSectionsBefore; s++)
    {
        // draw empty spaces to account for E/W lanes to left of intersection
        for (int i = 0; i < numSectionsBefore; i++) 
            if (s == 0 && i == numSectionsBefore - 1)
                std::cout << (i > 0 ? " " : "") 
                          << getTrafficLight(Direction::east); // or west
            else
                std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;


        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) southbound vehicle if present, 
        // or an empty section
        int section = numSectionsBefore + s + 2;
        if (northToSouth[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(northToSouth[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << northToSouth[section]->getVehicleID()
                      << Animator::COLOR_RESET;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        // either draw (a portion of) northbound vehicle if present, 
        // or an empty section
        section = numSectionsBefore - s - 1;
        if (southToNorth[section] == nullptr)
            std::cout << Animator::EMPTY_SECTION;
        else
            std::cout << getVehicleColor(southToNorth[section])
                      << std::setfill('0') << std::setw(Animator::DIGITS_TO_DRAW)
                      << southToNorth[section]->getVehicleID()
                      << Animator::COLOR_RESET;

        std::cout << Animator::SECTION_BOUNDARY_NS;

        if (s == 0)
            std::cout << getTrafficLight(Direction::north);  // or south
            
        std::cout << std::endl;

        if (s < numSectionsBefore - 1)  // no need to draw last section spacer
        {
            // draw empty spaces to account for E/W lanes to left of intersection
            for (int i = 0; i < numSectionsBefore; i++) 
                std::cout << (i > 0 ? " " : "") << Animator::EMPTY_SECTION;
            std::cout << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW
                      << Animator::SECTION_BOUNDARY_NS << Animator::SECTION_BOUNDARY_EW 
                      << Animator::SECTION_BOUNDARY_NS << std::endl;
        }

    }
}

#endif
