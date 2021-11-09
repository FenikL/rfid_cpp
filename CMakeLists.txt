cmake_minimum_required(VERSION 3.20)
project(rfid)

set(CMAKE_CXX_STANDARD 20)

add_executable(rfid main.cpp variables.cpp variables.h inderectVariables.cpp inderectVariables.h snr_to_ber.cpp snr_to_ber.h)
