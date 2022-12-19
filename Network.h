//
// Created by david on 19/12/22.
//

#include "BusLine.h"

class PTNetwork{
    std::vector<BusStop> busstops;
    std::vector<BusLine> buslines;
    std::string admin;
    std::string admin_psk;
public:
    PTNetwork(): admin(""), admin_psk(""){};
    PTNetwork(std::vector<BusStop> stops, std::vector<BusLine> lines) : busstops(std::move(stops)), buslines(std::move(lines)) {};
    void readStopsAndLines(const std::string& file);
    BusStop getBusStopById(int id) const;
    int getNumberOfLines() const { return buslines.size();};
    int getNumberOfStops() const { return busstops.size();};
    BusLine getBusLineByNum(int linenum) const;
};
