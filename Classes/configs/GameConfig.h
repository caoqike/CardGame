#pragma once
#include "cocos2d.h"
#ifndef __GAME_CONFIG_H__
#define __GAME_CONFIG_H__
class GameConfig {
public:
    static const cocos2d::Size DESIGN_RESOLUTION;
    static const cocos2d::Rect PLAYFIELD_RECT;
    static const cocos2d::Rect STACK_RECT;
    static const cocos2d::Size CARD_SIZE;
};
#endif