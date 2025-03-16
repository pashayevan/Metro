#include "Station.h"
#include <iostream>
#include <thread>
#include <chrono>

Station::Station(const std::string& stationName) : name(stationName) {}

void Station::trainArrives(const std::string& trainID) {
    std::lock_guard<std::mutex> lock(stationMutex);
    std::cout << trainID << " прибыл на станцию " << name << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Время ожидания
}

void Station::trainDeparts(const std::string& trainID) {
    std::lock_guard<std::mutex> lock(stationMutex);
    std::cout << trainID << " покинул станцию " << name << std::endl;
}
