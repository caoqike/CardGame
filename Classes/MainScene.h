#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__




#include "json/document.h"  // rapidjson头文件



class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainScene);

private:
    void initUI();
    void loadCardsFromJson();  // 从JSON加载卡牌
    void createCardFromJson(const rapidjson::Value& cardData, const std::string& zone);
    void arrangeStackCards();  // 整理堆牌区卡牌
    void onExchangeTopCard(PokerCard* card);
    void onUndo();
    std::vector<PokerCard*> m_deck;  // 所有卡牌
    std::vector<PokerCard*> m_playfieldCards;  // 主牌区卡牌
    std::vector<PokerCard*> m_stackCards;  // 堆牌区卡牌
    cocos2d::Node* m_cardLayer;
    struct SingleCardUndo {
        PokerCard* card;
        cocos2d::Vec2 originalPos;
        enum class Zone { STACK, PLAYFIELD } originalZone;
    };


    using UndoAction = std::vector<SingleCardUndo>;  // 一次操作 = 多张卡的原位置记录

    std::vector<UndoAction> m_undoStack; // 回退记录栈
    void recordCurrentStackPositions();
};
#endif