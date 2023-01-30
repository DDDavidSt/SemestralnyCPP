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
std::string to_string(std::vector<std::tuple<int, int,int>> vec){
    std::string hah;
    for(std::tuple<int, int,int> &i: vec){
        hah += "(" + to_string(std::get<2>(i)) + ":" + to_string(std::get<0>(i))+ ":(mins)"+
                to_string(std::get<1>(i)) +"); ";
    }
    return hah.substr(0,hah.size()-2);
}
TEST(TestBFSNetwork, FindAdjacent){
    PTNetwork ba;
    ba.readStopsAndLines("read_from_me.txt");
    ASSERT_EQ("", to_string(ba.whereNext(ba.getBusStopById(1))));
    ASSERT_EQ("(39:10:(mins)6); (35:14:(mins)19); (29:3:(mins)9)", to_string(ba.whereNext(ba.getBusStopById(9))));
    ASSERT_EQ("", to_string(ba.whereNext(ba.getBusStopById(15))));
    ASSERT_EQ("(4:14:(mins)12); (35:13:(mins)3); (29:2:(mins)5); (29:4:(mins)10)", to_string(ba.whereNext(ba.getBusStopById(7))));
    BusStop zla("Hojdana", 3);
    try{
        ba.whereNext(zla);
    }catch (Exception &e){
        ASSERT_EQ("Given stop Hojdana not in network", e.message());
    }

}

TEST(TestBFSNetwork, CreateAdjMatrix){
    PTNetwork ba;
    ba.readStopsAndLines("read_from_me.txt");
    std::map<int, std::vector<std::tuple<int, int, int>>> adjma = ba.createAdjMap();
    ASSERT_EQ(15, adjma.size());
    ASSERT_EQ("", to_string(adjma[1]));
    ASSERT_EQ("(4:8:(mins)15); (29:5:(mins)7); (29:7:(mins)5)", to_string(adjma[2]));
    ASSERT_EQ("(29:4:(mins)13); (29:9:(mins)9)", to_string(adjma[3]));
    ASSERT_EQ("(35:10:(mins)12); (29:7:(mins)10); (29:3:(mins)13)", to_string(adjma[4]));
    ASSERT_EQ("(29:6:(mins)4); (29:2:(mins)7)", to_string(adjma[5]));
    ASSERT_EQ("(35:4:(mins)16); (29:5:(mins)4)", to_string(adjma[6]));
    ASSERT_EQ("(4:14:(mins)12); (35:13:(mins)3); (29:2:(mins)5); (29:4:(mins)10)", to_string(adjma[7]));
    ASSERT_EQ("(4:7:(mins)13)", to_string(adjma[8]));
    ASSERT_EQ("(39:10:(mins)6); (35:14:(mins)19); (29:3:(mins)9)", to_string(adjma[9]));
    ASSERT_EQ("(39:11:(mins)7); (35:7:(mins)11)", to_string(adjma[10]));
    ASSERT_EQ("(39:12:(mins)6)", to_string(adjma[11]));
    ASSERT_EQ("(39:13:(mins)10); (35:9:(mins)17)", to_string(adjma[12]));
    ASSERT_EQ("(39:14:(mins)4); (35:12:(mins)12)", to_string(adjma[13]));
    ASSERT_EQ("(39:15:(mins)13); (4:15:(mins)21)", to_string(adjma[14]));
    ASSERT_EQ("", to_string(adjma[15]));
    ASSERT_EQ("", to_string(adjma[16]));
}