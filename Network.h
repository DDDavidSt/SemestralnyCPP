//
// Created by david on 19/12/22.
//

#include <map>
#include "BusLine.h"

class PTNetwork{
    std::map<int,BusStop> busstops;
    std::map<int, BusLine> buslines;
    std::string admin;
    std::string admin_psk;
public:
    PTNetwork(): admin(""), admin_psk(""){};
    PTNetwork(std::map<int,BusStop> stops, std::map<int, BusLine> lines) : busstops(std::move(stops)), buslines(std::move(lines)) {};
    void readStopsAndLines(const std::string& file);
    BusStop& getBusStopById(int id) ;
    int getNumberOfLines() const { return buslines.size();};
    int getNumberOfStops() const { return busstops.size();};
    std::vector<std::tuple<Time,int, BusStop>> getClosestFromStop(BusStop &stop, Time time1, Time interval = time_closest, bool weekend=false);
    std::string getClosestFromStopString(BusStop &stop, Time time1, Time interval = time_closest, bool weekend=false);
    BusLine& getBusLineByNum(int linenum);
    ~PTNetwork();
    void clearAll();
};
