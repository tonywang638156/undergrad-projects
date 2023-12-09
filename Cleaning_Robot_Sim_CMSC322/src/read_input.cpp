// #include robot and room files
#include "yaml-cpp/yaml.h"
#include <string>


// YAML parser code tutorial retreived from: https://github.com/jbeder/yaml-cpp/wiki/Tutorial and https://spl.hevs.io/spl-docs/languages/c++/libraries/yamlcpp.html

//read in and process input file
void readConfig(){
    YAML::Node config;
    
    try{
        config = YAML::LoadFile("config.yaml");
    }

    if (config["robots"]){
        // instantiate correct number robots for each type, default to 0 if left blank
        std::int val = config["robots"]["large_sweepers"] ? config["robots"]["large_sweepers"].as<int>() : 0;
        // add large sweepers to map
        val = config["robots"]["small_sweepers"] ? config["robots"]["small_sweepers"].as<int>() : 0;
        // add small sweepers to map
        val = config["robots"]["large_vacuums"] ? config["robots"]["large_vacuums"].as<int>() : 0;
        // add large vacuums to map
        val = config["robots"]["small_vacuums"] ? config["robots"]["small_vacuums"].as<int>() : 0;
        // add small vacuums to map
        val = config["robots"]["large_moppers"] ? config["robots"]["large_moppers"].as<int>() : 0;
        // add large moppers to map
        val = config["robots"]["small_moppers"] ? config["robots"]["small_moppers"].as<int>() : 0;
        // add small moppers to map
    }
    if (config["rooms"]){
        for (std::string room : config["rooms"]["large"]){
            // add each room to map of large rooms
        }

        for (std::string room : config["rooms"]["medium"]){
            // add each room to map of medium rooms
        }

        for (std::string room : config["rooms"]["small"]){
            // add each ßroom to map of small rooms
        }
    }

    return 0;
}