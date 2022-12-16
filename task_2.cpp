//
// Created by david on 14/12/22.
//
#include <iostream>
#include <algorithm>
#include <vector>
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
    if(new_direct != direction && new_direct != 0){
        std::vector<std::pair<BusStop, int>> stops_reversed;
        auto it = stops.end()-1;
        Time time;
        stops_reversed.push_back(std::make_pair(it->first, 0));
        --it;
        for(; it != stops.begin()-1; --it){
            stops_reversed.push_back(std::make_pair(it->first, (it+1)->second));
        }
        stops = std::move(stops_reversed);
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

std::string BusLine::getStopsString() const{
    std::stringstream ss;
    for(auto it = stops.begin(); it != stops.end(); it++){
        ss << it->first.getName();
        switch (direction) {
            case 0:
                ss << " <-"<< (it+1)->second << "-> ";
                break;
            default:
                ss << " -"<< (it+1)->second << "-> ";
                break;
        }
    }
    std::string res = ss.str().substr(0,ss.str().find_last_of(' '));
    return res.substr(0,res.find_last_of(' '));
}

bool BusLine::addStop(int position, BusStop &new_stop, int mins_from_prev, int mins_from_next) {
    if(new_stop.getName().empty() || position < -1 || position > int(stops.size())
        || position == 0 && stops.size() != 0 &&mins_from_next <= 0 || position == stops.size()-1 && stops.size() != 0 && mins_from_prev <=0
            || position == -1 && stops.size() != 0 && mins_from_prev <=0){
        return false;
    }
    for(auto it = stops.begin(); it != stops.end(); ++it){ //check if not already in list
        if(it->first == new_stop){
            return false;
        }
    }
    new_stop.addLine(line_num);
    if(position == 0) {
        if(stops.size() > 0){
            stops[0].second = mins_from_next;
        }
        stops.insert(stops.begin(), std::make_pair(new_stop, 0));
        return true;
    }
    if(position == stops.size()-1 || position == -1){
        stops.push_back(std::make_pair(new_stop, mins_from_prev));
        return true;
    }
    stops[position].second = mins_from_next;
    stops.insert(stops.begin() + position, std::make_pair(new_stop, mins_from_prev));
    return true;
}

bool BusLine::removeStop(BusStop &stop_rem, int mins_prev_to_next) {
    std::vector<std::pair<BusStop,int>>::iterator it;
    for(it = stops.begin(); it != stops.end(); ++it){
        if(it->first == stop_rem){
            break;
        }
    }
    if(it == stops.end()){
        return false;
    }
    if(it == stops.begin()){
        stops.erase(it);
        if(stops.size() != 0) {
            stops[0].second = 0;
        }
        return true;
    }
    if(it == stops.end()-1){
        stops.erase(it);
        return true;
    }
    if(mins_prev_to_next <= 0){
        mins_prev_to_next = (it)->second + (it+1)->second;
    }
    (it+1)->second = mins_prev_to_next;
    stops.erase(it);
    return true;
}

std::vector<std::pair<BusStop, Time>> BusLine::getEarliestFromStop(BusStop &start,BusStop &dest, Time &time, bool weekend){
    Time interval(interval_workdays/60, interval_workdays%60);
    if(weekend){
        interval.setTime(interval_weekends/60, interval_weekends%60);
    }
    Time fromOrigToStart;
    std::vector<std::pair<BusStop, Time>> result;
    if(start == dest){
        while (fromOrigToStart < time) {
            fromOrigToStart += interval;
        }
        result.push_back(std::make_pair(start,fromOrigToStart));
        return result;
    }
    if(direction != 0) {
        auto it = stops.begin();
        for (; it != stops.end(); ++it) {
            Time delta(it->second / 60, it->second % 60);
            fromOrigToStart += delta;
            if (it->first == start) {
                break;
            }
        }
        if(it == stops.end()){
            throw Exception("Given start point is not in Line number " + std::to_string(line_num));
        }
        while (fromOrigToStart < time) {
            fromOrigToStart += interval;
        }
        for (; it != stops.end() && (it-1)->first != dest; ++it) {
            result.push_back(std::make_pair(it->first, fromOrigToStart));
            Time delta((it + 1)->second / 60, (it + 1)->second % 60);
            fromOrigToStart += delta;
        }
        if(it == stops.end() && (it-1)->first != dest){
            throw Exception("Given destination is not in Line number " + std::to_string(line_num));
        }
        return result;
    }
    auto it = stops.begin();
    bool before = false;
    for (; it != stops.end(); ++it) {
        Time delta(it->second / 60, it->second % 60);
        fromOrigToStart += delta;
        if(it->first == dest){
            before = true;
            break;
        }
        if (it->first == start) {
            break;
        }
    }
    if(before){
        changeDirection(-1);
        Time fromOrigToStart;
        auto it = stops.begin();
        for (; it != stops.end(); ++it) {
            Time delta(it->second / 60, it->second % 60);
            fromOrigToStart += delta;
            if (it->first == start) {
                break;
            }
        }
        if(it == stops.end()){
            changeDirection(0);
            throw Exception("Given start point is not in Line number " + std::to_string(line_num));
        }
        while (fromOrigToStart < time) {
            fromOrigToStart += interval;
        }
        for (; it != stops.end() && (it-1)->first != dest; ++it) {
            result.push_back(std::make_pair(it->first, fromOrigToStart));
            Time delta((it + 1)->second / 60, (it + 1)->second % 60);
            fromOrigToStart += delta;
        }
        changeDirection(0);
        if(it == stops.end() && (it-1)->first != dest){
            throw Exception("Given destination is not in Line number " + std::to_string(line_num));
        }
        return result;

    }
    if(it == stops.end()){
        throw Exception("Given start point is not in Line number " + std::to_string(line_num));
    }
    while (fromOrigToStart < time) {
        fromOrigToStart += interval;
    }

    for (; it != stops.end() and (it-1)->first != dest; ++it) {
        result.push_back(std::make_pair(it->first, fromOrigToStart));
        Time delta((it + 1)->second / 60, (it + 1)->second % 60);
        fromOrigToStart += delta;
    }
    if(it == stops.end() && (it-1)->first != dest){
        throw Exception("Given destination is not in Line number " + std::to_string(line_num));
    }
    return result;
}

std::string BusLine::getEarliestFromStopString(BusStop &stop,BusStop &dest, Time &time, bool weekend) {
    std::vector<std::pair<BusStop, Time>> route;
    route = this->getEarliestFromStop(stop, dest,time, weekend);
    std::stringstream  ss;
    for(auto it = route.begin(); it != route.end(); ++it){
        ss << "("<<(it)->second.getTime()<<")"<<it->first.getName() << " -"<<((it+1)->second - (it)->second).getTime() << "-> ";
    }
    return ss.str().substr(0, ss.str().size()-10);
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