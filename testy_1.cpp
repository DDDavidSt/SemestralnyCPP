// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "task_1.h"


using namespace std;
using namespace ::testing;

TEST(TestFirstTask, KorektnaZastavka) {
    BusStop cminter("Cintorin Slavicie", 2);
    ASSERT_EQ("Cintorin Slavicie", cminter.getName());
    ASSERT_EQ(2,cminter.getStopNumber());
    cminter.addLine(2);
    cminter.addLine(4);
    cminter.addLine(9);
    ASSERT_EQ("2,4,9", cminter.getBSlines());
    ASSERT_EQ(3,cminter.numberOfLines());
}

TEST(TestFirstTask, KorektnaRemoveline) {
    BusStop cminter("Cintorin Slavicie", 2);
    ASSERT_EQ("Cintorin Slavicie", cminter.getName());
    ASSERT_EQ(2,cminter.getStopNumber());
    ASSERT_EQ("", cminter.getBSlines());
    cminter.addLine(2);
    cminter.addLine(4);
    cminter.addLine(9);
    cminter.removeLine(4);
    ASSERT_EQ("2,9", cminter.getBSlines());
    cminter.removeLine(2);
    ASSERT_EQ("9", cminter.getBSlines());
    ASSERT_EQ(1,cminter.numberOfLines());

}


TEST(TestFirstTask, PrazdneMeno) {
    try{
        BusStop cminter("", 2);
    }catch (Exception &e){
        ASSERT_EQ(WrongBusStopName, e.message());
    }
}

TEST(TestFirstTask, ZleCislo) {
    try{
        BusStop cminter("Holicova", -1);
    }catch (Exception &e){
        ASSERT_EQ(WrongBusStopNumber, e.message());
    }
}


TEST(TestFirstTask, ZmenMenoCislo) {
    BusStop cminter("Cintorin Slavicie", 2);
    cminter.changeName("Rokfort");
    ASSERT_EQ("Rokfort", cminter.getName());
    cminter.changeNumLine(120);
    ASSERT_EQ(120, cminter.getStopNumber());
}

TEST(TestFirstTask, ZmenMenoCisloNespr) {

    BusStop cminter("Cintorin Slavicie", 2);
    try{
        cminter.changeName("");
    }catch (Exception &e){
        ASSERT_EQ(WrongBusStopName, e.message());
    }
    try{
        cminter.changeNumLine(-10);
    }catch (Exception &e){
        ASSERT_EQ(WrongBusStopNumber, e.message());
    }
    ASSERT_EQ("Cintorin Slavicie", cminter.getName());
    ASSERT_EQ(2, cminter.getStopNumber());
    cminter.changeName("Rokfort");
    ASSERT_EQ("Rokfort", cminter.getName());
    cminter.changeNumLine(120);
    ASSERT_EQ(120, cminter.getStopNumber());

}

TEST(TestFirstTask, BusLineINIT){
    BusLine tridevat(39);
    ASSERT_EQ(39, tridevat.getLineNum());
    ASSERT_EQ("",tridevat.getStopsString());
    ASSERT_EQ(-1,tridevat.getIntervalWeekends());

    ASSERT_FALSE(tridevat.isLineInOrder());
    ASSERT_FALSE(tridevat.changeStatus());

    ASSERT_EQ(-1,tridevat.getIntervalWorkdays());
    tridevat.setIntervalWorkdays(60);

}

TEST(TestFirstTask, BusLineOK){
    BusLine tridevat(39);
    ASSERT_EQ(39, tridevat.getLineNum());

    ASSERT_TRUE(tridevat.setIntervalWeekends(40));
    ASSERT_EQ(40,tridevat.getIntervalWeekends());
    tridevat.setIntervalWorkdays(60);
    ASSERT_EQ(60,tridevat.getIntervalWorkdays());
    ASSERT_FALSE(tridevat.isLineInOrder());
    ASSERT_TRUE(tridevat.changeStatus());
    ASSERT_TRUE(tridevat.isLineInOrder());
}

TEST(TestFirstTask, AddStops){
    BusLine tridevat(39, 1);
    BusStop cminter("Cintorin Slavicie", 1);
    BusStop zoo("Zoo", 2);
    BusStop lanf("Lanfranconi", 3);
    BusStop kralud("Kralovske udolie", 4);
    BusStop chaso("Chatam sofer", 5);
    tridevat.setIntervalWeekends(40);
    tridevat.setIntervalWorkdays(60);

    tridevat.addStop(cminter);
    ASSERT_EQ("Cintorin Slavicie", tridevat.getStopsString());
    tridevat.addStop(zoo);
    tridevat.addStop(lanf);
    tridevat.addStop(kralud);
    ASSERT_EQ("Cintorin Slavicie --> Zoo --> Lanfranconi --> Kralovske udolie", tridevat.getStopsString());
    tridevat.changeDirection(0);
    ASSERT_EQ("Cintorin Slavicie <--> Zoo <--> Lanfranconi <--> Kralovske udolie", tridevat.getStopsString());
    tridevat.changeDirection(-1);
    ASSERT_EQ("Cintorin Slavicie <-- Zoo <-- Lanfranconi <-- Kralovske udolie", tridevat.getStopsString());

}

