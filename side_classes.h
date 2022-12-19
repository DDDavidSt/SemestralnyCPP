//
// Created by david on 18/12/22.
//
#include <iostream>

#pragma once
const unsigned int min_interval_mins  = 5;
const unsigned int max_interval_mins = 240;

const std::string WrongBusStopName = "Wrong bus stop name";
const std::string WrongBusStopNumber = "Wrong bus stop number";
const std::string WrongLineNum = "Incorrect line number";
const std::string WrongBusLineNumber = "Wrong bus line number";
const std::string WrongDirection = "Unknown direction - use values -1,0,1 only!";
const std::string OutsideWeekendInterval = "Interval for weekends outside range <" + std::to_string(min_interval_mins)+ "," +
                                           std::to_string(max_interval_mins) + ">!";
const std::string OutsideWorkdayInterval = "Interval for workdays outside range <" + std::to_string(min_interval_mins)+ "," +
                                           std::to_string(max_interval_mins) + ">!";



class Exception : public std::exception {
    std::string mess;
public:
    explicit Exception(std::string message) : mess(std::move(message)) {}
    std::string message(){
        return mess;
    }
};

class Time{
    int hours = 0;
    int minutes = 0;
public:
    Time();
    Time(int hours0, int minutes0);
    explicit Time(std::string& time);
    bool setTime(int hours0, int minutes0);
    std::string getTime() const;
    std::pair<int,int> getTimePair() { return std::make_pair(hours, minutes);};
    Time operator +(Time &other);
    Time operator -(Time &other);
    Time operator +=(Time &other);
    Time operator -=(Time &other);
    bool operator <(Time &other);
    bool operator >(Time &other);
    bool operator ==(Time &other);
};
