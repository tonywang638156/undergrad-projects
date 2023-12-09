// room.hpp

#define LIBHELLO_ROOM_HPP
#define LIBHELLO_ROOM_HPP

// rooms.hpp
#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

enum class RoomSize {
    Small,
    Medium,
    Large
};

class Room {
public:
    Room(const std::string& name, RoomSize size)
        : name_(name), size_(size) {}

    // Getters
    std::string getName() const {
        return name_;
    }

    RoomSize getSize() const {
        return size_;
    }

private:
    std::string name_;
    RoomSize size_;
};
