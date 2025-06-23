#pragma once
#include "cocos2d.h"
#include "CardView.h"

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__
class GameController;
class GameView : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(GameView);

    void addCardToPlayfield(CardView* card);
    void addCardToStack(CardView* card);
    void moveCardToPlayfield(CardView* card);
    void moveCardToStack(CardView* card);
    void setController(GameController* controller); // 新增这一句
    std::vector<CardView*> getStackCards() ;
    std::vector<CardView*> getPlayfieldCards() const;
    std::vector<CardView*> getAllCards() const;

    virtual void onExit() override;
private:
    void initUI();
    cocos2d::Node* m_cardLayer;
    std::vector<CardView*> m_stackCards;
    std::vector<CardView*> m_playfieldCards;
    GameController* m_controller = nullptr; // 控制器指针


};
#endif