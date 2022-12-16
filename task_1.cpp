#include <iostream>
#include <sstream>
#include <iomanip>
#include "task_1.h"

Time::Time(){
    hours = 0;
    minutes = 0;
}
bool isTimeOK(int hours,int minutes){
    if (hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60) {
        return true;
    }
    return false;
}
Time::Time(int hours0, int minutes0) {
    setTime(hours0, minutes0);
}
bool Time::setTime(int hours0, int minutes0){
    if (isTimeOK(hours0, minutes0)) {
        hours = 0;
        minutes = 0;
    } else {
        hours = hours0;
        minutes = minutes0;
    }
}
std::string Time::getTime() const {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":" <<std::setw(2)<< minutes;
    return ss.str();
}

Time Time::operator+(Time &other) {
    Time newTime;
    if(minutes + other.minutes >= 60) {
        newTime.hours += 1;
    }
    newTime.minutes = (minutes + other.minutes)%60;
    newTime.hours += (hours + other.hours)%24;
    return newTime;
}

Time Time::operator-(Time &other) {
    Time newTime;
    if(minutes - other.minutes < 0) {
        newTime.hours -= 1;
    }
    newTime.minutes = (minutes - other.minutes)%60;
    newTime.hours = (hours - other.hours)%24;
    if(newTime.minutes < 0){
        newTime.hours -= 1;
        newTime.minutes = 60 + newTime.minutes;
    }
    if(newTime.hours < 0){
        newTime.hours = 24 + newTime.hours;
    }
    return newTime;
}
Time Time::operator+=(Time &other) {
    if(minutes + other.minutes >= 60) {
        hours += 1;
    }
    minutes = (minutes + other.minutes)%60;
    hours = (hours + other.hours)%24;
}

Time Time::operator-=(Time &other) {
    if(minutes - other.minutes < 0) {
        hours -= 1;
    }
    minutes = (minutes - other.minutes)%60;
    hours = (hours - other.hours)%24;
    if(minutes < 0){
        minutes = 60 + minutes;
    }
    if(hours < 0){
        hours = 24 + hours;
    }
}

bool Time::operator<(Time &other) {
    if(hours < other.hours){
        return true;
    }if(hours == other.hours && minutes < other.minutes){
        return true;
    }
    return false;
}

bool Time::operator>(Time &other) {
    if(hours > other.hours){
        return true;
    }if(hours == other.hours && minutes > other.minutes){
        return true;
    }
    return false;
}

bool Time::operator==(Time &other) {
    return hours == other.hours && minutes == other.minutes;
}


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