#include "PokerCard.h"

const cocos2d::Size PokerCard::CARD_SIZE = cocos2d::Size(200, 250);

USING_NS_CC;

PokerCard* PokerCard::create(CardSuitType suit, CardFaceType face) {
    PokerCard* card = new (std::nothrow) PokerCard();
    if (card && card->init(suit, face)) {


        card->autorelease();
        return card;
    }



    CC_SAFE_DELETE(card);
    return nullptr;
}

bool PokerCard::init(CardSuitType suit, CardFaceType face) {
    if (!Node::init()) {
        return false;
    }

    _suit = suit;
    _face = face;

    setAnchorPoint(Vec2(0, 0));
    initUI();

    // 添加触摸事件监听器
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PokerCard::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(PokerCard::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void PokerCard::initUI() {
    // 创建卡牌背景
    auto cardBg = Sprite::create("card_general.png"); // 空白牌在Resource根目录
    cardBg->setName("cardBg");
    cardBg->setContentSize(CARD_SIZE);
    // 计算缩放因子
    //Size texSize = cardBg->getContentSize(); // 图片原始尺寸
    //cardBg->setScaleX(CARD_SIZE.width / texSize.width);
    //cardBg->setScaleY(CARD_SIZE.height / texSize.height);
    addChild(cardBg);

    // 添加右上角花色图标
    auto suitSprite = Sprite::create(getSuitResourceName());
  //  suitSprite->setPosition(30, cardBg->getContentSize().height - 30);
    suitSprite->setPosition(cardBg->getContentSize().width-150 ,
        cardBg->getContentSize().height - 150);
    suitSprite->setScale(0.4f);
    addChild(suitSprite);

    // 添加左上角小数字
    auto smallFaceSprite = Sprite::create(getSmallFaceResourceName());
    //smallFaceSprite->setPosition(cardBg->getContentSize().width - 30,
    //    cardBg->getContentSize().height - 30);
    smallFaceSprite->setPosition(cardBg->getContentSize().width - 270, cardBg->getContentSize().height - 150);
    smallFaceSprite->setColor(getSuitColor());
    smallFaceSprite->setScale(0.5f);
    addChild(smallFaceSprite);

    // 添加中间大数字
    auto bigFaceSprite = Sprite::create(getBigFaceResourceName());
    bigFaceSprite->setPosition(cardBg->getContentSize().width / 2-100, cardBg->getContentSize().height / 2 - 120);
    bigFaceSprite->setColor(getSuitColor());
    bigFaceSprite->setScale(0.8f);
    addChild(bigFaceSprite);

    this->setContentSize(CARD_SIZE);
}

std::string PokerCard::getSuitResourceName() const {
    switch (_suit) {
    case CardSuitType::CLUBS: return "suits/club.png";
    case CardSuitType::DIAMONDS: return "suits/diamond.png";
    case CardSuitType::HEARTS: return "suits/heart.png";
    case CardSuitType::SPADES: return "suits/spade.png";
    default: return "";
    }
}

std::string PokerCard::getSmallFaceResourceName() const {
    std::string color = (_suit == CardSuitType::HEARTS || _suit == CardSuitType::DIAMONDS)
        ? "red" : "black";

    switch (_face) {
    case CardFaceType::ACE: return "number/small_" + color + "_A.png";
    case CardFaceType::TWO: return "number/small_" + color + "_2.png";
    case CardFaceType::THREE: return "number/small_" + color + "_3.png";
    case CardFaceType::FOUR: return "number/small_" + color + "_4.png";
    case CardFaceType::FIVE: return "number/small_" + color + "_5.png";
    case CardFaceType::SIX: return "number/small_" + color + "_6.png";
    case CardFaceType::SEVEN: return "number/small_" + color + "_7.png";
    case CardFaceType::EIGHT: return "number/small_" + color + "_8.png";
    case CardFaceType::NINE: return "number/small_" + color + "_9.png";
    case CardFaceType::TEN: return "number/small_" + color + "_10.png";
    case CardFaceType::JACK: return "number/small_" + color + "_J.png";
    case CardFaceType::QUEEN: return "number/small_" + color + "_Q.png";
    case CardFaceType::KING: return "number/small_" + color + "_K.png";
   
    default: return "";
    }
}

std::string PokerCard::getBigFaceResourceName() const {
    std::string color = (_suit == CardSuitType::HEARTS || _suit == CardSuitType::DIAMONDS)
        ? "red" : "black";

    switch (_face) {
    case CardFaceType::ACE: return "number/big_" + color + "_A.png";
    case CardFaceType::TWO: return "number/big_" + color + "_2.png";
    case CardFaceType::THREE: return "number/big_" + color + "_3.png";
    case CardFaceType::FOUR: return "number/big_" + color + "_4.png";
    case CardFaceType::FIVE: return "number/big_" + color + "_5.png";
    case CardFaceType::SIX: return "number/big_" + color + "_6.png";
    case CardFaceType::SEVEN: return "number/big_" + color + "_7.png";
    case CardFaceType::EIGHT: return "number/big_" + color + "_8.png";
    case CardFaceType::NINE: return "number/big_" + color + "_9.png";
    case CardFaceType::TEN: return "number/big_" + color + "_10.png";
    case CardFaceType::JACK: return "number/big_" + color + "_J.png";
    case CardFaceType::QUEEN: return "number/big_" + color + "_Q.png";
    case CardFaceType::KING: return "number/big_" + color + "_K.png";
    default: return "";
    }
}

Color3B PokerCard::getSuitColor() const {
    return (_suit == CardSuitType::HEARTS || _suit == CardSuitType::DIAMONDS)
        ? Color3B::RED : Color3B::BLACK;
}

void PokerCard::setSelected(bool selected) {
    if (_isSelected != selected) {
        _isSelected = selected;


    }
}


//bool PokerCard::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
//    // 获取触摸点在卡片节点坐标系中的位置
//    auto pos = convertToNodeSpace(touch->getLocation());
//
//    // 获取卡片背景精灵，假设卡片背景精灵是第一个子节点
//     auto cardBg = getChildByName("cardBg");
//    if (!cardBg) {
//        return false;
//    }
//
//    // 获取卡片背景精灵的实际显示区域
//    auto rect = cardBg->getBoundingBox();
//    rect.origin = Vec2(0, 0);
//    if (rect.containsPoint(pos)) {
//        return true;
//    }
//
//    return false;
//}

//bool PokerCard::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
//    Vec2 localPos = convertToNodeSpace(touch->getLocation());
//
//    Rect bounds = Rect(0,0, getContentSize().width, getContentSize().height);
//
//    log("Touch localPos: (%.2f, %.2f)", localPos.x, localPos.y);
//    log("ContentSize: (%.2f, %.2f)", getContentSize().width, getContentSize().height);
//
//    return bounds.containsPoint(localPos);
//}
bool PokerCard::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    Vec2 localPos = convertToNodeSpace(touch->getLocation());
    

    Size size = getContentSize();

    Vec2 origin = Vec2(-size.width * getAnchorPoint().x,
        -size.height * getAnchorPoint().y);

    Rect bounds(origin.x, origin.y, size.width, size.height);

    if (bounds.containsPoint(localPos)) {
        log("Card % p: ,Touch localPos: (%.2f, %.2f)",this, localPos.x, localPos.y);
        log("Card % p: ,ContentSize: (%.2f, %.2f)", this,size.width, size.height);
        return true;  // 
    }

    return false;  // 
}

void PokerCard::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    // 获取触摸点在卡片节点坐标系中的位置
    auto pos = convertToNodeSpace(touch->getLocation());
    

    // 获取卡片背景精灵，假设卡片背景精灵是第一个子节点
    auto cardBg= getChildByName("cardBg");
    if (!cardBg) {
        return;
    }

    // 获取卡片背景精灵的实际显示区域
    auto rect = cardBg->getBoundingBox();
    rect.origin = Vec2(0, 0); // 调整为卡片节点坐标系

    if (rect.containsPoint(pos) && _callback) {
        _callback(this);
    }
}