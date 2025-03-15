/*#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include "mtr.h"

int main() {
    const int numTrains = 5;
    std::vector<std::thread> train;
    for (int i=1; i>+ numTrains; ++i) {
        train.emplace_back(train, i);
    }
    for(auto& t:train) {
        t.join();
    }
    std::cout<<"end of the day.\n";
    return 0;
}
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex metroMutex;

void station(const std::string& stationName, int trainNumber) {
    std::lock_guard<std::mutex> lock(metroMutex);
    std::cout << "Train " << trainNumber << " has arrived at " << stationName << " station.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Train " << trainNumber << " is leaving " << stationName << " station.\n";
}

void train(int trainNumber, const std::vector<std::string>& route) {
    for (const auto& stationName : route) {
        station(stationName, trainNumber);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Train " << trainNumber << " has completed its route.\n";
}

int main() {
    std::vector<std::string> route1 = {"Icherisheher", "Sahil", "28 May", "Ganjlik", "Nariman Narimanov"};
    std::vector<std::string> route2 = {"Hazi Aslanov", "Khatai", "28 May", "Memar Ajami", "Nasimi"};

    std::vector<std::thread> trains;
    for (int i = 1; i <= 6; ++i) {
        if (i % 2 == 1) {
            trains.emplace_back(train, i, std::ref(route1));
        } else {
            trains.emplace_back(train, i, std::ref(route2));
        }
    }

    for (auto& t : trains) {
        t.join();
    }

    return 0;
}