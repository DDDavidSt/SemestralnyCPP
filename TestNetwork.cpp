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
    ASSERT_EQ("39: o 00:18 minut (odchod->00:20), smer: Zochova\n", bratislava.getClosestFromStopString(cminter, time));
    BusStop snp = bratislava.getBusStopById(7);
    ASSERT_EQ("29: o 00:14 minut (odchod->00:16), smer: Cintorin Slavicie\n", bratislava.getClosestFromStopString(snp, time));
    Time interv(0,30);
    ASSERT_EQ("29: o 00:14 minut (odchod->00:16), smer: Cintorin Slavicie\n4: o 00:26 minut (odchod->00:28), smer: Zochova\n", bratislava.getClosestFromStopString(snp, time, interv));

}

TEST(TestNetwork, TimeTableForStop){
    PTNetwork blava;
    blava.readStopsAndLines("read_from_me.txt");
    ASSERT_EQ("============-39-SMER-Zochova-============\n   Cintorin Slavicie -6-> Televizia -7-> Zoo -6-> Lanfranconi -10-> Kralovske udolie -4-> Chatam sofer -13-> Zochova\n   Pondelok az piatok:\n   0| 0 20 40 \n   1| 0 20 40 \n   2| 0 20 40 \n   3| 0 20 40 \n   4| 0 20 40 \n   5| 0 20 40 \n   6| 0 20 40 \n   7| 0 20 40 \n   8| 0 20 40 \n   9| 0 20 40 \n   10| 0 20 40 \n   11| 0 20 40 \n   12| 0 20 40 \n   13| 0 20 40 \n   14| 0 20 40 \n   15| 0 20 40 \n   16| 0 20 40 \n   17| 0 20 40 \n   18| 0 20 40 \n   19| 0 20 40 \n   20| 0 20 40 \n   21| 0 20 40 \n   22| 0 20 40 \n   23| 0 20 40 \n   Vikendy:\n   0| 0 22 44 \n   1| 6 28 50 \n   2| 12 34 56 \n   3| 18 40 \n   4| 2 24 46 \n   5| 8 30 52 \n   6| 14 36 58 \n   7| 20 42 \n   8| 4 26 48 \n   9| 10 32 54 \n   10| 16 38 \n   11| 0 22 44 \n   12| 6 28 50 \n   13| 12 34 56 \n   14| 18 40 \n   15| 2 24 46 \n   16| 8 30 52 \n   17| 14 36 58 \n   18| 20 42 \n   19| 4 26 48 \n   20| 10 32 54 \n   21| 16 38 \n   22| 0 22 44 \n   23| 6 28 50 \n\n============-35-SMER-Chatam sofer-============\n   Holicska -16-> Mala scena -12-> Televizia -11-> Most SNP -3-> Kralovske udolie -12-> Lanfranconi -17-> Cintorin Slavicie -19-> Chatam sofer\n   Linka momentalne mimo prevadzky\n============-29-SMER-Cintorin Slavicie-============\n   Holicska <-4-> SND <-7-> Safarikovo namestie <-5-> Most SNP <-10-> Mala scena <-13-> Jesenskeho <-9-> Cintorin Slavicie\n   Pondelok az piatok:\n   0| 0 30 \n   1| 0 30 \n   2| 0 30 \n   3| 0 30 \n   4| 0 30 \n   5| 0 30 \n   6| 0 30 \n   7| 0 30 \n   8| 0 30 \n   9| 0 30 \n   10| 0 30 \n   11| 0 30 \n   12| 0 30 \n   13| 0 30 \n   14| 0 30 \n   15| 0 30 \n   16| 0 30 \n   17| 0 30 \n   18| 0 30 \n   19| 0 30 \n   20| 0 30 \n   21| 0 30 \n   22| 0 30 \n   23| 0 30 \n   Vikendy:\n   0| 0 \n   1| 0 \n   2| 0 \n   3| 0 \n   4| 0 \n   5| 0 \n   6| 0 \n   7| 0 \n   8| 0 \n   9| 0 \n   10| 0 \n   11| 0 \n   12| 0 \n   13| 0 \n   14| 0 \n   15| 0 \n   16| 0 \n   17| 0 \n   18| 0 \n   19| 0 \n   20| 0 \n   21| 0 \n   22| 0 \n   23| 0 \n\n"
    , blava.getTimeTableForStop(blava.getBusStopById(9)));
    ASSERT_EQ("============-35-SMER-Chatam sofer-============\n   Holicska -16-> Mala scena -12-> Televizia -11-> Most SNP -3-> Kralovske udolie -12-> Lanfranconi -17-> Cintorin Slavicie -19-> Chatam sofer\n   Linka momentalne mimo prevadzky\n============-29-SMER-Cintorin Slavicie-============\n   Holicska <-4-> SND <-7-> Safarikovo namestie <-5-> Most SNP <-10-> Mala scena <-13-> Jesenskeho <-9-> Cintorin Slavicie\n   Pondelok az piatok:\n   0| 26 56 \n   1| 26 56 \n   2| 26 56 \n   3| 26 56 \n   4| 26 56 \n   5| 26 56 \n   6| 26 56 \n   7| 26 56 \n   8| 26 56 \n   9| 26 56 \n   10| 26 56 \n   11| 26 56 \n   12| 26 56 \n   13| 26 56 \n   14| 26 56 \n   15| 26 56 \n   16| 26 56 \n   17| 26 56 \n   18| 26 56 \n   19| 26 56 \n   20| 26 56 \n   21| 26 56 \n   22| 26 56 \n   23| 26 56 \n   Vikendy:\n   0| 26 \n   1| 26 \n   2| 26 \n   3| 26 \n   4| 26 \n   5| 26 \n   6| 26 \n   7| 26 \n   8| 26 \n   9| 26 \n   10| 26 \n   11| 26 \n   12| 26 \n   13| 26 \n   14| 26 \n   15| 26 \n   16| 26 \n   17| 26 \n   18| 26 \n   19| 26 \n   20| 26 \n   21| 26 \n   22| 26 \n   23| 26 \n\n"
    , blava.getTimeTableForStop(blava.getBusStopById(4)));
}

