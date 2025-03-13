#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

const int train_count = 6;

class Station {
public:
    Station(std::string name) {
        this->name =name;
    }
    void arrive(const std::string& train) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout<<"train"<<train<<"arrived"<< name <<'\n';
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout<<"train"<<train<<"left";
    }
private:
    std ::mutex mtx;
    std::string name;
};

class Depo {
public:
    void enterDepo(const std::string& train) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout<<"train"<<train<<"arrived into the depo";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout<<"train"<<train<<"left the depo";
    }
private:
    std::mutex mtx;
};
void trainRide(std::string trainName,std::vector<Station>& route, Depo& depo) {
    depo.enterDepo(trainName);
    for(auto& station:route) {
        station.arrive(trainName);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    depo.enterDepo(trainName);
}

std::vector<std::vector<Station*>> createRoutes(std::vector<Station>& stations) {
    return {
    {&stations[0],&stations[1],&stations[2],&stations[3]},
    {&stations[3],&stations[2],&stations[1],&stations[0]},
    {&stations[1],&stations[4],&stations[5]},
    {&stations[5],&stations[4],&stations[1]},
    {&stations[2],&stations[3],&stations[4],&stations[5]},
    {&stations[5],&stations[4],&stations[3],&stations[2]}
    };
}

int main() {
    std::vector<Station> stations = {{"sahil"}, {"icheri"}, {"28 may"}, {"narimanov"}, {"ganjlik"}};

    Depo depo;
    std::vector<std::thread> trains;
    for(int i=0; i<train_count; ++i) {
        trains.emplace_back(trainRide, i+1, std::ref(routes[i]),std::ref(depo));
    }
    for(auto& t: trains) {
        t.join();
    }
    return 0;
}