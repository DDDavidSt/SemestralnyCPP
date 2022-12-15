#include <iostream>
#include "task_1.h"

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
    for(int i = 0; i < number_of_lines; ++i){
        res += std::to_string(lines[i]) + ",";
    }
    return res.substr(0,res.size()-1);
}

bool BusStop::addLine(int line_num){
    if(line_num <= 0){
        throw Exception(WrongLineNum);
    }
    lines[number_of_lines] = line_num;
    number_of_lines += 1;
    return true;
}

bool BusStop::removeLine(int line_num) {
    if(number_of_lines == 0){
        throw Exception("No line to delete");
    }
    int *newarr = new int[number_of_lines -1];
    bool found = false;
    for(int i = 0; i < number_of_lines; ++i){
        if(lines[i] == line_num) {
            found = true;
        }
    }
    if(found){
        int counter = 0;
        for(int i = 0; i < number_of_lines; ++i){
            if(lines[i] != line_num) {
                newarr[counter] = lines[i];
                ++counter;
            }
        }
        number_of_lines -= 1;
        delete lines;
        lines = newarr;
        return true;
    }else{
        throw Exception("Line number not found");
    }
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