#include <sstream>
#include <bits/stdc++.h>
#include "Network.h"
using namespace std;

std::vector<std::tuple<int, int, int>> PTNetwork::whereNext(BusStop &from) {
    /*
     * returns a vector of tuples of adjacent stops(number), corresponding distance(mins) and connecting line(number) if stop in network (id and name)
     */
    if(busstops.find(from.getStopNumber()) == busstops.end() || busstops.find(from.getStopNumber())->second.getName() != from.getName()){
        throw Exception("Given stop " + from.getName() + " not in network");
    }
    BusLine *curr;
    std::vector<std::tuple<int, int, int>> result;
    for(int i:from.getCrossingLines()){
        curr = &buslines[i];
        if(curr->getStopVector().size() <= 1){
            continue;
        }
        std::vector<std::pair<BusStop *, int>> stops = curr->getStopVector();
        for(auto it = stops.begin(); it != stops.end(); ++it){
            if(it->first == &from){
                if(curr->direction <= 0){
                    if(it->first != curr->getFirstStop()){ //not in the beggining
                        auto prev = it -1;
                        result.emplace_back((prev)->first->getStopNumber(), it->second,curr->getLineNum());
                    }
                }if(curr->direction >= 0) {
                    if (it->first != curr->getLastStop()) {
                        auto next = it + 1;
                        result.emplace_back((next)->first->getStopNumber(), next->second,curr->getLineNum());
                    }
                }

            }
        }
    }
    return result;
}

std::map<int, std::vector<std::tuple<int, int, int>>> PTNetwork::createAdjMap(){
    /*
     * returns a map of adjacencies- key is stop number and value is tuple of adjacent bus stop(number), distance (mins) and connecting busline(number)
     */
    std::map<int, std::vector<std::tuple<int, int,int>>> result;
    for(auto &i:busstops){
        result[i.first] = whereNext(i.second);
    }
    return result;
}