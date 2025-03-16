#include <iostream>
#include <vector>
#include <thread>
#include "Station.h"
#include "Train.h"

int main() {
    // Создание станций для красной линии
    std::vector<Station> redLineStations = {
        Station("Icherisheher"),
        Station("Sahil"),
        Station("28 May"),
        Station("Gara Garayev"),
        Station("Neftchilar"),
        Station("Khatai"),
        Station("Hazi Aslanov")
    };

    // Создание станций для зелёной линии
    std::vector<Station> greenLineStations = {
        Station("28 May"),
        Station("Jafar Jabbarly"),
        Station("Ganjlik"),
        Station("Nariman Narimanov"),
        Station("Bakmil"),
        Station("Ulduz"),
        Station("Koroglu"),
        Station("Khalglar Dostlugu"),
        Station("Ahmadli"),
        Station("Hazi Aslanov")
    };

    // Создание станций для фиолетовой линии
    std::vector<Station> purpleLineStations = {
        Station("Memar Ajami 2"),
        Station("Avtovagzal"),
        Station("8 Noyabr"),
        Station("Khojasan")
    };

    // Функция для создания маршрута туда и обратно
    auto createRoute = [](std::vector<Station>& stations) {
        std::vector<Station*> route;
        for (auto& station : stations) route.push_back(&station);
        for (int i = stations.size() - 2; i >= 0; --i) route.push_back(&stations[i]);
        return route;
    };

    // Создание маршрутов для каждой линии
    auto redLineRoute = createRoute(redLineStations);
    auto greenLineRoute = createRoute(greenLineStations);
    auto purpleLineRoute = createRoute(purpleLineStations);

    // Создание поездов для каждой линии
    std::vector<Train> trains;
    for (int i = 1; i <= 2; ++i) {
        trains.emplace_back("Red Line Train " + std::to_string(i), redLineRoute);
        trains.emplace_back("Green Line Train " + std::to_string(i), greenLineRoute);
        trains.emplace_back("Purple Line Train " + std::to_string(i), purpleLineRoute);
    }

    // Запуск поездов в отдельных потоках
    std::vector<std::thread> threads;
    for (auto& train : trains) {
        threads.emplace_back(std::ref(train));
    }

    // Ожидание завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
