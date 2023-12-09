#ifndef __ANIMATOR_H__
#define __ANIMATOR_H__

#include <string>
#include <vector>
#include "VehicleBase.h"

//==========================================================================
//* class Animator
//* Author: Barry Lawson
//* Written: 22 Apr 2018
//* Modified: 18 Nov 2018
//* This class is intended to provide a text-based graphical interface
//* for animating the traffic intersection simulation from CMSC 240.
//* The idea for this class was heavily influenced by code written for
//* a similar project in F2017 by the following students:
//*       Tracy Nguyen, Hannah Small, Toan Nguyen
//*
//* This code requires that the VehicleBase class be a super class of
//* any vehicle class(es) in your implementation.
//*
//* Usage:
//*   - construct an instance of Animator, passing the number of sections
//*     prior to the intersection (e.g., 8 will result in a lane of 
//*     (8*2) + 2 = 18 sections)
//*   - construct four std::vector<VehicleBase*>, one for each direction
//*     of westbound, easbound, southbound, and northbound
//*   - assign a _pointer_ to each VehicleBase vehicle in your simulation
//*     into the entry in the vector corresponding to the section(s)
//*     occupied by that vehicle
//*   - call each of setVehiclesNorthbound, setVehiclesSouthbound,
//*     setVehiclesEastbound, and setVehiclesWestbound, passing the
//*     corresponding std::vector<VehicleBase*>
//*   - call setLightEastWest and setLightNorthSouth passing the appropriate
//*     color (either LightColor::green, LightColor::yellow, LightColor::red)
//*   - call draw(), passing in the value of the simulation time clock
//*   - then repeat:
//*        - update each std::vector<VehicleBase*> appropriately
//*        - call each of setVehiclesNorthbound, setVehiclesSouthbound,
//*          setVehiclesEastbound, and setVehiclesWestbound, passing the
//*          corresponding std::vector<VehicleBase*>
//*        - if appropriate, call setLightEastWest and setLightNorthSouth passing 
//*          the updated color
//*        - call draw(), passing in the value of the simulation time clock
//*
//* Modifications done 18 Nov 2018:
//*   - added enum classes Direction, VehicleType, and LightColor in
//*     VehicleBase.cpp
//*   - changed coloring so that originally east/west-bound vehicles are colored
//*     via background and originally nort/south-bound vehicles are colored via
//*     foreground
//*   - added capability for traffic lights display (north/south lights are 
//*     identical, as are east/west lights)
//==========================================================================

class Animator
{
   private:
      static int         DIGITS_TO_DRAW;
      static std::string SECTION_BOUNDARY_EW;
      static std::string EMPTY_SECTION;
      
      static const std::string SECTION_BOUNDARY_NS;
      static const std::string ERROR_MSG;

      static const std::string COLOR_RED_FG;
      static const std::string COLOR_GREEN_FG;
      static const std::string COLOR_BLUE_FG;
      static const std::string COLOR_RED_BG;
      static const std::string COLOR_GREEN_BG;
      static const std::string COLOR_BLUE_BG;
      static const std::string COLOR_YELLOW_BG;
      static const std::string COLOR_RESET;

      static std::string GREEN_LIGHT;
      static std::string YELLOW_LIGHT;
      static std::string RED_LIGHT;

      std::vector<bool> vehiclesAreSet;  // 0:north 1:west 2:south 3:east
      int numSectionsBefore;

      std::string getVehicleColor(VehicleBase* vptr);
      std::string createLight(LightColor color);
      std::string getTrafficLight(Direction direction);

      void drawNorthPortion(int time);
      void drawEastbound();
      void drawEastWestBoundary();
      void drawWestbound();
      void drawSouthPortion();

      LightColor northSouthLightColor;
      LightColor eastWestLightColor;

      std::vector<VehicleBase*> eastToWest;
      std::vector<VehicleBase*> westToEast;
      std::vector<VehicleBase*> northToSouth;
      std::vector<VehicleBase*> southToNorth;

   public:
      static int MAX_VEHICLE_COUNT;

      Animator(int numSectionsBeforeIntersection);
      ~Animator();

      inline void setLightNorthSouth(LightColor color)
            { northSouthLightColor = color; }
      inline void setLightEastWest(LightColor color)
            { eastWestLightColor = color; }

      inline void setVehiclesNorthbound(std::vector<VehicleBase*> vehicles)
            { southToNorth = vehicles;  vehiclesAreSet[0] = true; }
      inline void setVehiclesWestbound(std::vector<VehicleBase*> vehicles)
            { eastToWest   = vehicles;  vehiclesAreSet[1] = true; }
      inline void setVehiclesSouthbound(std::vector<VehicleBase*> vehicles)
            { northToSouth = vehicles;  vehiclesAreSet[2] = true; }
      inline void setVehiclesEastbound(std::vector<VehicleBase*> vehicles)
            { westToEast   = vehicles;  vehiclesAreSet[3] = true; }


      void draw(int time);
};

#endif