TEST(TestNetwork, TimeTableStopBad){
    PTNetwork blava;
    blava.readStopsAndLines("read_from_me.txt");
    BusStop hejahej;
    try{
        blava.getTimeTableForStop(hejahej);
    }catch (Exception &e){
        ASSERT_EQ("Given bus stop  not in this network",e.message());
    }
}
TEST(TestNetwork, TimeTableWrite){
    PTNetwork blava;
    blava.readStopsAndLines("read_from_me.txt");
    blava.writeTimeTableForStop("output_for_stop.txt", blava.getBusStopById(9));
    std::ifstream read_file("output_for_stop.txt");
    std::stringstream buffer;
    buffer << read_file.rdbuf();
    ASSERT_EQ(buffer.str(),blava.getTimeTableForStop(blava.getBusStopById(9)));
}

TEST(TestNetwork, NetworkWrite){
    PTNetwork blava;
    blava.readStopsAndLines("read_from_me.txt");
    blava.writeStopsAndLines("read_from_check.txt");
    std::ifstream read_file("read_from_check.txt");
    PTNetwork bratislava;
    bratislava.readStopsAndLines("read_from_check.txt");
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
    ASSERT_EQ("29,35,39", cminter.getBSlines());
    lanf = (bratislava.getBusStopById(12));
    ASSERT_EQ("(00:40)Cintorin Slavicie -00:06-> (00:46)Televizia -00:07-> (00:53)Zoo -00:06-> (00:59)Lanfranconi", bratislava.getBusLineByNum(39).getEarliestFromStopString(cminter, lanf, cas));
    BusStop holic, kralud;
    holic = (bratislava.getBusStopById(6));
    kralud = (bratislava.getBusStopById(4));
    ASSERT_EQ("(00:30)Holicska -00:04-> (00:34)SND -00:07-> (00:41)Safarikovo namestie -00:05-> (00:46)Most SNP -00:10-> (00:56)Mala scena", bratislava.getBusLineByNum(29).getEarliestFromStopString(holic, kralud,cas));
}

TEST(TestNetwork, AddStop){
    PTNetwork ba;
    ba.readStopsAndLines("read_from_me.txt");
    ASSERT_EQ(15, ba.getNumberOfStops());
    BusStop kremnica("Kremnica", 2);
    ba.addStop(kremnica);
    ASSERT_EQ("Kremnica",ba.getBusStopById(16).getName());
    ASSERT_EQ(16,ba.getBusStopById(16).getStopNumber());
    ASSERT_EQ(16, ba.getNumberOfStops());
    BusStop prazdna;
    ba.addStop(prazdna);
    ASSERT_EQ(16, ba.getNumberOfStops());
    BusStop uniq("Siveckin", 18);
    ba.addStop(uniq);
    ASSERT_EQ(18, ba.getBusStopById(18).getStopNumber());

    try{
        ba.addStop(ba.getBusStopById(9));
    }catch (Exception &e){
        ASSERT_EQ(e.message(), "Bus stop with name Cintorin Slavicie already exists - 9 - Cintorin Slavicie");
    }
}

TEST(TestNetwork, AddLine){
    PTNetwork ba;
    ba.readStopsAndLines("read_from_me.txt");
    ASSERT_EQ(4,ba.getNumberOfLines());
    BusLine sty(40, BUS,  1);
    BusStop cminter("Cintorin Slavicie", 1);
    BusStop zoo("Zoo", 2);
    BusStop lanf("Lanfranconi", 3);
    BusStop kralud("Kralovske udolie", 4);
    BusStop chaso("Chatam sofer", 5);
    BusStop nova("Nova Zastavka", 4);
    sty.setIntervalWeekends(40);
    sty.setIntervalWorkdays(60);
    sty.addStop(-1,cminter,0,0);
    sty.addStop(-1, zoo,5,0);
    sty.addStop(-1, lanf, 7,0);
    sty.addStop(-1, kralud, 4,0);
    sty.addStop(-1, chaso, 12,0);
    sty.addStop(-1, nova, 32,0);
    ba.addLine(sty);
    ASSERT_EQ(5,ba.getNumberOfLines());
    ba.writeStopsAndLines("nova_lajn.txt");
/*TODO write some test
}