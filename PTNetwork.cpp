//
// Created by david on 19/12/22.
//
#include <fstream>
#include <sstream>
#include "Network.h"
using namespace std;

PTNetwork::~PTNetwork() {
    busstops.clear();
    buslines.clear();
    admin = admin_psk = "";
}

void PTNetwork::clearAll(){
    busstops.clear();
    buslines.clear();
}

BusStop& PTNetwork::getBusStopById(int id) {
    auto stop = busstops.find(id);
    if(stop == busstops.end()){
        throw Exception("No such stop ID "+ std::to_string(id) + " in network");
    }
    return busstops.find(id)->second;
}

BusLine& PTNetwork::getBusLineByNum(int linenum) {
    if(linenum < 0){
        throw Exception("Bus line id out of range");
    }
    auto line = buslines.find(linenum);
    if(line == buslines.end()){
        throw Exception("No such line with number "+ std::to_string(linenum) + " in network");
    }
    return line->second;
}

std::vector<std::tuple<Time,int, BusStop>> PTNetwork::getClosestFromStop(BusStop &stop, Time time1, Time interval, bool weekend){
    std::vector<std::tuple<Time,int, BusStop>> result;
    for(int line:stop.getCrossingLines()){
        if(!buslines[line].isLineInOrder()){
            continue;
        }
        BusStop dest = buslines[line].getLastStop();

        std::vector<std::pair<BusStop, Time>> cur = buslines[line].getEarliestFromStop(stop, dest, time1, weekend);
        if(cur[0].second <= time1 + interval) {
            result.emplace_back(std::make_tuple(cur[0].second, line, cur[cur.size() - 1].first));
        }
    }
    return result;
}

std::string PTNetwork::getClosestFromStopString(BusStop &stop, Time time1, Time interval, bool weekend){
    std::string result;
    for(std::tuple<Time,int, BusStop> i: getClosestFromStop(stop, time1, interval, weekend)){
        result += std::to_string(get<1>(i)) + ": o " + (get<0>(i) - time1).getTime() + " minut (odchod->" + get<0>(i).getTime() + "), smer: " + get<2>(i).getName() + "\n";
    }
    return result;
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
        auto stop = busstops.find(stopNo);
        if(stop  == busstops.end()){
            clearAll();
            throw Exception("Bus Stop number " + std::to_string(stopNo) + " not among list of bus stops");
        }

        lineb.addStop(-1,stop->second, 0,0);

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
