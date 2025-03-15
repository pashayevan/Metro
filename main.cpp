#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex metroMutex;
std::mutex depotMutex;

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
    std::cout << "Train " << trainNumber << " has completed its route and is returning to the depot.\n";
}

void returnToDepot(int trainNumber) {
    std::lock_guard<std::mutex> lock(depotMutex);
    std::cout << "Train " << trainNumber << " has arrived at the depot.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Train " << trainNumber << " is now stationed in the depot.\n";
}

int main() {
    std::vector<std::string> redLine = {"Icherisheher", "Sahil", "28 May", "Ganjlik", "Nariman Narimanov"};
    std::vector<std::string> greenLine = {"Hazi Aslanov", "Khatai", "28 May", "Memar Ajami", "Nasimi"};
    std::vector<std::string> blueLine = {"Darnagul", "Azadlig Prospekti", "Nizami", "20 Yanvar", "Inshaatchilar"};

    std::vector<std::thread> trains;
    for (int i = 1; i <= 6; ++i) {
        if (i % 3 == 1) {
            trains.emplace_back(train, i, std::ref(redLine));
        } else if (i % 3 == 2) {
            trains.emplace_back(train, i, std::ref(greenLine));
        } else {
            trains.emplace_back(train, i, std::ref(blueLine));
        }
    }

    for (auto& t : trains) {
        t.join();
    }

    // Return all trains to depot
    trains.clear(); // Clear the vector to reuse it
    for (int i = 1; i <= 6; ++i) {
        trains.emplace_back(returnToDepot, i);
    }

    for (auto& t : trains) {
        t.join();
    }

    return 0;
}