//
// Created by david on 19/12/22.
//
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include "Network.h"
using namespace std;

PTNetwork::~PTNetwork() {
    busstops.clear();
    buslines.clear();
    admin_psk = "";
}

bool PTNetwork::changePsk(std::string &old_psk, std::string &new_psk) {
    if(old_psk == admin_psk){
        admin_psk = new_psk;
        return true;
    }
    return false;
}

int PTNetwork::addStop(BusStop *new_stop) {
    /*
     * Adds a new stop to network - if the stop name is already in bus stop list then returns its number
     * if the stop number is not among stops vector then it just gets appended otherwise rewrites stops' number to last index + 1,appends it and returns its position
     */
    if(new_stop->getName().empty()){
        return -1;
    }
    for(std::pair<int, BusStop> i: busstops){
        if(i.second.getName() == new_stop->getName()){
            new_stop->changeStopNum(i.second.getStopNumber());
            return i.second.getStopNumber();
        }
    }
    if(busstops.find(new_stop->getStopNumber()) != busstops.end()){
        new_stop->changeStopNum((--busstops.end())->first +1);
        busstops[(--busstops.end())->first +1] = *new_stop;
    }
    busstops[new_stop->getStopNumber()] = *new_stop;
    return new_stop->getStopNumber();

}

bool PTNetwork::addLine(BusLine &new_line){
    /*
     * takes a reference to a line and adds it to the network - adds all its stops using addStop and then adds the new line to the list of lines for each stop
     * after that it adds the line if the line is not already among lines - otherwise throws an exception
     */
    for(auto it = new_line.getStopVector().begin(); it != new_line.getStopVector().end(); ++it){
        int which = addStop(it->first);
//        it->first.changeStopNum(which);
//        amended.addStop(-1, busstops[which], it->second,0);
        busstops[which].addLine(new_line.getLineNum());
    }
    if(buslines.find(new_line.getLineNum()) == buslines.end()){
        buslines[new_line.getLineNum()] = new_line;
        return true;
    }
    throw Exception("Bus line number " + to_string(new_line.getLineNum()) + " already exists with route: " + buslines[new_line.getLineNum()].getStopsString());

}

void PTNetwork::clearAll(){
    /*
     * clears busstops and buslines
     */
    busstops.clear();
    buslines.clear();
}

BusStop& PTNetwork::getBusStopById(int id) {
    /*
     * return a reference to a BusStop for given number - if not found throws an exception
     */
    auto stop = busstops.find(id);
    if(stop == busstops.end()){
        throw Exception("No such stop ID "+ std::to_string(id) + " in network");
    }
    return busstops.find(id)->second;
}

BusLine& PTNetwork::getBusLineByNum(int linenum) {
    /*
     * return a reference to a bus line by given number if not found throws an exception
     */
    if(linenum < 0){
        throw Exception("Bus line id out of range");
    }
    auto line = buslines.find(linenum);
    if(line == buslines.end()){
        throw Exception("No such line with number "+ std::to_string(linenum) + " in network");
    }
    return line->second;
}

bool compareTuple(std::tuple<Time, int, BusStop> &i1, std::tuple<Time, int, BusStop> &i2){
    /*
     * used to sort the closest connections from stop
     */
    return get<1>(i1) > get<1>(i2);
}

std::vector<std::tuple<Time,int, BusStop>> PTNetwork::getClosestFromStop(BusStop &stop, Time time1, Time interval, bool weekend){
    /*
     * for given bus stop and time returns a sorted vector (by shortest time) of tuples of:
     * Time - departure
     * int - number of Line
     * BusStop - ending stop/direction of the line
     *
     * for all lines that are in order
     */
    std::vector<std::tuple<Time,int, BusStop>> result;
    for(int line:stop.getCrossingLines()){
        if(!buslines[line].isLineInOrder()){
            continue;
        }
        BusStop* dest = buslines[line].getLastStop();

        std::vector<std::pair<BusStop, Time>> cur = buslines[line].getEarliestFromStop(stop, *dest, time1, weekend);
        if(cur[0].second <= time1 + interval) {
            result.emplace_back(std::make_tuple(cur[0].second, line, cur[cur.size() - 1].first));
        }
    }
    sort(result.begin(), result.end(), compareTuple);
    return result;
}

