// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include <fstream>
#include "gtest/gtest.h"

#include "BusLine.h"


using namespace std;
using namespace ::testing;

TEST(TestSideClassesAndBusStop, Time){
    Time cas1;
    ASSERT_EQ("00:00", cas1.getTime());
    Time cass2(0,5);
    ASSERT_EQ("23:55", (cas1-cass2).getTime());
    std::pair<int, int> kk;
    kk = std::make_pair(0,0);
    ASSERT_EQ(kk, cas1.getTimePair());
    Time cas2(9,10);
    Time cas3(2,30);
    ASSERT_EQ("11:40", (cas2 + cas3).getTime());
    Time cas4(20, 50);
    ASSERT_EQ("06:00", (cas2+ cas4).getTime());
    Time cas5(21, 55);
    ASSERT_EQ("22:55", (cas4 - cas5).getTime());
    cas5 += cas2;
    ASSERT_EQ("07:05", (cas5).getTime());
    cas5 -= cas2;
    Time cassss5(21,55);
    ASSERT_EQ("21:55", cas5.getTime());
    std::string cas = "23:55";
    Time csc(cas);
    ASSERT_EQ(std::make_pair(23,55), csc.getTimePair());
}

TEST(TestSideClassesAndBusStop, TimeCompare){
    Time vacsi(9,55);
    Time mensi(9,54);
    Time rovny(9,55);
    ASSERT_TRUE(vacsi > mensi);
    ASSERT_TRUE(mensi < vacsi);
    ASSERT_TRUE(vacsi == rovny);
    vacsi.setTime(0,4);
    mensi.setTime(0,0);
    rovny.setTime(23,59);
    ASSERT_TRUE(rovny > mensi);
    ASSERT_TRUE(mensi < rovny);
    ASSERT_TRUE(mensi== mensi);
}

TEST(TestSideClassesAndBusStop, KorektnaZastavka) {
    BusStop cminter("Cintorin Slavicie", 2);
    ASSERT_EQ("Cintorin Slavicie", cminter.getName());
    ASSERT_EQ(2,cminter.getStopNumber());
    cminter.addLine(2);
    cminter.addLine(4);
    cminter.addLine(9);
    ASSERT_EQ("2,4,9", cminter.getBSlines());
    ASSERT_EQ(3,cminter.numberOfLines());
}

TEST(TestSideClassesAndBusStop, KorektnaRemoveline) {
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


TEST(TestSideClassesAndBusStop, PrazdneMeno) {
    try{
        BusStop cminter("", 2);
    }catch (Exception &e){
        ASSERT_EQ(WrongBusStopName, e.message());
    }
}

TEST(TestSideClassesAndBusStop, ZleCislo) {
    try{
        BusStop cminter("Holicova", -1);
    }catch (Exception &e){
        ASSERT_EQ(WrongBusStopNumber, e.message());
    }
}


TEST(TestSideClassesAndBusStop, ZmenMenoCislo) {
    BusStop cminter("Cintorin Slavicie", 2);
    cminter.changeName("Rokfort");
    ASSERT_EQ("Rokfort", cminter.getName());
    cminter.changeNumLine(120);
    ASSERT_EQ(120, cminter.getStopNumber());
}

TEST(TestSideClassesAndBusStop, ZmenMenoCisloNespr) {

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

