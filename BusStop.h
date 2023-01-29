//
// Created by david on 18/12/22.
//
#include <iostream>
#include <vector>
#include "side_classes.h"

#pragma once
class BusStop{
    int number = -1;
    std::string name;
    std::vector<int> lines;
public:
    BusStop(): number(-1), name(""){};
    BusStop(const std::string& stop_name, int num);
    bool operator == (BusStop const &b1){
        if(b1.getName() == name && b1.getStopNumber() == number){
            return true;
        }
        return false;
    };
    bool operator != (BusStop const &b1){
        if(b1.getName() != name || b1.getStopNumber() != number){
            return true;
        }
        return false;
    };
    bool addLine(int line_num);
    bool removeLine(int line_num);
    bool changeName(const std::string &new_name);
    bool changeStopNum(int new_num);
    int numberOfLines() const {return lines.size();};
    std::string getName() const { return name; };
    int getStopNumber() const {return number; };
    std::vector<int> getCrossingLines(){return lines;};
    std::string getBSlines() const;
};