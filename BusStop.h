//
// Created by david on 18/12/22.
//
#include <iostream>
#include "side_classes.h"

#pragma once
class BusStop{
    int number = -1;
    std::string name;
    int *lines = new int;
    int number_of_lines = 0;
public:
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
    bool changeName(std::string new_name);
    bool changeNumLine(int new_num);
    int numberOfLines() const {return number_of_lines;};
    std::string getName() const { return name; };
    int getStopNumber() const {return number; };
    std::string getBSlines() const;
};