//
// Created by david on 14/12/22.
//
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "task_1.h"

BusLine::BusLine(int number, int direct) {
    if(number <= 0){
        throw Exception(WrongBusLineNumber);
    }
    if(direct != 0 && direct != 1 && direct != -1){
        throw Exception("Unknown direction - use values -1,0,1 only!");
    }
    line_num = number;
    direction = direct;
}

BusLine::BusLine(int number,  int workdays_interval, int weekends_interval, int direct){
    if(number <= 0){
        throw Exception(WrongBusLineNumber);
    }
    if(direct != 0 && direct != 1 && direct != -1){
        throw Exception(WrongDirection);
    }
    if(weekends_interval < min_interval_mins | weekends_interval > max_interval_mins){
        throw Exception(OutsideWeekendInterval);
    }
    if(workdays_interval < min_interval_mins | workdays_interval > max_interval_mins){
        throw Exception(OutsideWorkdayInterval);
    }
    line_num = number;
    interval_workdays = workdays_interval;
    interval_weekends = weekends_interval;
    direction = direct;
}


bool BusLine::changeDirection(int new_direct) {
    if(new_direct != -1 && new_direct != 0 && new_direct != 1){
        throw Exception(WrongDirection);
    }
    direction = new_direct;
    return true;
}

bool BusLine::changeStatus() {
    if(line_num > 0 && interval_weekends != -1 && interval_workdays != -1){
        if(!status){
            status = true;
            return true;
        }
        status = false;
        return true;
    }else{
        if(!status){
            return false;
        }else{
            status = false;
            return true;
        }
    }
}

bool BusLine::setIntervalWeekends(int wknd_int) {
    if(wknd_int < min_interval_mins | wknd_int > max_interval_mins){
        throw Exception(OutsideWeekendInterval);
    }
    interval_weekends = wknd_int;
    return true;
}

bool BusLine::setIntervalWorkdays(int work_int) {
    if(work_int < min_interval_mins | work_int > max_interval_mins){
        throw Exception(OutsideWorkdayInterval);
    }
    interval_workdays = work_int;
    return true;
}

bool BusLine::changeLineNum(int new_line_num) {
    if(new_line_num <= 0){
        throw Exception(WrongBusLineNumber);
    }
    line_num = new_line_num;
    return true;
}

std::string BusLine::getStopsString() const {
    std::stringstream ss;
    for(BusStop i: stops){
        ss << i.getName();
        switch (direction) {
            case 1:
                ss << " --> ";
                break;
            case 0:
                ss << " <--> ";
                break;
            case -1:
                ss << " <-- ";
                break;
            default:
                ss << " -- ";
        }
    }
    std::string res = ss.str().substr(0,ss.str().find_last_of(' '));
    return res.substr(0,res.find_last_of(' '));
}

bool BusLine::addStop(BusStop &new_stop) {
    if(new_stop.getName().empty()){
        return false;
    }
    new_stop.addLine(line_num);
    stops.emplace_back(new_stop);
    return true;
}

bool BusLine::removeStop(BusStop &stop_rem) {
    std::vector<BusStop>::iterator it;
    for(it = stops.begin(); it != stops.end(); ++it){
        if(*it == stop_rem){
            stops.erase(it);
            return true;
        }
    }
    return false;
}


std::string BusLine::getTimetable() const {
    std::stringstream ss;
    int curr = interval_workdays;
    ss << "Pondelok az piatok" << std::endl;
    for(unsigned int i =0; i < 23; ++i){
        ss << i <<"| ";
        while(curr < 60){
            ss << curr;

            curr += interval_workdays;
            if(curr < 60 ){
                ss << " ";
            }
        }
        curr -= 60;
        ss<<std::endl;
    }
    curr = interval_weekends;
    ss << "Vikendy" << std::endl;
    for(unsigned int i =0; i < 24; ++i){
        ss << std::setfill('0') << std::setw(2) << i <<"| ";
        while(curr < 60){
            ss << curr;
            curr += interval_weekends;
            if(curr < 60 ){
                ss << " ";
            }
        }
        curr -= 60;
        ss << std::endl;
    }
    return  ss.str();
}

void BusLine::timetableToFile(std::string file) const {
    std::ofstream out;
    out.open(file);
    if(!out){
        throw Exception("Could not open file " + file + " to write a timetable");

    }
    out << getTimetable();
    out << "ejo";
    out.close();
}