std::string PTNetwork::getClosestFromStopString(BusStop &stop, Time time1, Time interval, bool weekend){
    /*
     * returns a string represenation of the function getClosestFromStop
     */
    std::string result;
    for(std::tuple<Time,int, BusStop> i: getClosestFromStop(stop, time1, interval, weekend)){
        result += std::to_string(get<1>(i)) + ": o " + (get<0>(i) - time1).getTime() + " minut (odchod->" + get<0>(i).getTime() + "), smer: " + get<2>(i).getName() + "\n";
    }
    return result;
}


void PTNetwork::readStopsAndLines(const std::string& file){
    /* reads a file in following format:
     * <admin password>
     *
     * <StopNumber>;<StopName>
     * ...
     * empty line
     * <LineNumber>;<IntervalWorkdays>;<IntervalWeekends>;<IsInOrder>;<Direction(-1 <-, 0 <->, 1 ->)>
     * <StartStopNumber> -> <TimeToGet> -> <StopNumber> ... <EndStopNumber>
     * empty line
     * ...
    */
    std::ifstream ss(file);
    std::vector<BusLine> lines;
    std::string line;
    std::string psk;
    std::getline(ss, psk);
    admin_psk = psk;
    std::getline(ss, line);
    while (std::getline(ss,line) && !line.empty()){
        int StopNum;
        std::string stopName;
        StopNum = stoi(line.substr(0,line.find(';')));
        stopName = line.substr(line.find(';') + 1, line.size()-1);
        BusStop stop(stopName, StopNum);
        if(busstops.find(StopNum) == busstops.end()) {
            busstops[StopNum] = stop;
        }else{
            clearAll();
            throw Exception("Multiple Bus Stop number definition in file " + file + " on stop number " + std::to_string(StopNum));
        }
    }

    //Stops are now loaded, reading Bus lines
    while (std::getline(ss,line) && !line.empty()) {
        std::stringstream s2(line);
        int count;
        int info[6];
        count = 0;
        std::string word;

        while (std::getline(s2,word, ';')) { // Extract word from the stream.
            info[count]= stoi(word);
            ++count;
        }
        Type what_type;
        switch(info[5]){
            case 0:
                what_type = BUS;
                break;
            case 1:
                what_type = TRAM;
                break;
            case 2:
                what_type = METRO;
                break;
            default:
                throw Exception("Bus Line " + std::to_string(info[0]) + " must be within range 0-2 representing Bus, Tram, Metro respectively");
        }

        BusLine lineb(info[0],what_type, info[1], info[2], info[4]);
        if(lineb.getLineNum() == -1){
            clearAll();
            throw Exception("Bus Line parameters not formatted correctly");
        }
        if(info[3] == 1){
            lineb.changeStatus();
        }
        std::getline(ss, line);
        int stopNo;
        std::string time;
        stopNo = stoi(line.substr(0, line.find(' ')));
        if(busstops.find(stopNo)  == busstops.end()){
            clearAll();
            throw Exception("Bus Stop number " + std::to_string(stopNo) + " not among list of bus stops");
        }
        BusStop &stop = busstops.find(stopNo)->second;

        lineb.addStop(-1,stop, 0,0);

        std::string delimiter;
        line += " ->"; // to avoid additional exception handling
        std::stringstream s3(line.substr(line.find('>') + 2, line.size()-1));
        while(s3 >> time >> delimiter >> stopNo >> delimiter) { // Extract word from the stream.
            auto stop = busstops.find(stopNo);
            if(stop == busstops.end()){
                clearAll();
                throw Exception("Bus Stop number " + std::to_string(stopNo) + " not among list of bus stops");
            }
            Time time0(time);
            lineb.addStop(-1, stop->second, time0.getTimePair().first*60 + time0.getTimePair().second,0);
        }

        std::getline(ss, line);
        if(buslines.find(lineb.getLineNum()) == buslines.end()){
            buslines[lineb.getLineNum()] = lineb;
        }else{
            clearAll();
            throw Exception("Multiple Bus Line number definition in file " +file +" on Bus Line number "+ std::to_string(lineb.getLineNum()));
        }

    }
}

