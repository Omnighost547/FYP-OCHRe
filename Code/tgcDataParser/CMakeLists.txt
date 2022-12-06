cmake_minimum_required(VERSION 3.23)
project(tgcDataParser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-luring")

add_executable(tgcDataParser main.cpp ThinkGearConnector.h ThinkGearConnector.cpp ThinkGearPacket.cpp ThinkGearPacket.h)
# needs boost
# on linux also needs liburing