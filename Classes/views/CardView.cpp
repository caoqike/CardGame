

#include "CardView.h"
#include "configs/models/GameConfig.h"  // 假设定义了CARD_SIZE
#include <PokerCard.h>



USING_NS_CC;

CardView* CardView::create(const CardModel& model) {
    CardView* card = new (std::nothrow) CardView();
    if (card && card->init(model)) {
        card->autorelease();

        log("CardView init succeed");
        return card;
       
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}
//bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
//    Vec2 localPos = convertToNodeSpace(touch->getLocation());
//    
//
//    Size size = getContentSize();
//
//    Vec2 origin = Vec2(-size.width * getAnchorPoint().x,
//        -size.height * getAnchorPoint().y);
//
//    Rect bounds(origin.x, origin.y, size.width, size.height);
//
//    if (bounds.containsPoint(localPos)) {
//        log("Card % p: ,Touch localPos: (%.2f, %.2f)",this, localPos.x, localPos.y);
//        log("Card % p: ,ContentSize: (%.2f, %.2f)", this,size.width, size.height);
//        return true;  // 
//    }
//
//    return false;  // 
//}
//
//void CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
//    // 获取触摸点在卡片节点坐标系中的位置
//    auto pos = convertToNodeSpace(touch->getLocation());
//    
//
//    // 获取卡片背景精灵，假设卡片背景精灵是第一个子节点
//    auto cardBg= getChildByName("cardBg");
//    if (!cardBg) {
//        return;
//    }
//
//    // 获取卡片背景精灵的实际显示区域
//    auto rect = cardBg->getBoundingBox();
//    rect.origin = Vec2(0, 0); // 调整为卡片节点坐标系
//
//    if (rect.containsPoint(pos) && _callback) {
//        _callback(this);
//    }
//}


bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    // 检查触摸是否在卡片区域内
    if (isTouchInside(touch)) {
        // 触摸开始处理
        return true;
    }
    return false;
}

//bool CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
//    if (isTouchInside(touch)) {
//        // 触摸结束且在卡片区域内，调用回调
//        if (_callback) {
//            _callback(this);
//        }
//    }
//    return true;
//}


