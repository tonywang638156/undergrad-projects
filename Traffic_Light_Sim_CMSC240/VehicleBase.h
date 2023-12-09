#ifndef __VEHICLE_BASE_H__
#define __VEHICLE_BASE_H__

#include <vector>
using namespace std;
// enum: see http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-enum
enum class Direction   {north, south, east, west};
enum class VehicleType {car, suv, truck};
enum class LightColor {green, yellow, red};
enum class Decision {straight, left, right};

class VehicleBase
{
   public:
      static int vehicleCount;

   private:
      int         vehicleID;
      VehicleType vehicleType;
      Direction   vehicleDirection;
      Decision    vehicleDecision;
      int length;

   public:
   	  // default contruction
   	  VehicleBase();
      VehicleBase(VehicleType type, Direction originalDirection, Decision decision);
      VehicleBase(const VehicleBase& other);
      VehicleBase& operator=(const VehicleBase& other);
      VehicleBase(VehicleBase&& other)noexcept;
      VehicleBase& operator=(VehicleBase&& other)noexcept;
      ~VehicleBase();
      
      int getLength();

      inline int getVehicleID() const { return this->vehicleID; }
      inline VehicleType getVehicleType() const { return this->vehicleType; }
      inline Direction   getVehicleOriginalDirection() const { return this->vehicleDirection; }
      inline Decision   getVehicleDecision() const { return this->vehicleDecision; }
      
};

#endif
