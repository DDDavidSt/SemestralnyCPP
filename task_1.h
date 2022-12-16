//
// Created by david on 14/12/22.
//
#include <iostream>
#include <vector>

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

class BusLine{
    int direction = 0; //which stop is a starting point (-1 last one to first one, 1 => first to last in stops list, 0 => both ways)
    std::vector<std::pair<BusStop, int>> stops;
    int interval_workdays = -1;
    int interval_weekends = -1;
    int line_num = -1;
    bool status = false;

public:
    BusLine(int number, int direct=1);
    BusLine(int number,  int workdays_interval, int weekends_interval,int direct = 1);
    bool changeDirection(int new_direct);
    bool isLineInOrder() const {return status;};
    std::string getStopsString() const;
    std::vector<std::pair<BusStop,int>> getStopVector() const { return stops;};
    bool changeStatus(); //cannot change if some of the variables is negative or not initialized except for stops
    bool setIntervalWorkdays(int work_int);
    bool setIntervalWeekends(int wknd_int);
    std::string getTimetable() const;
    void timetableToFile(std::string file) const;
    bool changeLineNum(int new_line_num) ;
    int getLineNum() const {return line_num;};
    int getIntervalWorkdays() const {return interval_workdays;};
    int getIntervalWeekends() const{return interval_weekends;};
    bool addStop(int position, BusStop &new_stop, int mins_from_prev, int mins_from_next) ; //inserts a new stop at the postion in vector
    bool removeStop(BusStop &stop_rem, int mins_prev_to_next = -1); //if mins_prev_to_next not sepcified then just uses sum of the neighbouring elements around stop_rem
    std::vector<std::pair<BusStop, Time>> getEarliestFromStop(BusStop &start, BusStop &dest, Time &time, bool weekend=false);//return vector of stops and their scheduled departure
    std::string getEarliestFromStopString(BusStop &start, BusStop &dest, Time &time, bool weekend=false);
};