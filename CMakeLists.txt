cmake_minimum_required(VERSION 3.6.2)
project(Projekt)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(CMAKE_CXX_STANDARD 14)

find_package(Threads)

set(SOURCE_FILES
        side_classes.cpp
        BusStop.cpp
        BusLine.cpp
        PTNetwork.cpp
        BFSNetwork.cpp

        TestBusLine.cpp
        TestSideClassesAndBusStop.cpp
        TestNetwork.cpp
        TestBFSNetwork.cpp

        GoogleTest/gtest-death-test.cc
        GoogleTest/gtest-filepath.cc
        GoogleTest/gtest-port.cc
        GoogleTest/gtest-printers.cc
        GoogleTest/gtest-test-part.cc
        GoogleTest/gtest-typed-test.cc
        GoogleTest/gtest.cc
        GoogleTest/gtest_main.cc)

include_directories(GoogleTest)
add_executable(Projekt ${SOURCE_FILES} ${SOURCES})
target_link_libraries(Projekt ${CMAKE_THREAD_LIBS_INIT})