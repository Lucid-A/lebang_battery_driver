cmake_minimum_required(VERSION 2.8.3)
project(battery)

set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(catkin REQUIRED 
   COMPONENTS 
    roscpp
    diagnostic_updater
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS diagnostic_updater roscpp sensor_msgs
  DEPENDS Boost
)

include_directories(
    include 
    ${catkin_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS} 
)
add_executable(battery_node src/bms_node.cpp src/bms.cpp)
target_link_libraries(battery_node ${catkin_LIBRARIES} ${Boost_LIBRARIES})

install(
    TARGETS
        battery_node
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(DIRECTORY config launch
    DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

