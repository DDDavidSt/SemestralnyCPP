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
    ASSERT_EQ("(39:10:(mins)6); (29:3:(mins)9)", to_string(ba.whereNext(ba.getBusStopById(9))));
    ASSERT_EQ("", to_string(ba.whereNext(ba.getBusStopById(15))));
    ASSERT_EQ("(4:14:(mins)12); (29:2:(mins)5); (29:4:(mins)10)", to_string(ba.whereNext(ba.getBusStopById(7))));
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
    ASSERT_EQ("(29:7:(mins)10); (29:3:(mins)13)", to_string(adjma[4]));
    ASSERT_EQ("(29:6:(mins)4); (29:2:(mins)7)", to_string(adjma[5]));
    ASSERT_EQ("(29:5:(mins)4)", to_string(adjma[6]));
    ASSERT_EQ("(4:14:(mins)12); (29:2:(mins)5); (29:4:(mins)10)", to_string(adjma[7]));
    ASSERT_EQ("(4:7:(mins)13)", to_string(adjma[8]));
    ASSERT_EQ("(39:10:(mins)6); (29:3:(mins)9)", to_string(adjma[9]));
    ASSERT_EQ("(39:11:(mins)7)", to_string(adjma[10]));
    ASSERT_EQ("(39:12:(mins)6)", to_string(adjma[11]));
    ASSERT_EQ("(39:13:(mins)10)", to_string(adjma[12]));
    ASSERT_EQ("(39:14:(mins)4)", to_string(adjma[13]));
    ASSERT_EQ("(39:15:(mins)13); (4:15:(mins)21)", to_string(adjma[14]));
    ASSERT_EQ("", to_string(adjma[15]));
    ASSERT_EQ("", to_string(adjma[16]));
}

TEST(TestBFSNetwork, FindShortestRoute){
    PTNetwork ba;
    ba.readStopsAndLines("read_from_me.txt");
    std::vector<pair<int,int>> result = { {9, 39}, {10, 39}, {11, 39}, {12, 39}, {13, 39}, {14, 39}, {15, 0} };
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(9), ba.getBusStopById(15)));
    result = { {12, 39}, {13, 39}, {14, 39}, {15, 0} };
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(12), ba.getBusStopById(15)));
    result = { {7, 4}, {14, 39}, {15, 0} };
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(7), ba.getBusStopById(15)));

    ba.getBusLineByNum(35).changeStatus();
    ASSERT_TRUE(ba.getBusLineByNum(35).isLineInOrder());

    result = { {7, 35}, {13, 39}, {14, 39}, {15, 0} };
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(7), ba.getBusStopById(15)));
    result = {};
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(1), ba.getBusStopById(15)));
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(15), ba.getBusStopById(1)));
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(15), ba.getBusStopById(15)));
    ASSERT_TRUE(ba.getBusLineByNum(35).isLineInOrder());
    result = { {2, 29}, {5, 35}, {10, 0} };
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(2), ba.getBusStopById(10)));
}

TEST(TestBFSNetwork, FindShortestNasty){
    PTNetwork ba;
    ba.readStopsAndLines("read_from_me.txt");
    std::vector<pair<int,int>> result = {};
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(1), ba.getBusStopById(15)));
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(15), ba.getBusStopById(1)));
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(15), ba.getBusStopById(15)));
    ba.getBusLineByNum(35).changeStatus();
    result = { {2, 29}, {5, 35}, {10, 0} };
    ASSERT_EQ(result, ba.findShortestPath(ba.getBusStopById(2), ba.getBusStopById(10)));
}

TEST(TestBFSNetwork, GetRoute){
    PTNetwork ba;
    ba.readStopsAndLines("read_from_me.txt");
    Time cas(0,0);
    ASSERT_EQ("39: (00:00)Cintorin Slavicie -00:06-> (00:06)Televizia -00:07-> (00:13)Zoo -00:06-> (00:19)Lanfranconi -00:10-> (00:29)Kralovske udolie -00:04-> (00:33)Chatam sofer -00:13-> (00:46)Zochova"
    , ba.getRoute(ba.getBusStopById(9), ba.getBusStopById(15), cas));
    ASSERT_EQ("00:00", cas.getTime());
    ba.getBusLineByNum(35).changeStatus();
    ASSERT_EQ("35: (01:14)Cintorin Slavicie -00:19-> (01:33)Chatam sofer /-prestup- 39 -/ (01:33)Chatam sofer -00:13-> (01:46)Zochova"
    , ba.getRoute(ba.getBusStopById(9), ba.getBusStopById(15), cas));
    ASSERT_EQ("No path from Zochova to Zochova"
    , ba.getRoute(ba.getBusStopById(15), ba.getBusStopById(15), cas));
    ASSERT_EQ("No path from Dobrovicova to Zochova"
    , ba.getRoute(ba.getBusStopById(1), ba.getBusStopById(15), cas));
    ASSERT_EQ("No path from Zochova to Dobrovicova"
    , ba.getRoute(ba.getBusStopById(15), ba.getBusStopById(1), cas));

    cas.setTime(13,42);

    ASSERT_EQ("39: (13:59)Lanfranconi -00:10-> (14:09)Kralovske udolie -00:04-> (14:13)Chatam sofer -00:13-> (14:26)Zochova", ba.getRoute(ba.getBusStopById(12), ba.getBusStopById(15),cas));

    cas.setTime(23,45);

    ASSERT_EQ("35: (23:48)Most SNP -00:03-> (23:51)Kralovske udolie /-prestup- 39 -/ (00:29)Kralovske udolie -00:04-> (00:33)Chatam sofer -00:13-> (00:46)Zochova",
              ba.getRoute(ba.getBusStopById(7), ba.getBusStopById(15),cas));


}