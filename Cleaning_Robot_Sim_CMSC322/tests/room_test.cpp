// roomMain.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../include/libroom/room.hpp"


#include <unordered_map>
#include <iostream>

TEST_CASE("Room Tests"){

    // Create rooms
    Room smallRoom("Bathroom", RoomSize::Small);
    Room mediumRoom("Bedroom", RoomSize::Medium);
    Room largeRoom("Library", RoomSize::Large);

    REQUIRE( smallRoom.getName() == "Bathroom" );
    REQUIRE( smallRoom.getSize() == RoomSize::Small );

    REQUIRE( mediumRoom.getName() == "Bedroom" );
    REQUIRE( mediumRoom.getSize() == RoomSize::Medium );

    REQUIRE( largeRoom.getName() == "Library" );
    REQUIRE( largeRoom.getSize() == RoomSize::Large );

    // roomDictionary[smallRoom.getName()] = smallRoom.getSize();
    // roomDictionary[mediumRoom.getName()] = mediumRoom.getSize();
    // roomDictionary[largeRoom.getName()] = largeRoom.getSize();

    // Display the contents of the dictionary
    // std::cout << "Room Dictionary:\n";
    // for (const auto& entry : roomDictionary) {
    //     std::cout << "Room Name: " << entry.first << ", Size: ";
    //     switch (entry.second) {
    //         case RoomSize::Small:
    //             std::cout << "Small\n";
    //             break;
    //         case RoomSize::Medium:
    //             std::cout << "Medium\n";
    //             break;
    //         case RoomSize::Large:
    //             std::cout << "Large\n";
    //             break;
    //     }
    // }
}
