#ifndef STATION_H
#define STATION_H

#include <string>
#include <mutex>

class metro {
public:
    explicit metro(const std::string& stationName);
    void trainArrives(const std::string& trainID);
    void trainDeparts(const std::string& trainID);

private:
    std::string name;
    std::mutex stationMutex;
};

#endif // STATION_H
