//
// Created by david on 19/12/22.
//
#include <fstream>
#include <sstream>
#include "Network.h"

BusStop PTNetwork::getBusStopById(int id) const {
    if(id <0){
        throw Exception("Bus Stop id out of range");
    }
    for(BusStop i: busstops){
        if(i.getStopNumber() == id){
            return i;
        }
    }
    BusStop b;
    return b;
}

BusLine PTNetwork::getBusLineByNum(int linenum) const {
    for(BusLine i: buslines){
        if(i.getLineNum() == linenum){
            return i;
        }
    }
    BusLine b(0,0);
    return b;
}

void PTNetwork::readStopsAndLines(const std::string& file){
    /* reads a file in following format:
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
    while (std::getline(ss,line) && !line.empty()){
        int StopNum;
        std::string stopName;
        StopNum = stoi(line.substr(0,line.find(';')));
        stopName = line.substr(line.find(';') + 1, line.size()-1);
        BusStop stop(stopName, StopNum);
        busstops.push_back(stop);
    }
    //Stops are now loaded, reading Bus lines
    while (std::getline(ss,line) && !line.empty()) {
        std::stringstream s2(line);
        int count;
        int info[5];
        count = 0;
        std::string word;

        while (std::getline(s2,word, ';')) { // Extract word from the stream.
            info[count]= stoi(word);
            ++count;
        }

        BusLine lineb(info[0], info[1], info[2], info[4]);
        if(lineb.getLineNum() == -1){
            throw Exception("Bus Line parameters not formatted correctly");
        }
        if(info[3] == 1){
            lineb.changeStatus();
        }
        std::getline(ss, line);
        int stopNo;
        std::string time;
        stopNo = stoi(line.substr(0, line.find(' ')));
        BusStop stop;
        try {
            stop = std::move(getBusStopById((stopNo)));
        }catch (Exception &e){
            ;
        }
        if(stop.getName().empty()){
            throw Exception("Bus Stop number " + std::to_string(stopNo) + " not among list of bus stops");
        }
        lineb.addStop(-1,stop, 0,0);

        std::string delimiter;
        std::stringstream s3(line.substr(line.find('>') + 2, line.size()-1));

        while(s3 >> time >> delimiter >> stopNo >> delimiter) { // Extract word from the stream.
            stop = std::move(getBusStopById((stopNo)));
            if(stop.getName().empty()){
                throw Exception("Bus Stop number " + std::to_string(stopNo) + " not among list of bus stops");
            }
            Time time0(time);
            lineb.addStop(-1, stop, time0.getTimePair().first*60 + time0.getTimePair().second,0);
        }

        std::getline(ss, line);
        buslines.push_back(lineb);
    }
}
