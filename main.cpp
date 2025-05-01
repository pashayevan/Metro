#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <string>
#include <fstream>

struct StationState {
    int forwardTrains = 0;
    int backwardTrains = 0;
};
std::unordered_map<std::string, StationState> stationStates;

std::ofstream purpleFile("purple.txt", std::ios::app);
std::ofstream yellowFile("yellow.txt", std::ios::app);
std::mutex fileMutex, consoleMutex, stationMutex, independentMutex;
std::condition_variable stationCv, independentCv;
int trainsInTunnel = 0, independentTrainsInTunnel = 0;
const int maxTrainsInTunnel = 2, maxIndependentTrains = 2;

void printMessage(const std::string& message, bool isPurple, bool isYellow, const std::string& direction) {
    std::string fullMessage = message + " [Direction: " + direction + "]";
    std::lock_guard<std::mutex> lock(fileMutex);
    (isPurple ? purpleFile : (isYellow ? yellowFile : std::cout)) << fullMessage << "\n";
    if (!isPurple && !isYellow) std::cout.flush();
}

void station(const std::string& trainId, const std::string& name, bool isForward, bool isPurple, bool isYellow, const std::string& direction) {
    std::unique_lock<std::mutex> lock(isPurple || isYellow ? independentMutex : stationMutex);
    auto& cv = (isPurple || isYellow) ? independentCv : stationCv;
    int& tunnelCount = (isPurple || isYellow) ? independentTrainsInTunnel : trainsInTunnel;
    const int maxTunnel = (isPurple || isYellow) ? maxIndependentTrains : maxTrainsInTunnel;

    cv.wait(lock, [&] {
        StationState& state = stationStates[name];
        return tunnelCount < maxTunnel && ((name == "Khojasan" || name == "IcheriSheher") ? state.forwardTrains + state.backwardTrains == 0 : (isForward ? state.forwardTrains == 0 : state.backwardTrains == 0));
    });

    tunnelCount++;
    StationState& state = stationStates[name];
    isForward ? state.forwardTrains++ : state.backwardTrains++;

    printMessage("Train " + trainId + " arrived at " + name + " (Trains in tunnel: " + std::to_string(tunnelCount) + ")", isPurple, isYellow, direction);
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    lock.lock();

    printMessage("Train " + trainId + " departed from " + name, isPurple, isYellow, direction);
    isForward ? state.forwardTrains-- : state.backwardTrains--;
    tunnelCount--;

    printMessage("Train " + trainId + " left tunnel (Trains in tunnel: " + std::to_string(tunnelCount) + ")", isPurple, isYellow, direction);
    lock.unlock();
    cv.notify_all();
}

#define DEFINE_STATION(name) \
    void name(const std::string& trainId, bool isForward, bool isPurple, const std::string& direction) { station(trainId, #name, isForward, isPurple, false, direction); } \
    void name##Yellow(const std::string& trainId, bool isForward, bool isPurple, const std::string& direction) { station(trainId, #name, isForward, isPurple, true, direction); }

DEFINE_STATION(HaziAslanov)
DEFINE_STATION(Ahmadli)
DEFINE_STATION(KhalglarDostlugu)
DEFINE_STATION(Neftchilar)
DEFINE_STATION(GaraGarayev)
DEFINE_STATION(Koroglu)
DEFINE_STATION(Ulduz)
DEFINE_STATION(Bakmil)
DEFINE_STATION(Narimanov)
DEFINE_STATION(Ganjlik)
DEFINE_STATION(May28)
DEFINE_STATION(Sahil)
DEFINE_STATION(IcheriSheher)
DEFINE_STATION(JafarJabbarly)
DEFINE_STATION(Nizami)
DEFINE_STATION(ElmlerAkademiyasi)
DEFINE_STATION(Inshaatchilar)
DEFINE_STATION(January20)
DEFINE_STATION(MemarAjami)
DEFINE_STATION(Nasimi)
DEFINE_STATION(AzadligProspekti)
DEFINE_STATION(Avtovagzal)
DEFINE_STATION(Khojasan)
DEFINE_STATION(Darnagul)
DEFINE_STATION(Noyabr8)
DEFINE_STATION(AvtoVogzal)
DEFINE_STATION(MemarAjami2)
DEFINE_STATION(KhojasanPurple)
DEFINE_STATION(Khatai)

int main() {
    if (!purpleFile.is_open() || !yellowFile.is_open()) {
        std::cerr << "Error: Could not open log files" << std::endl;
        return 1;
    }

    std::vector<std::thread> trains;
    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> redRoute = { Bakmil, Ulduz, Koroglu, GaraGarayev, Neftchilar, KhalglarDostlugu, Ahmadli, HaziAslanov };
    std::vector<std::function<void(const std::string&, bool, bool, const std::string&)>> greenRoute = { Bakmil, Narimanov, Ganjlik, May28, Nizami, ElmlerAkademiyasi, Inshaatchilar, January20, MemarAjami, Nasimi, AzadligProspekti, Avtovagzal, Khojasan, Darnagul };

    for (int i = 0; i < 4; i++) {
        trains.emplace_back([=] { for (int j = 0; j < 2; j++) { for (auto& station : redRoute) station("redLine" + std::to_string(i+1), true, false, "Icheri Sheher"); }});
        trains.emplace_back([=] { for (int j = 0; j < 2; j++) { for (auto& station : greenRoute) station("greenLine" + std::to_string(i+1), true, false, "Darnagul"); }});
    }

    for (auto& t : trains) t.join();
    std::cout << "All trains have returned to depots." << std::endl;
    return 0;
}