void PTNetwork::writeStopsAndLines(const std::string &file) {
    /*
     * writes the current network in following format
     * <admin password>
     *
     * <StopNumber>;<StopName>
     * ...
     * empty line
     * <LineNumber>;<IntervalWorkdays>;<IntervalWeekends>;<IsInOrder>;<Direction(-1 <-, 0 <->, 1 ->)>
     * <StartStopNumber> -> <TimeToGet> -> <StopNumber> ... <EndStopNumber>
     * empty line
     * ...
    */
    ofstream output;
    output.open(file);
    output << admin_psk << endl << endl;
    for(std::pair<int, BusStop> i:busstops){
        output << i.first << ";" << i.second.getName() << endl;
    }
    output << endl;
    for(std::pair<int, BusLine> line:buslines){
        output << line.second.getLineNum() << ";" << line.second.getIntervalWorkdays() << ";" << line.second.getIntervalWeekends() << ";" << line.second.isLineInOrder() << ";" << line.second.direction << ";" << line.second.getLineType() << endl;
        Time diff;
        output << line.second.getStopVector().begin()->first->getStopNumber();
        for(auto it = line.second.getStopVector().begin() + 1; it != line.second.getStopVector().end(); ++it){
            diff.setTime(0, it->second);
            output << " -> " <<diff.getTime()<< " -> " << it->first->getStopNumber() ;
        }
        output << endl << endl;
    }
}

std::string PTNetwork::getTimeTableForStop(BusStop &stop) {
    /*
     * return a timetable for all lines that drive through given stop in the same format as for an individual line
     * if line not in order then prints out that information
     */
    if(busstops.find(stop.getStopNumber()) == busstops.end()){
        throw Exception("Given bus stop " + stop.getName() + " not in this network");
    }
    std::stringstream result;
    Time time1;
    for(int line:stop.getCrossingLines()){
        BusLine current_line = buslines[line];
        BusStop* last = current_line.getLastStop();
        if(!current_line.isLineInOrder()){
            result << "============-" << current_line.getLineNum() << "-SMER-" << last->getName() << "-============" <<endl;
            result << "   " << current_line.getStopsString() << endl;
            result << "   Linka momentalne mimo prevadzky" << endl;
            continue;
        }
        Time shifted_by_stop = current_line.getEarliestFromStop(stop, *last, time1).begin()->second;//get the delay from the start stop to the given stop
        int mins = shifted_by_stop.getTimePair().second;
        int hours = shifted_by_stop.getTimePair().first;
        int interval = current_line.getIntervalWorkdays();
        result << "============-" << current_line.getLineNum() << "-SMER-" << last->getName() << "-============" <<endl;
        result << "   " << current_line.getStopsString() << endl;

        result << "   Pondelok az piatok:" << endl;
        while (hours < 24){
            result <<"   " << hours << "| ";
            while(mins < 60){
                result << mins << " ";
                mins += interval;
            }
            mins %=60;
            hours += 1;
            result << endl;
        }
        result << "   Vikendy:" << endl;
        mins = shifted_by_stop.getTimePair().second;
        hours = shifted_by_stop.getTimePair().first;
        interval = current_line.getIntervalWeekends();
        while (hours < 24){
            result << "   "<<hours << "| ";
            while(mins < 60){
                result << mins << " ";
                mins += interval;
            }
            mins %=60;
            hours += 1;
            result << endl;

        }
        result <<  endl;
    }
    return result.str();
}

void PTNetwork::writeTimeTableForStop(std::string filename, BusStop &stop) {
    /*
     * writes the output of getTimeTableForStop into the given file
     */
    std::ofstream file;
    file.open(filename);
    file << getTimeTableForStop(stop);
}