/*#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include "mtr.h"

std::mutex TrackMutex;

void train(int id) {
    std::cout << "train "<<id<<"arrive.\n";

    trackMutex.lock();
    std::cout<<"Trarin "<<id<<"ride.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout<<"train "<<id<<"left.\n";
    trackMutex.unlock();
}*/
