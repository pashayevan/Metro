#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <vector>
#include "Station.h"

class Train {
public:
    Train(const std::string& id, const std::vector<Station*>& path);
    void operator()();

private:
    std::string trainName;
    std::vector<Station*> trainRoute;
};

#endif // TRAIN_H
