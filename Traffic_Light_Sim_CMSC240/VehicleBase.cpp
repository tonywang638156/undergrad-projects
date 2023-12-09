#ifndef __VEHICLE_BASE_CPP__
#define __VEHICLE_BASE_CPP__

#include "VehicleBase.h"

int VehicleBase::vehicleCount = 0;

// default constructor
VehicleBase::VehicleBase()
	: vehicleID(0), 
      vehicleType{VehicleType::car},
      vehicleDirection{Direction::north},
      vehicleDecision{Decision::straight},
      length{0} {}

// common constructor
VehicleBase::VehicleBase(VehicleType type, Direction direction, Decision decision)
    : vehicleID{VehicleBase::vehicleCount++}, 
      vehicleType{type},
      vehicleDirection{direction},
      vehicleDecision{decision}
{
	// set the length of the vehicle
	if (type == VehicleType::truck) {
		this->length =  4;
	} else if (type == VehicleType::suv) {
		this->length = 3;
	} else {
		this->length = 2;
	}
}

// copy constructor
VehicleBase::VehicleBase(const VehicleBase& other)
    : vehicleID{other.vehicleID},
      vehicleType{other.vehicleType},
      vehicleDirection{other.vehicleDirection},
      vehicleDecision{other.vehicleDecision},
      length{other.length} {}

// copy assignment
VehicleBase& VehicleBase::operator=(const VehicleBase& other) {
	if (this == &other) { // if it started the same, no need to copy
		return *this;
	}
	this->vehicleID = other.vehicleID;
    this->vehicleType = other.vehicleType;
    this->vehicleDirection = other.vehicleDirection;
    this->vehicleDecision = other.vehicleDecision;
    this->length = other.length;
	return *this;
}

// move constructor
VehicleBase::VehicleBase(VehicleBase&& other)noexcept
	: vehicleID{other.vehicleID},
      vehicleType{other.vehicleType},
      vehicleDirection{other.vehicleDirection},
      vehicleDecision{other.vehicleDecision},
      length{other.length} {
	other.vehicleID = 0;
	other.vehicleType = VehicleType::car;
	other.vehicleDirection = Direction::north;
	other.vehicleDecision = Decision::straight;
	other.length = 0;
}

// move assignment
VehicleBase& VehicleBase::operator=(VehicleBase&& other)noexcept {
	if (this == &other) { // if it started the same, no need to move
		return *this;
	}
	this->vehicleID = other.vehicleID;
    this->vehicleType = other.vehicleType;
    this->vehicleDirection = other.vehicleDirection;
    this->vehicleDecision = other.vehicleDecision;
    this->length = other.length;
	other.vehicleID = 0;
	other.vehicleType = VehicleType::car;
	other.vehicleDirection = Direction::north;
	other.vehicleDecision = Decision::straight;
	other.length = 0;
	return *this;
}

// destructor
VehicleBase::~VehicleBase() {}

int VehicleBase::getLength() {
	return this->length;
}

#endif
