#include "Train.h"
#include <thread>
#include <chrono>

Train::Train(const std::string& id, const std::vector<Station*>& path)
        : trainName(id), trainRoute(path) {}

void Train::operator()() {
    for (auto* stop : trainRoute) {
        stop->trainArrives(trainName);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Время в пути
        stop->trainDeparts(trainName);
    }
}
