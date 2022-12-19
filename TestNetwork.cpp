//
// Created by david on 19/12/22.
//
// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "Network.h"


using namespace std;
using namespace ::testing;

TEST(TestNetwork, ReadFromFIle){
    PTNetwork bratislava;
    bratislava.readStopsAndLines("read_from_me.txt");
    ASSERT_EQ(15, bratislava.getNumberOfStops());
    ASSERT_EQ(4, bratislava.getNumberOfLines());
    ASSERT_TRUE(bratislava.getBusLineByNum(39).isLineInOrder());
    ASSERT_FALSE(bratislava.getBusLineByNum(35).isLineInOrder());
    Time cas(0,30);
    BusStop cminter, lanf;
    cminter = std::move(bratislava.getBusStopById(9));
    lanf = std::move(bratislava.getBusStopById(12));
    ASSERT_EQ("(00:40)Cintorin Slavicie -00:06-> (00:46)Televizia -00:07-> (00:53)Zoo -00:06-> (00:59)Lanfranconi", bratislava.getBusLineByNum(39).getEarliestFromStopString(cminter, lanf,cas));
    BusStop holic, kralud;
    holic = std::move(bratislava.getBusStopById(6));
    kralud = std::move(bratislava.getBusStopById(4));
    ASSERT_EQ("(00:30)Holicska -00:04-> (00:34)SND -00:07-> (00:41)Safarikovo namestie -00:05-> (00:46)Most SNP -00:10-> (00:56)Mala scena", bratislava.getBusLineByNum(29).getEarliestFromStopString(holic, kralud,cas));
}

TEST(TestNetwork, Nasty){
    PTNetwork bratislava;
    bratislava.readStopsAndLines("read_from_me.txt");
    ASSERT_EQ(15, bratislava.getNumberOfStops());
    ASSERT_EQ(4, bratislava.getNumberOfLines());
    ASSERT_TRUE(bratislava.getBusLineByNum(39).isLineInOrder());
    ASSERT_FALSE(bratislava.getBusLineByNum(35).isLineInOrder());
    Time cas(0,20);
    BusStop cminter, lanf;
    cminter = std::move(bratislava.getBusStopById(9));
    lanf = std::move(bratislava.getBusStopById(12));
    try{
        std::string res;
        res = bratislava.getBusLineByNum(4).getEarliestFromStopString(cminter, lanf,cas);
    }catch (Exception &e){
        ASSERT_EQ("Given start point is not in Line number 4", e.message());
    }
    BusStop sest, desat;
    sest = std::move(bratislava.getBusStopById(6));
    desat = std::move(bratislava.getBusStopById(10));
    try{
        bratislava.getBusLineByNum(35).getEarliestFromStopString(sest, desat,cas);
    }catch (Exception &e){
        ASSERT_EQ("Line 35 currently not in order", e.message());
    }
}

TEST(TestNetwork, NastyFile){
    PTNetwork bratislava;
    try {
        bratislava.readStopsAndLines("read_from_me_bad.txt");
    }catch (Exception &e){
        ASSERT_EQ("Bus Stop number 23 not among list of bus stops", e.message());
    }

}