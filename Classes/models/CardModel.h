#pragma once
#include "cocos2d.h"
#include"../PokerCard.h"
//enum class CardSuitType {
//    NONE = -1,
//    CLUBS,     // Ã·»¨
//    DIAMONDS,  // ·½¿é
//    HEARTS,    // ºìÌÒ
//    SPADES     // ºÚÌÒ
//};
//
//enum class CardFaceType {
//    NONE = -1,
//    ACE = 0,
//    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
//    JACK, QUEEN, KING
//};

class CardModel {
public:
    CardModel() = default;
    CardModel(CardSuitType suit, CardFaceType face)
        : _suit(suit), _face(face) {}
    CardModel(CardSuitType suit, CardFaceType face,float x, float y)
        : _suit(suit), _face(face), _x(x),_y(y){}

    // Getters
    CardSuitType getSuit() const { return _suit; }
    CardFaceType getFace() const { return _face; }
    float getX()const { return _x; }
    float getY()const { return _y; }
    bool isSelected() const { return _isSelected; }

    // Setters
    void setSelected(bool selected) { _isSelected = selected; }

private:

    CardSuitType _suit = CardSuitType::NONE;
    CardFaceType _face = CardFaceType::NONE;
    bool _isSelected = false;
    int _x;
    int _y;

};