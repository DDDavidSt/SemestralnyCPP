// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "BusLine.h"


using namespace std;
using namespace ::testing;

TEST(TestBusLine, BusLineINIT) {
    BusLine tridevat(39);
    ASSERT_EQ(39, tridevat.getLineNum());
    ASSERT_EQ("", tridevat.getStopsString());
    ASSERT_EQ(-1, tridevat.getIntervalWeekends());

    ASSERT_FALSE(tridevat.isLineInOrder());
    ASSERT_FALSE(tridevat.changeStatus());

    ASSERT_EQ(-1, tridevat.getIntervalWorkdays());
    tridevat.setIntervalWorkdays(60);

}

TEST(TestBusLine, BusLineOK) {
    BusLine tridevat(39);
    ASSERT_EQ(39, tridevat.getLineNum());

    ASSERT_TRUE(tridevat.setIntervalWeekends(40));
    ASSERT_EQ(40, tridevat.getIntervalWeekends());
    tridevat.setIntervalWorkdays(60);
    ASSERT_EQ(60, tridevat.getIntervalWorkdays());
    ASSERT_FALSE(tridevat.isLineInOrder());
    ASSERT_TRUE(tridevat.changeStatus());
    ASSERT_TRUE(tridevat.isLineInOrder());
}

TEST(TestBusLine, AddStops) {
    BusLine tridevat(39, 1);
    BusStop cminter("Cintorin Slavicie", 1);
    BusStop zoo("Zoo", 2);
    BusStop lanf("Lanfranconi", 3);
    BusStop kralud("Kralovske udolie", 4);
    tridevat.setIntervalWeekends(40);
    tridevat.setIntervalWorkdays(60);

    ASSERT_TRUE(tridevat.addStop(-1, cminter, 0, 0));
    ASSERT_EQ("Cintorin Slavicie", tridevat.getStopsString());
    tridevat.addStop(-1, zoo, 10, 0);
    tridevat.addStop(-1, lanf, 13, 0);
    tridevat.addStop(-1, kralud, 6, 0);
    ASSERT_EQ("Cintorin Slavicie -10-> Zoo -13-> Lanfranconi -6-> Kralovske udolie", tridevat.getStopsString());
    tridevat.changeDirection(0);
    ASSERT_EQ("Cintorin Slavicie <-10-> Zoo <-13-> Lanfranconi <-6-> Kralovske udolie", tridevat.getStopsString());
    tridevat.changeDirection(-1);
    ASSERT_EQ("Kralovske udolie -6-> Lanfranconi -13-> Zoo -10-> Cintorin Slavicie", tridevat.getStopsString());

}

TEST(TestBusLine, RemoveStops) {
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
    tridevat.addStop(-1, cminter, 0, 0);
    ASSERT_EQ("Cintorin Slavicie", tridevat.getStopsString());
    tridevat.addStop(-1, zoo, 10, 0);
    tridevat.addStop(-1, lanf, 13, 0);
    ASSERT_EQ("Cintorin Slavicie -10-> Zoo -13-> Lanfranconi", tridevat.getStopsString());
    tridevat.addStop(1, kralud, 6, 9);
    ASSERT_EQ("Cintorin Slavicie -6-> Kralovske udolie -9-> Zoo -13-> Lanfranconi", tridevat.getStopsString());
    ASSERT_FALSE(tridevat.addStop(340, kralud, 6, 9));
    ASSERT_FALSE(tridevat.addStop(1, kralud, 6, 9));
    tridevat.
            removeStop(kralud,
                       10);
    ASSERT_EQ("Cintorin Slavicie -10-> Zoo -13-> Lanfranconi", tridevat.getStopsString());
    tridevat.changeDirection(0);
    tridevat.
            removeStop(lanf,
                       0);
    ASSERT_EQ("Cintorin Slavicie <-10-> Zoo", tridevat.getStopsString());
    tridevat.changeDirection(-1);
    tridevat.addStop(2, chaso, 20, 0);
    ASSERT_EQ("Zoo -10-> Cintorin Slavicie -20-> Chatam Sofer", tridevat.getStopsString());
    tridevat.
            removeStop(zoo,
                       0);
    tridevat.
            removeStop(cminter,
                       0);
    ASSERT_EQ("Chatam Sofer", tridevat.getStopsString());
    tridevat.
            removeStop(chaso,
                       0);
    ASSERT_EQ("", tridevat.getStopsString());

}

