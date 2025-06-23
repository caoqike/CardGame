#pragma once
#include "cocos2d.h"

enum class CardSuitType
{
    NONE = -1,
    CLUBS,      // Ã·»¨
    DIAMONDS,   // ·½¿é
    HEARTS,     // ºìÌÒ
    SPADES,     // ºÚÌÒ
    NUM_CARD_SUIT_TYPES
};

enum class CardFaceType
{
    NONE = -1,
    ACE ,    // A
    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING,        // J, Q, K
    NUM_CARD_FACE_TYPES
};




class PokerCard : public cocos2d::Node {
public:
    static const cocos2d::Size CARD_SIZE;

    static PokerCard* create(CardSuitType suit, CardFaceType face);
    virtual bool init(CardSuitType suit, CardFaceType face);

    CardSuitType getSuit() const { return _suit; }
    CardFaceType getFace() const { return _face; }
    bool isSelected() const { return _isSelected; }

    void setCallback(const std::function<void(PokerCard*)>& callback) { _callback = callback; }
    void setSelected(bool selected);

protected:
    PokerCard() {}
    virtual ~PokerCard() {}

private:
    CardSuitType _suit;
    CardFaceType _face;
    bool _isSelected = false;
    std::function<void(PokerCard*)> _callback;

    void initUI();
    std::string getSuitResourceName() const;
    std::string getSmallFaceResourceName() const;
    std::string getBigFaceResourceName() const;
    cocos2d::Color3B getSuitColor() const;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};   
