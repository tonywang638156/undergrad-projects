cmake_minimum_required(VERSION 3.16)
project(libsim)

include(dependencies.cmake)

add_library(robot
src/robot.cpp
)

# add_library(librms
#   src/RMS.cpp
# )

# target_include_directories(librms
#   PUBLIC
#     $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
# )

# target_link_libraries(librms
#   fmt::fmt
# )

add_subdirectory(app)
# add_subdirectory(tests)

