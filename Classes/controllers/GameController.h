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

  

    using UndoAction = std::vector<SingleCardUndo>;  // һ�β��� = ���ſ���ԭλ�ü�¼
    std::vector<UndoAction> m_undoStack; // ���˼�¼ջ
    GameView* m_view;
    GameModel* m_model;

}; 