TEST(TestBusLine, BadBusLIne) {
    try {
        BusLine zla(-1);
    } catch (
            Exception &e
    ) {
        ASSERT_EQ(WrongBusLineNumber, e.message());
    }
    try {
        BusLine zla(49, 43);
    } catch (
            Exception &e
    ) {
        ASSERT_EQ(WrongDirection, e.message());
    }
    try {
        BusLine zla(49, -43);
    } catch (
            Exception &e
    ) {
        ASSERT_EQ(WrongDirection, e.message());
    }
    try {
        BusLine a(39, -1, 30, 1);
    } catch (
            Exception &e
    ) {
        ASSERT_EQ(OutsideWorkdayInterval, e.message());
    }
    try {
        BusLine a(39, 6, -30, 1);
    } catch (
            Exception &e
    ) {
        ASSERT_EQ(OutsideWeekendInterval, e.message());
    }
    try {
        BusLine a(39, 500, 30, 1);
    } catch (
            Exception &e
    ) {
        ASSERT_EQ(OutsideWorkdayInterval, e.message());
    }
    try {
        BusLine a(39, 50, 600, 1);
    } catch (
            Exception &e
    ) {
        ASSERT_EQ(OutsideWeekendInterval, e.message());
    }
}

TEST(TestBusLine, TimeTable) {
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
              "23| 20\n", tridevat.getTimetable());
    tridevat.setIntervalWorkdays(22);
    ASSERT_EQ(22, tridevat.getIntervalWorkdays());
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

TEST(TestBusLine, getNearestConn) {
    BusLine tridevat(39, 1);
    BusStop cminter("Cintorin Slavicie", 1);
    BusStop zoo("Zoo", 2);
    BusStop lanf("Lanfranconi", 3);
    BusStop kralud("Kralovske udolie", 4);
    BusStop chaso("Chatam Sofer", 5);
    tridevat.setIntervalWeekends(40);
    tridevat.setIntervalWorkdays(22);
    tridevat.addStop(-1, cminter, 0, 0);
    tridevat.addStop(-1, zoo, 10, 0);
    tridevat.addStop(-1, lanf, 13, 0);
    tridevat.addStop(1, kralud, 6, 9);
    tridevat.changeStatus();
    ASSERT_EQ("Cintorin Slavicie -6-> Kralovske udolie -9-> Zoo -13-> Lanfranconi", tridevat.getStopsString());
    Time cas1(0, 44);
    ASSERT_EQ("(00:44)Cintorin Slavicie -00:06-> (00:50)Kralovske udolie -00:09-> (00:59)Zoo",
              tridevat.getEarliestFromStopString(cminter, zoo, cas1));
    tridevat.changeDirection(-1);
    ASSERT_EQ(
            "(00:44)Lanfranconi -00:13-> (00:57)Zoo -00:09-> (01:06)Kralovske udolie -00:06-> (01:12)Cintorin Slavicie",
            tridevat.getEarliestFromStopString(lanf, cminter, cas1));
    ASSERT_EQ("(00:57)Zoo -00:09-> (01:06)Kralovske udolie", tridevat.getEarliestFromStopString(zoo, kralud, cas1));
    tridevat.changeDirection(1);
    ASSERT_EQ("(00:44)Cintorin Slavicie -00:06-> (00:50)Kralovske udolie -00:09-> (00:59)Zoo",
              tridevat.getEarliestFromStopString(cminter, zoo, cas1));

    tridevat.changeDirection(0);
    tridevat.addStop(-1, chaso, 9, 0);
    ASSERT_EQ(
            "(00:44)Cintorin Slavicie -00:06-> (00:50)Kralovske udolie -00:09-> (00:59)Zoo -00:13-> (01:12)Lanfranconi",
            tridevat.getEarliestFromStopString(cminter, lanf, cas1));
    ASSERT_EQ("(00:44)Cintorin Slavicie -00:06-> (00:50)Kralovske udolie",
              tridevat.getEarliestFromStopString(cminter, kralud, cas1));
    ASSERT_EQ("(00:50)Kralovske udolie -00:09-> (00:59)Zoo", tridevat.getEarliestFromStopString(kralud, zoo, cas1));
    ASSERT_EQ(
            "(00:44)Chatam Sofer -00:09-> (00:53)Lanfranconi -00:13-> (01:06)Zoo -00:09-> (01:15)Kralovske udolie -00:06-> (01:21)Cintorin Slavicie",
            tridevat.getEarliestFromStopString(chaso, cminter, cas1));
    ASSERT_EQ("(00:44)Zoo -00:09-> (00:53)Kralovske udolie", tridevat.getEarliestFromStopString(zoo, kralud, cas1));
    ASSERT_EQ("(00:50)Kralovske udolie -00:09-> (00:59)Zoo", tridevat.getEarliestFromStopString(kralud, zoo, cas1));
    ASSERT_EQ(
            "(01:20)Chatam Sofer -00:09-> (01:29)Lanfranconi -00:13-> (01:42)Zoo -00:09-> (01:51)Kralovske udolie -00:06-> (01:57)Cintorin Slavicie",
            tridevat.getEarliestFromStopString(chaso, cminter, cas1, true));
    ASSERT_EQ("(01:02)Zoo -00:09-> (01:11)Kralovske udolie",
              tridevat.getEarliestFromStopString(zoo, kralud, cas1, true));


}

