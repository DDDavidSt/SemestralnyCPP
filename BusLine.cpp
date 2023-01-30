//
// Created by david on 18/12/22.
//
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <fstream>
#include "BusLine.h"

BusLine::BusLine(int number, Type type1, int direct) {
    if(number <= 0){
        throw Exception(WrongBusLineNumber);
    }
    if(direct != 0 && direct != 1 && direct != -1){
        throw Exception("Unknown direction - use values -1,0,1 only!");
    }
    line_num = number;
    direction = direct;
    type = type1;
}

BusLine::BusLine(int number, Type type1, int workdays_interval, int weekends_interval, int direct){
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
    type = type1;
}


bool BusLine::changeDirection(int new_direct) {
    /*
     * reverses the stops vector if changed new_direct in {-1,1}
     */
    if(new_direct != -1 && new_direct != 0 && new_direct != 1){
        throw Exception(WrongDirection);
    }
    if(new_direct != direction && new_direct != 0){
        std::vector<std::pair<BusStop*, int>> stops_reversed;
        auto it = stops.end()-1;
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
    /*
     * switches the current status - to working only when correct variables are set (intervals, line number...)
     */
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
    /*
     * return a string representation of stops - if direction = 0 then uses <-|minutes|-> otherwise -|minutes|-> since the direction is given by the bus stops
     */
    std::stringstream ss;
    for(auto it = stops.begin(); it != stops.end(); it++){
        ss << it->first->getName();
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

bool BusLine::addStop(int position, BusStop &new_stop, int mins_from_prev, int mins_to_next) {
    /*
     * adds stop to given position - if position is 0 - adds to the beginning and uses mins_to_next (e.g. position=0, new_stop, 4,3; so Stop1 -3-> Stop2 changes to new_stop -3-> Stop1 -3-> Stop2)
     * if position is within <1, stops.size()) then it inserts it as expected (e.g. position 1, new_stop, 7,6; so Stop1 -3-> Stop2 changes to  Stop1 -7-> new_stop -6-> Stop2)
     * if position is -1 or stops.size() - appends to the end and uses mins_from_prev (e.g. position=-1, new_stop, 4,3; so Stop1 -3-> Stop2 changes to Stop1 -3-> Stop2 -4-> new_stop)
     */
    if(new_stop.getName().empty() || position < -1 || position > int(stops.size())
       || position == 0 && stops.size() != 0 &&mins_to_next <= 0 || position == stops.size()-1 && stops.size() != 0 && mins_from_prev <=0
       || position == -1 && stops.size() != 0 && mins_from_prev <=0){
        return false;
    }
    for(auto it = stops.begin(); it != stops.end(); ++it){ //check if not already in list
        if(it->first == &new_stop){
            return false;
        }
    }
    new_stop.addLine(line_num);
    if(position == 0) {
        if(stops.size() > 0){
            stops[0].second = mins_to_next;
        }
        stops.insert(stops.begin(), std::make_pair(&new_stop, 0));
        return true;
    }
    if(position == stops.size() || position == -1){
        stops.emplace_back(&new_stop, mins_from_prev);
        return true;
    }
    stops[position].second = mins_to_next;
    stops.insert(stops.begin() + position, std::make_pair(&new_stop, mins_from_prev));
    return true;
}

bool BusLine::removeStop(BusStop &stop_rem, int mins_prev_to_next) {
    /*
     * removes a stop and rewrites the minutes in following behaviour:
     * -if given mins_prev_to_next is less or equal to zero - just adds the two times and removes the stop (e.g. Stop1 -2-> stop_rem -3-> Stop2 changes to Stop1 -5-> Stop2)
     * -else replaces the gap with the given value (mins_prev_to_next = 1; e.g. Stop1 -2-> stop_rem -3-> Stop2 changes to Stop1 -1-> Stop2)
     * the following does not apply for start and end stop
     */
    std::vector<std::pair<BusStop*,int>>::iterator it;
    if(stops.empty()){
        return false;
    }
    for(it = stops.begin(); it != stops.end(); ++it){
        if(it->first == &stop_rem){
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
    /*
     * takes start and destination stops as input and return earliest (from given time) route on Line instance
     * throws exception if line out of order or stops not in the vector of stops
     */
    if(!status){
        throw Exception("Line " + std::to_string(line_num) + " currently not in order");
    }
    Time interval(interval_workdays/60, interval_workdays%60);
    if(weekend){
        interval.setTime(interval_weekends/60, interval_weekends%60);
    }
    Time fromOrigToStart;
    std::vector<std::pair<BusStop, Time>> result;
    if(start == dest){
        if(time.getTimePair().first == 23 && (fromOrigToStart + interval).getTimePair().first == 0 ){
            time.setTime(0,0);
        }
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
            if (it->first == &start) {
                break;
            }
        }
        if(it == stops.end()){
            throw Exception("Given start point is not in Line number " + std::to_string(line_num));
        }
        if(time.getTimePair().first == 23 && (fromOrigToStart + interval).getTimePair().first == 0 ){
            time.setTime(0,0);
        }
        while (fromOrigToStart < time) {
            fromOrigToStart += interval;
        }

        for (; it != stops.end() && (it-1)->first != &dest; ++it) {
            result.push_back(std::make_pair(*it->first, fromOrigToStart));
            Time delta((it + 1)->second / 60, (it + 1)->second % 60);
            fromOrigToStart += delta;
        }
        if(it == stops.end() && (it-1)->first != &dest){
            throw Exception("Given destination is not in Line number " + std::to_string(line_num));
        }
        return result;
    }
    auto it = stops.begin();
    bool before = false;
    for (; it != stops.end(); ++it) {
        Time delta(it->second / 60, it->second % 60);
        fromOrigToStart += delta;
        if(it->first == &dest){
            before = true;
            break;
        }
        if (it->first == &start) {
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
            if (it->first == &start) {
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
        for (; it != stops.end() && (it-1)->first != &dest; ++it) {
            result.push_back(std::make_pair(*it->first, fromOrigToStart));
            Time delta((it + 1)->second / 60, (it + 1)->second % 60);
            fromOrigToStart += delta;
        }
        changeDirection(0);
        if(it == stops.end() && (it-1)->first != &dest){
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

    for (; it != stops.end() and (it-1)->first != &dest; ++it) {
        result.push_back(std::make_pair(*it->first, fromOrigToStart));
        Time delta((it + 1)->second / 60, (it + 1)->second % 60);
        fromOrigToStart += delta;
    }
    if(it == stops.end() && (it-1)->first != &dest){
        throw Exception("Given destination is not in Line number " + std::to_string(line_num));
    }
    return result;
}

std::string BusLine::getEarliestFromStopString(BusStop &stop,BusStop &dest, Time &time, bool weekend) {
    /*
     * return a string representation of getEarliestFromStop
     */
    std::vector<std::pair<BusStop, Time>> route;
    route = getEarliestFromStop(stop, dest,time, weekend);
    std::stringstream  ss;
    if(route.begin() == route.end()-1){
        ss << "("<<route.begin()->second.getTime()<<")"<<route.begin()->first.getName() << " -00:00-> ";
    }else {
        for (auto it = route.begin(); it != route.end(); ++it) {
            ss << "(" << (it)->second.getTime() << ")" << it->first.getName() << " -"
               << ((it + 1)->second - (it)->second).getTime() << "-> ";
        }
    }
    return ss.str().substr(0,ss.str().size()-10);
}
std::string BusLine::getTimetable() const {
    /*
     * return a string - timetable for current line in following format:
     * Monday to Friday
     * <hour> | <minute> <minute> ...
     * ...
     * Weekends
     * <hour> | <minute> <minute> ...
     * ...
     */
    std::stringstream ss;
    int curr = 0;
    ss << "Pondelok az piatok" << std::endl;
    for(unsigned int i =0; i <= 23; ++i){
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
    curr = 0;
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

void BusLine::timetableToFile(const std::string& file) const {
    /*
     * writes timetable from getTimetable into given file
     */
    std::ofstream out;
    out.open(file);
    if(!out){
        throw Exception("Could not open file " + file + " to write a timetable");
    }
    out << getTimetable();
    out.close();
}


