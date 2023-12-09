# User Guide

## Input File
In order for the system to manage your robot fleet, we must first know what robots you have, as well as the specifications of your building. Please make sure the config.YAML file in the *input* directory is updated accurately with all the following information:
```
robots:
    large_sweepers: int
    small_sweepers: int
    large_vacuums: int
    small_vacuums: int
    large_moppers: int
    small_moppers: int
rooms:
    large: [<room_name>, ...]
    medium: [<room_name>, ...]
    small: [<room_name>, ...]
```
Specify the number of robots of each type you have. Specify the names of the rooms in your buildings in their respective lists based on their size.