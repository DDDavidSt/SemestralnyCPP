//
// Created by david on 19/12/22.
//

#include <map>
#include "BusLine.h"

const Time time_closest(0,20); //interval within which the soonest departures are shown for a Bus Stop (getClosestFromStop)


class PTNetwork{
    std::map<int,BusStop> busstops;
    std::map<int, BusLine> buslines;
    std::string admin_psk;
public:
    PTNetwork(): admin_psk(""){};
    PTNetwork(std::map<int,BusStop> stops, std::map<int, BusLine> lines, std::string admin_psk1="") : busstops(std::move(stops)), buslines(std::move(lines)), admin_psk(std::move(admin_psk1)) {};
    void readStopsAndLines(const std::string& file);
    void writeStopsAndLines(const std::string &file);
    int addStop(BusStop *new_stop);
    bool addLine(BusLine &new_line);
    bool changePsk(std::string &old_psk, std::string &new_psk);
    BusStop& getBusStopById(int id) ;
    int getNumberOfLines() const { return buslines.size();};
    int getNumberOfStops() const { return busstops.size();};
    std::vector<std::tuple<Time,int, BusStop>> getClosestFromStop(BusStop &stop, Time time1, Time interval = time_closest, bool weekend=false);
    std::string getClosestFromStopString(BusStop &stop, Time time1, Time interval = time_closest, bool weekend=false);
    BusLine& getBusLineByNum(int linenum);
    std::string getTimeTableForStop(BusStop &stop);
    void writeTimeTableForStop(std::string filename, BusStop &stop);
    std::vector<std::tuple<int, int,int>> whereNext(BusStop &from);
    std::map<int,  std::vector<std::tuple<int, int, int>>> createAdjMap();
    std::vector<std::pair<int,int>> findShortestPath(BusStop &start, BusStop &dest);
    std::string getRoute(BusStop &start, BusStop &end, Time when);
    ~PTNetwork();
    void clearAll();
};
