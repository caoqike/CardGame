#pragma once
#include "../models/GameModel.h"
#include <string>

class CardConfigLoader {
public:
    static bool loadFromFile(const std::string& filePath, GameModel& model);
};