TEST(TestBusLine, NastyConnections) {
    BusLine tridevat(39, 1);
    BusStop cminter("Cintorin Slavicie", 1);
    BusStop zoo("Zoo", 2);
    BusStop lanf("Lanfranconi", 3);
    BusStop kralud("Kralovske udolie", 4);
    BusStop chaso("Chatam Sofer", 5);
    tridevat.setIntervalWeekends(40);
    tridevat.setIntervalWorkdays(22);
    tridevat.addStop(-1, cminter, 0, 0);
    tridevat.addStop(-1, zoo, 10, 0);
    tridevat.addStop(-1, lanf, 13, 0);
    tridevat.addStop(1, kralud, 6, 9);
    tridevat.changeStatus();
    Time cas1(23, 59);

    ASSERT_EQ(
            "(00:00)Cintorin Slavicie -00:06-> (00:06)Kralovske udolie -00:09-> (00:15)Zoo -00:13-> (00:28)Lanfranconi",
            tridevat.getEarliestFromStopString(cminter, lanf, cas1));

    try {
        tridevat.
                getEarliestFromStopString(cminter, chaso, cas1
        );
    } catch (
            Exception &e
    ) {
        ASSERT_EQ("Given destination is not in Line number 39", e.message());
    }
    try {
        tridevat.
                getEarliestFromStopString(chaso, cminter, cas1
        );
    } catch (
            Exception &e
    ) {
        ASSERT_EQ("Given start point is not in Line number 39", e.message());
    }
    tridevat.changeDirection(0);
    tridevat.
            removeStop(zoo);
    ASSERT_EQ("(00:00)Cintorin Slavicie -00:06-> (00:06)Kralovske udolie -00:22-> (00:28)Lanfranconi",
              tridevat.getEarliestFromStopString(cminter, lanf, cas1));
    try {
        tridevat.
                getEarliestFromStopString(cminter, lanf, cas1
        );
    } catch (
            Exception &e
    ) {
        ASSERT_EQ("Given destination is not in Line number 39", e.message());
    }
    try {
        tridevat.
                getEarliestFromStopString(chaso, lanf, cas1
        );
    } catch (
            Exception &e
    ) {
        ASSERT_EQ("Given start point is not in Line number 39", e.message());
    }
    ASSERT_EQ("(00:00)Cintorin Slavicie -00:06-> (00:06)Kralovske udolie",
              tridevat.getEarliestFromStopString(cminter, kralud, cas1));
    ASSERT_EQ("(00:00)Cintorin Slavicie", tridevat.getEarliestFromStopString(cminter, cminter, cas1));
}
