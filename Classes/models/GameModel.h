#pragma once

#include <vector>
#include "CardModel.h"
//#include "UndoModel.h" 
//class UndoModel;

class GameModel {
public:
    std::vector<CardModel> playfieldCards;
    std::vector<CardModel> stackCards;
 //   UndoModel undoModel;
};
#pragma once
