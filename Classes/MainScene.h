#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__




#include "json/document.h"  // rapidjsonͷ�ļ�



class MainScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainScene);

private:
    void initUI();
    void loadCardsFromJson();  // ��JSON���ؿ���
    void createCardFromJson(const rapidjson::Value& cardData, const std::string& zone);
    void arrangeStackCards();  // �������������
    void onExchangeTopCard(PokerCard* card);
    void onUndo();
    std::vector<PokerCard*> m_deck;  // ���п���
    std::vector<PokerCard*> m_playfieldCards;  // ����������
    std::vector<PokerCard*> m_stackCards;  // ����������
    cocos2d::Node* m_cardLayer;
    struct SingleCardUndo {
        PokerCard* card;
        cocos2d::Vec2 originalPos;
        enum class Zone { STACK, PLAYFIELD } originalZone;
    };


    using UndoAction = std::vector<SingleCardUndo>;  // һ�β��� = ���ſ���ԭλ�ü�¼

    std::vector<UndoAction> m_undoStack; // ���˼�¼ջ
    void recordCurrentStackPositions();
};
#endif