bool CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!isVisible()) { // 假设你有一个isEnabled()方法
        return false;
    }

    if (isTouchInside(touch)) {
        // 使用弱引用或标记来检查卡牌是否有效
        if (_callback) {
            // 在调用回调前再次确认自身状态
            if (getParent()) {
                _callback(this);
            }
        }
    }
    return true;
}
// 辅助方法：判断触摸点是否在卡片区域内
bool CardView::isTouchInside(cocos2d::Touch* touch) {
    auto touchLocation = convertToNodeSpace(touch->getLocation());
    auto contentSize = getContentSize();
    auto rect = cocos2d::Rect(0, 0, contentSize.width, contentSize.height);
    return rect.containsPoint(touchLocation);
}
bool CardView::init(const CardModel& model) {
    if (!Node::init()) {
        return false;
    }

    _model = model;
    setContentSize(GameConfig::CARD_SIZE);  // 假设配置了卡牌尺寸

    initUI();
    // updateView();
   // setupTouchListener();


     // 添加触摸事件监听器
     auto listener = EventListenerTouchOneByOne::create();
     listener->setSwallowTouches(true);
     listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
     listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
     _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::initUI() {
    // 创建卡牌背景
    _cardBg = Sprite::create("card_general.png");
    _cardBg->setContentSize(GameConfig::CARD_SIZE);
    addChild(_cardBg);
    // this->_cardBg->addChild(_cardBg);

     // 添加花色图标
    auto suitSprite = Sprite::create(getSuitResourceName());
    suitSprite->setPosition(getContentSize().width - 150, getContentSize().height - 150);
    suitSprite->setScale(0.4f);
    addChild(suitSprite);
    //this->_cardBg->addChild(suitSprite);

    // 添加小数字
    auto smallFaceSprite = Sprite::create(getSmallFaceResourceName());
    //log();
    smallFaceSprite->setPosition(getContentSize().width - 270, getContentSize().height - 150);
    smallFaceSprite->setColor(getSuitColor());
    smallFaceSprite->setScale(0.3f);
    //this->_cardBg->addChild(smallFaceSprite);
    addChild(smallFaceSprite);

    auto bigFaceSprite = Sprite::create(getBigFaceResourceName());
    log("111111");
    bigFaceSprite->setPosition(getContentSize().width / 2 - 100, getContentSize().height / 2 - 120);
    bigFaceSprite->setColor(getSuitColor());
    bigFaceSprite->setScale(0.8f);
    addChild(bigFaceSprite);

    this->setContentSize(GameConfig::CARD_SIZE);
    //this->_cardBg->addChild(bigFaceSprite);
}

void CardView::updateView() {
    // 根据卡牌状态更新视图
    if (_model.isSelected()) {
        _cardBg->setColor(Color3B::YELLOW);  // 选中状态
    }
    else {
        _cardBg->setColor(Color3B::WHITE);   // 正常状态
    }
}

void CardView::setSelected(bool selected) {
    this->_isSelected = selected;
    _model.setSelected(selected);
    updateView();
}

Color3B CardView::getSuitColor() const {
    return (_model.getSuit() == CardSuitType::HEARTS ||
        _model.getSuit() == CardSuitType::DIAMONDS)
        ? Color3B::RED : Color3B::BLACK;
}

std::string CardView::getSuitResourceName() const {
    log("getSuitResourceName");
    switch (_model.getSuit()) {
    case CardSuitType::CLUBS:log("CLUBS"); return "suits/club.png";
    case CardSuitType::DIAMONDS:log("DIAMONDS"); return "suits/diamond.png";
    case CardSuitType::HEARTS: log("HEARTS"); return "suits/heart.png";
    case CardSuitType::SPADES:log("SPADES"); return "suits/spade.png";
    default: return "";
    }
}


std::string CardView::getBigFaceResourceName() const {
    std::string color = (_model.getSuit() == CardSuitType::HEARTS ||
        _model.getSuit() == CardSuitType::DIAMONDS)
        ? "red" : "black";

    switch (_model.getFace()) {
    case CardFaceType::ACE: log("ACE"); return "number/big_" + color + "_A.png";
    case CardFaceType::TWO: log("2"); return "number/big_" + color + "_2.png";
    case CardFaceType::THREE:log("3"); return "number/big_" + color + "_3.png";
    case CardFaceType::FOUR:log("4"); return "number/big_" + color + "_4.png";
    case CardFaceType::FIVE: log("5"); return "number/big_" + color + "_5.png";
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


std::string CardView::getSmallFaceResourceName() const {
    std::string color = (_model.getSuit() == CardSuitType::HEARTS ||
        _model.getSuit() == CardSuitType::DIAMONDS)
        ? "red" : "black";

    switch (_model.getFace()) {
    case CardFaceType::ACE: log("ACE"); return "number/small_" + color + "_A.png";
    case CardFaceType::TWO: log("2"); return "number/small_" + color + "_2.png";
    case CardFaceType::THREE:log("3"); return "number/small_" + color + "_3.png";
    case CardFaceType::FOUR:log("4"); return "number/small_" + color + "_4.png";
    case CardFaceType::FIVE: log("5"); return "number/small_" + color + "_5.png";
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
//void CardView::setupTouchListener() {
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = [this](Touch* touch, Event* event) {
//        if (this->getBoundingBox().containsPoint(touch->getLocation())) {
//            this->setScale(1.1f);  // 按下效果
//            return true;
//        }
//        return false;
//    };
//
//    listener->onTouchEnded = [this](Touch* touch, Event* event) {
//        this->setScale(1.0f);  // 恢复效果
//
//        if (this->getBoundingBox().containsPoint(touch->getLocation()) && _callback) {
//            _callback(this);  // 触发回调
//        }
//    };
//
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//}