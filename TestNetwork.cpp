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
    cminter = (bratislava.getBusStopById(9));
    ASSERT_EQ("Cintorin Slavicie", cminter.getName());
    ASSERT_EQ(BUS, bratislava.getBusLineByNum(39).getLineType());
    ASSERT_EQ(TRAM, bratislava.getBusLineByNum(4).getLineType());
    ASSERT_EQ(METRO, bratislava.getBusLineByNum(29).getLineType());
    ASSERT_EQ("39,35,29", cminter.getBSlines());
    lanf = (bratislava.getBusStopById(12));
    ASSERT_EQ("(00:40)Cintorin Slavicie -00:06-> (00:46)Televizia -00:07-> (00:53)Zoo -00:06-> (00:59)Lanfranconi", bratislava.getBusLineByNum(39).getEarliestFromStopString(cminter, lanf, cas));
    BusStop holic, kralud;
    holic = (bratislava.getBusStopById(6));
    kralud = (bratislava.getBusStopById(4));
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
    cminter = (bratislava.getBusStopById(9));
    lanf = (bratislava.getBusStopById(12));

    BusStop nastyy;
    try {
        nastyy = bratislava.getBusStopById(900);
    }catch (Exception &e){
        ASSERT_EQ(e.message(), "No such stop ID 900 in network");
    }
    try{
        std::string res;
        res = bratislava.getBusLineByNum(4).getEarliestFromStopString(cminter, lanf,cas);
    }catch (Exception &e){
        ASSERT_EQ("Given start point is not in Line number 4", e.message());
    }
    BusStop sest, desat;
    sest = (bratislava.getBusStopById(6));
    desat = (bratislava.getBusStopById(10));
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

TEST(TestNetwork, NastyMultiBusStopsAndLines){
    PTNetwork bratislava_zla;
    try{
        bratislava_zla.readStopsAndLines("rfm_bad_multi_bus_num.txt");
    }catch (Exception &e){
        ASSERT_EQ("Multiple Bus Stop number definition in file rfm_bad_multi_bus_num.txt on stop number 14", e.message());
    }
    try{
        bratislava_zla.readStopsAndLines("rfm_multi_lines_num.txt");
    }catch (Exception &e){
        ASSERT_EQ("Multiple Bus Line number definition in file rfm_multi_lines_num.txt on Bus Line number 39", e.message());
    }
}

TEST(TestNetwork, NoLines){
    PTNetwork ba_without_lines;
    ba_without_lines.readStopsAndLines("rfm_no_lines.txt");
    ASSERT_EQ("Dobrovicova", ba_without_lines.getBusStopById(1).getName());
    ASSERT_EQ("Zochova", ba_without_lines.getBusStopById(15).getName());
}

TEST(TestNetwork, NoStops){
    PTNetwork ba_without_lines;

    try{
        ba_without_lines.readStopsAndLines("rfm_no_stops.txt");
    }catch (Exception &e){
        ASSERT_EQ("Wrong bus stop name - must be of non-empty length and start with a letter",e.message());
    }

}

TEST(TestNetwork, EarliestConn){
    PTNetwork bratislava;
    bratislava.readStopsAndLines("read_from_me.txt");
    BusStop cminter = bratislava.getBusStopById(9);
    Time time(0,2);
    ASSERT_EQ("39: o 00:18 minut (odchod->00:20), smer: Zochova\n29: o 00:28 minut (odchod->00:30), smer: Cintorin Slavicie\n", bratislava.getClosestFromStopString(cminter, time));
    BusStop snp = bratislava.getBusStopById(7);
    ASSERT_EQ("4: o 00:26 minut (odchod->00:28), smer: Zochova\n29: o 00:14 minut (odchod->00:16), smer: Cintorin Slavicie\n", bratislava.getClosestFromStopString(snp, time));

}