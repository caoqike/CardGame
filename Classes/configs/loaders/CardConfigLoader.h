#pragma once
#include "../../models/GameModel.h"

class CardConfigLoader {
public:
    static bool loadFromFile(const std::string& filePath, GameModel& model);
};