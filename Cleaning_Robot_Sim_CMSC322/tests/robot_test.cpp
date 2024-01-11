// roomMain.cpp
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "../include/librobot/robot.h"


#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>

TEST_CASE("Robot Tests"){

    // Create rooms and add them to the dictionary
    Robot robot1(Size::Large, Type::Sweeper, 0, "Bathroom");
    Robot robot2(Size::Small, Type::Vacuum, 2, "Hub");
    Mop mopRobot(Size::Large, Type::Mop, 15, "Kitchen");
    Vacuum vacuumRobot(Size::Small, Type::Vacuum, 12, "Bedroom");
    Sweeper sweeperRobot(Size::Large, Type::Sweeper, 18, "Hallway");

    REQUIRE( robot1.getSize() == Size::Large );
    REQUIRE( robot1.getType() == Type::Sweeper );
    REQUIRE( robot1.getTime() == 0 );
    REQUIRE( robot1.getLoc() == "Bathroom" );
    // REQUIRE( sweeperRobot.clean() == 18);

    REQUIRE( robot2.getSize() == Size::Small );
    REQUIRE( robot2.getType() == Type::Vacuum );
    REQUIRE( robot2.getTime() == 2 );
    REQUIRE( robot2.getLoc() == "Hub" );


    REQUIRE( mopRobot.getSize() == Size::Large );
    REQUIRE( mopRobot.getType() == Type::Mop );
    REQUIRE( mopRobot.getTime() == 15 );
    REQUIRE( mopRobot.getLoc() == "Kitchen" );

    REQUIRE( vacuumRobot.getSize() == Size::Small );
    REQUIRE( vacuumRobot.getType() == Type::Vacuum );
    REQUIRE( vacuumRobot.getTime() == 12 );
    REQUIRE( vacuumRobot.getLoc() == "Bedroom" );

    REQUIRE( sweeperRobot.getSize() == Size::Large );
    REQUIRE( sweeperRobot.getType() == Type::Sweeper );
    REQUIRE( sweeperRobot.getTime() == 18 );
    REQUIRE( sweeperRobot.getLoc() == "Hallway" );

    robot2.setTime(20);
    robot2.setLoc("Bathroom");

    REQUIRE( robot2.getSize() == Size::Small );
    REQUIRE( robot2.getType() == Type::Vacuum );
    REQUIRE( robot2.getTime() == 20 );
    REQUIRE( robot2.getLoc() == "Bathroom" );

    sweeperRobot.setSize("Small");
    sweeperRobot.setType("Vacuum");
    sweeperRobot.setTime(18);
    sweeperRobot.setLoc("Hub");

    REQUIRE( sweeperRobot.getSize() == Size::Large );
    REQUIRE( sweeperRobot.getType() == Type::Sweeper );
    REQUIRE( sweeperRobot.getTime() == 18 );
    REQUIRE( sweeperRobot.getLoc() == "Hub" );


    }