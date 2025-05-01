#include "metro.h"
#include <chrono>
#include <thread>

Metro::Metro() {
    lines["Red"] = {
            {"Icheri Sheher", "Sahil", "28 May", "Ganjlik", "Nariman Narimanov",
             "Bakmil", "Ulduz", "Koroglu", "Qara Qaraev", "Neftchilar",
             "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
            "Bakmil"
    };

    lines["Green"] = {
            {"Darnagul", "Azadlig Prospekti", "Nasimi", "Memar Ajami", "20 January",
             "Inshaatchilar", "Elmlar Akademiyasy", "Nizami", "28 May",
             "Ganjlik", "Nariman Narimanov", "Bakmil", "Ulduz", "Koroglu",
             "Qara Qaraev", "Neftchilar", "Khalglar Dostlugu", "Ahmedli", "Azi Aslanov"},
            "Bakmil"
    };

    lines["Purple"] = {
            {"Khojasan", "Avtovagzal", "8 Noyabr"},
            "Khojasan"
    };

    lines["Yellow"] = {
            {"Jafar Jabbarly", "Hatai"},
            "N/A",
            true
    };
}

void Metro::safe_print(const std::string& message) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << message << std::endl;
}

void Metro::train(int train_id, const std::string& line_name, int direction) {
    static std::map<std::string, std::mutex> station_mutexes;
    static std::mutex init_mutex;

    const auto& line = lines[line_name];
    const auto& stations = line.stations;
    const std::string& depot = line.depot != "N/A" ? line.depot : stations[0];

    {
        std::lock_guard<std::mutex> lock(init_mutex);
        for (const auto& station : stations) {
            station_mutexes.try_emplace(station);
        }
    }

    // Для Красной линии
    if (line_name == "Red") {
        if (direction == 0) {
            int route[] = {5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5};
            for (int i = 0; i < sizeof(route)/sizeof(route[0]); ++i) {
                int station_idx = route[i];
                std::unique_lock<std::mutex> lock(station_mutexes[stations[station_idx]]);
                safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[station_idx]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        else {
            int route[] = {
                5, 4, 3, 2, 1, 0,
                1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12,
                11, 10, 9, 8, 7, 6, 4, 5
            };
            for (int i = 0; i < sizeof(route)/sizeof(route[0]); ++i) {
                int station_idx = route[i];
                std::unique_lock<std::mutex> lock(station_mutexes[stations[station_idx]]);
                safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[station_idx]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        safe_print("Train " + std::to_string(train_id) + " returned to depot");
        return;
    }

    // Для Зеленой линии
    if (line_name == "Green") {
        if (direction == 0) {
            // Поезд 3: Бакмиль -> Дярнягюль -> Бакмиль
            int route[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
            for (int i = 0; i < sizeof(route)/sizeof(route[0]); ++i) {
                int station_idx = route[i];
                std::unique_lock<std::mutex> lock(station_mutexes[stations[station_idx]]);
                safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[station_idx]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        else {
            // Поезд 4: Бакмиль -> Дярнягюль -> Улдуз -> Ази Асланов -> Дярнягюль
            int route[] = {
                11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, // Бакмиль -> Дярнягюль
                1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, // до Ази Асланов
                17, 16, 15, 14, 13, 12, 11 // обратно в Бакмиль
                };
            for (int i = 0; i < sizeof(route)/sizeof(route[0]); ++i) {
                int station_idx = route[i];
                std::unique_lock<std::mutex> lock(station_mutexes[stations[station_idx]]);
                safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[station_idx]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        safe_print("Train " + std::to_string(train_id) + " завершил маршрут");
        return;
    }

    // Для Фиолетовой линии (поезда 5 и 6)
    if (line_name == "Purple" && (train_id == 5 || train_id == 6)) {
        // Маршрут: Ходжасан -> 8 Ноябрь -> Ходжасан
        std::vector<int> route = {0, 1, 2, 1, 0}; // индексы станций
        for (int i = 0; i < route.size(); ++i) {
            int station_idx = route[i];
            {
                std::unique_lock<std::mutex> lock(station_mutexes[stations[station_idx]]);
                safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[station_idx]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        safe_print("Train " + std::to_string(train_id) + " завершил маршрут и остановился");
        return; // Завершаем поток поезда
    }

    // Для Желтой линии (поезд 7)
    if (line_name == "Yellow" && train_id == 7) {
        int circles = 2;
        for (int c = 0; c < circles; ++c) {
            // Прямо
            for (int i = 0; i < stations.size(); ++i) {
                std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[i]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            // Обратно
            for (int i = stations.size() - 2; i >= 0; --i) { // -2 чтобы не заезжать дважды на начальную станцию подряд
                std::unique_lock<std::mutex> lock(station_mutexes[stations[i]]);
                safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[i]);
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
        }
        safe_print("Train " + std::to_string(train_id) + " завершил 2 круга и остановился");
        return; // Завершаем поток поезда
    }


    // Для остальных линий (стандартная логика)
    int current = 0;
    for (int i = 0; i < stations.size(); ++i) {
        if (stations[i] == depot) {
            current = i;
            break;
        }
    }

    int step = direction == 0 ? 1 : -1;
    auto start_time = std::chrono::steady_clock::now();

    while (std::chrono::steady_clock::now() - start_time < std::chrono::minutes(1)) {
        std::unique_lock<std::mutex> lock(station_mutexes[stations[current]]);
        safe_print("Train " + std::to_string(train_id) + " arrived at " + stations[current]);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        lock.unlock();

        current += step;
        if (current < 0 || current >= stations.size()) {
            step = -step;
            current += step * 2;
        }
    }
}



// Новая вспомогательная функция для движения между станциями
void Metro::move_to_station(int train_id, const std::string& line_name,
                          std::map<std::string, std::mutex>& station_mutexes,
                          int& current, int target,
                          const std::vector<std::string>& stations) {
    int step = current < target ? 1 : -1;

    while (current != target) {
        current += step;
        if (current < 0 || current >= static_cast<int>(stations.size())) {
            safe_print("Error: Train " + std::to_string(train_id) + " out of bounds!");
            break;
        }

        const auto& current_station = stations[current];
        {
            std::unique_lock<std::mutex> lock(station_mutexes[current_station]);
            safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") arrived at " + current_station);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            safe_print("Train " + std::to_string(train_id) + " (" + line_name + ") departing from " + current_station);
        }
    }
}

void Metro::run() {
    std::vector<std::thread> threads;

    threads.emplace_back(&Metro::train, this, 1, "Red", 0);
    threads.emplace_back(&Metro::train, this, 2, "Red", 1);
    threads.emplace_back(&Metro::train, this, 3, "Green", 0);
    threads.emplace_back(&Metro::train, this, 4, "Green", 1);
    threads.emplace_back(&Metro::train, this, 5, "Purple", 0);
    threads.emplace_back(&Metro::train, this, 6, "Purple", 0);
    threads.emplace_back(&Metro::train, this, 7, "Yellow", 1);

    for (auto& t : threads) t.join();
}