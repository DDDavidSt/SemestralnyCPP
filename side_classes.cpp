//
// Created by david on 18/12/22.
//
#include "side_classes.h"
#include <iomanip>
#include <sstream>

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
