//
// Created by david on 18/12/22.
//
#include <iostream>
#include "side_classes.h"
#include "BusStop.h"

BusStop::BusStop(const std::string& stop_name, int num) {
    if (stop_name.empty()){
        throw Exception(WrongBusStopName);
    }
    if (num <= 0){
        throw Exception(WrongBusStopNumber);
    }
    name = stop_name;
    number = num;
}


std::string BusStop::getBSlines() const{
    std::string res;
    for(int i: lines){
        res += std::to_string(i) + ",";
    }
    return res.substr(0,res.size()-1);
}

bool BusStop::addLine(int line_num){
    if(line_num <= 0){
        throw Exception(WrongLineNum);
    }
    lines.push_back(line_num);
    return true;
}

bool BusStop::removeLine(int line_num) {
    if(lines.empty()){
        throw Exception("No line to delete");
    }
    for(auto it = lines.begin(); it != lines.end(); it++){
        if(*it == line_num){
            lines.erase(it);
            return true;
        }
    }
    throw Exception("Line number not found");
}

bool BusStop::changeName(std::string new_name) {
    if(new_name.empty()){
        throw Exception(WrongBusStopName);
    }
    name = std::move(new_name);
    return true;
}

bool BusStop::changeNumLine(int new_num) {
    if(new_num < 0){
        throw Exception(WrongBusStopNumber);
    }
    number = new_num;
    return true;
}