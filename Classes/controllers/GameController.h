#pragma once
#include "../models/GameModel.h"
#include "../views/GameView.h"


class GameController {
public:
    GameController(GameView* view, GameModel* model);
    ~GameController();

    void loadCardsFromFile(const std::string& filePath);


    void onTouchCard(CardView* card);
    void onUndo();
    void arrangeStackCards();
    void recordCurrentStackPositions();

    GameView* getView()
    {
        return m_view;
    }

private:

        struct SingleCardUndo {
        CardView* card;
        cocos2d::Vec2 originalPos;
        enum class Zone { STACK, PLAYFIELD } originalZone;
    };

  

    using UndoAction = std::vector<SingleCardUndo>;  // 一次操作 = 多张卡的原位置记录
    std::vector<UndoAction> m_undoStack; // 回退记录栈
    GameView* m_view;
    GameModel* m_model;

}; 
