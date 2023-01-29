//
// Created by david on 18/12/22.
//
#include <iostream>
#include "side_classes.h"
#include "BusStop.h"

bool checkValidName(const std::string &stop_name){
    if (!stop_name.empty() && (stop_name[0] >= 'A' && stop_name[0] <= 'Z' || stop_name[0] >= 'a' && stop_name[0] <= 'z')){
        return true;
    }
    return false;
}

BusStop::BusStop(const std::string& stop_name, int num) {
    if (!checkValidName(stop_name)){
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

bool BusStop::changeName(const std::string &new_name) {
    if(!checkValidName(new_name)){
        throw Exception(WrongBusStopName);
    }
    name = std::move(new_name);
    return true;
}

bool BusStop::changeStopNum(int new_num) {
    if(new_num < 0){
        throw Exception(WrongBusStopNumber);
    }
    number = new_num;
    return true;
}