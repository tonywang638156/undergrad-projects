find_package(Catch2 3 REQUIRED)

IF (${Catch2_FOUND})
    MESSAGE(STATUS "Found Catch2")
ELSE (${Catch2_FOUND})
    MESSAGE(STATUS "Cound not find Catch2. Check to make sure its installed on your system!")
ENDIF (${Catch2_FOUND})

add_executable(room_test room_test.cpp)
add_executable(robot_test robot_test.cpp)
add_executable(rms_test rms_test.cpp)

target_compile_features(room_test PRIVATE cxx_std_20)
target_compile_features(robot_test PRIVATE cxx_std_20)
target_compile_features(rms_test PRIVATE cxx_std_20)

target_link_libraries(room_test
  PRIVATE
    Catch2::Catch2WithMain
)
target_link_libraries(robot_test
  PRIVATE
    Catch2::Catch2WithMain
    robot
)
target_link_libraries(rms_test
  PRIVATE
    Catch2::Catch2WithMain
)