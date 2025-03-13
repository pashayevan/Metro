/*#include <iostream>
#include <thread>
#include <vector>
#include <semaphore>
#include <chrono>

const
int train_count =6;
const int max_trains_on_track =2;
const int station_count =3;
const int station_stop_time = 2000;
const int travel_time = 300;
std::counting_semaphore<max_trains_on_track> track_semaphore(max_trains_on_track);

void sleep_for(int ms, const std::string& reason) {
    std::cout<<reason<<" (" <<ms<<" ";
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void train(int id) {
    int station = 0;
    while (true) {
        std::cout<<"train "<<id<<"on station"<< station + 1<<".'\n";
        sleep_for(station_stop_time, "waiting");

        track_semaphore.acquire();
        std::cout<<"train"<<id<<"left the station "<<station +1<<".\n";

        sleep_for(travel_time, "on way");

        station = (station + 1) % station_count;
        std::cout<<"train "<<id<< "arrived "<<station +1<<".\n";

        track_semaphore.release();
    }
}

int main() {
    std::vector<std::thread> trains;
    for(int i=1; i <= train_count; ++i) {
        trains.emplace_back(train, i);
    }
    for (auto& t : trains) {
        t.join();
    }
    return 0;
}*/