TEST(TestFirstTask, RemoveStops){
    BusLine tridevat(39, 1);
    BusStop cminter("Cintorin Slavicie", 1);
    BusStop zoo("Zoo", 2);
    BusStop lanf("Lanfranconi", 3);
    BusStop kralud("Kralovske udolie", 4);
    BusStop chaso("Chatam Sofer", 5);
    tridevat.setIntervalWeekends(40);
    tridevat.setIntervalWorkdays(60);
    ASSERT_FALSE(tridevat.isLineInOrder());
    ASSERT_TRUE(tridevat.changeStatus());
    ASSERT_TRUE(tridevat.isLineInOrder());
    tridevat.addStop(cminter);
    ASSERT_EQ("Cintorin Slavicie", tridevat.getStopsString());
    tridevat.addStop(zoo);
    tridevat.addStop(lanf);
    tridevat.addStop(kralud);
    ASSERT_EQ("Cintorin Slavicie --> Zoo --> Lanfranconi --> Kralovske udolie", tridevat.getStopsString());
    tridevat.removeStop(kralud);
    ASSERT_EQ("Cintorin Slavicie --> Zoo --> Lanfranconi", tridevat.getStopsString());
    tridevat.changeDirection(0);
    tridevat.removeStop(lanf);
    ASSERT_EQ("Cintorin Slavicie <--> Zoo", tridevat.getStopsString());
    tridevat.changeDirection(-1);
    tridevat.addStop(chaso);
    ASSERT_EQ("Cintorin Slavicie <-- Zoo <-- Chatam Sofer", tridevat.getStopsString());
}

TEST(TestFirstTask, BadBusLIne){
    try{
        BusLine zla(-1);
    }catch (Exception &e){
        ASSERT_EQ(WrongBusLineNumber, e.message());
    }
    try{
        BusLine zla(49, 43);
    }catch (Exception &e){
        ASSERT_EQ(WrongDirection, e.message());
    }
    try{
        BusLine zla(49, -43);
    }catch (Exception &e){
        ASSERT_EQ(WrongDirection, e.message());
    }
    try{
        BusLine a(39, -1, 30, 1);
    }catch (Exception &e){
            ASSERT_EQ(OutsideWorkdayInterval, e.message());
        }
    try{
        BusLine a(39, 6, -30, 1);
    }catch (Exception &e){
        ASSERT_EQ(OutsideWeekendInterval, e.message());
    }
    try{
        BusLine a(39, 500, 30, 1);
    }catch (Exception &e){
        ASSERT_EQ(OutsideWorkdayInterval, e.message());
    }
    try{
        BusLine a(39, 50, 600, 1);
    }catch (Exception &e){
        ASSERT_EQ(OutsideWeekendInterval, e.message());
    }
}

TEST(TestFirstTask, TimeTable){
    BusLine tridevat(39, 30, 40);

    ASSERT_EQ("Pondelok az piatok\n"
              "0| 30\n"
              "1| 0 30\n"
              "2| 0 30\n"
              "3| 0 30\n"
              "4| 0 30\n"
              "5| 0 30\n"
              "6| 0 30\n"
              "7| 0 30\n"
              "8| 0 30\n"
              "9| 0 30\n"
              "10| 0 30\n"
              "11| 0 30\n"
              "12| 0 30\n"
              "13| 0 30\n"
              "14| 0 30\n"
              "15| 0 30\n"
              "16| 0 30\n"
              "17| 0 30\n"
              "18| 0 30\n"
              "19| 0 30\n"
              "20| 0 30\n"
              "21| 0 30\n"
              "22| 0 30\n"
              "Vikendy\n"
              "00| 40\n"
              "01| 20\n"
              "02| 0 40\n"
              "03| 20\n"
              "04| 0 40\n"
              "05| 20\n"
              "06| 0 40\n"
              "07| 20\n"
              "08| 0 40\n"
              "09| 20\n"
              "10| 0 40\n"
              "11| 20\n"
              "12| 0 40\n"
              "13| 20\n"
              "14| 0 40\n"
              "15| 20\n"
              "16| 0 40\n"
              "17| 20\n"
              "18| 0 40\n"
              "19| 20\n"
              "20| 0 40\n"
              "21| 20\n"
              "22| 0 40\n"
              "23| 20\n"
    , tridevat.getTimetable());
    tridevat.setIntervalWorkdays(22);
    ASSERT_EQ(22,tridevat.getIntervalWorkdays());
    ASSERT_EQ(40, tridevat.getIntervalWeekends());
    ASSERT_EQ("Pondelok az piatok\n"
              "0| 22 44\n"
              "1| 6 28 50\n"
              "2| 12 34 56\n"
              "3| 18 40\n"
              "4| 2 24 46\n"
              "5| 8 30 52\n"
              "6| 14 36 58\n"
              "7| 20 42\n"
              "8| 4 26 48\n"
              "9| 10 32 54\n"
              "10| 16 38\n"
              "11| 0 22 44\n"
              "12| 6 28 50\n"
              "13| 12 34 56\n"
              "14| 18 40\n"
              "15| 2 24 46\n"
              "16| 8 30 52\n"
              "17| 14 36 58\n"
              "18| 20 42\n"
              "19| 4 26 48\n"
              "20| 10 32 54\n"
              "21| 16 38\n"
              "22| 0 22 44\n"
              "Vikendy\n"
              "00| 40\n"
              "01| 20\n"
              "02| 0 40\n"
              "03| 20\n"
              "04| 0 40\n"
              "05| 20\n"
              "06| 0 40\n"
              "07| 20\n"
              "08| 0 40\n"
              "09| 20\n"
              "10| 0 40\n"
              "11| 20\n"
              "12| 0 40\n"
              "13| 20\n"
              "14| 0 40\n"
              "15| 20\n"
              "16| 0 40\n"
              "17| 20\n"
              "18| 0 40\n"
              "19| 20\n"
              "20| 0 40\n"
              "21| 20\n"
              "22| 0 40\n"
              "23| 20\n", tridevat.getTimetable());
}