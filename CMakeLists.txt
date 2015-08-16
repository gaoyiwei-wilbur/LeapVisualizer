
cmake_minimum_required(VERSION 2.8)

PROJECT(LeapVisualizer)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(LeapVisualizer MACOSX_BUNDLE LeapVisualizer)

target_link_libraries(LeapVisualizer ${VTK_LIBRARIES})
