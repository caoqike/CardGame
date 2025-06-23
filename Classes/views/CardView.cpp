

#include "CardView.h"
#include "configs/models/GameConfig.h"  // ���趨����CARD_SIZE
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
//    // ��ȡ�������ڿ�Ƭ�ڵ�����ϵ�е�λ��
//    auto pos = convertToNodeSpace(touch->getLocation());
//    
//
//    // ��ȡ��Ƭ�������飬���迨Ƭ���������ǵ�һ���ӽڵ�
//    auto cardBg= getChildByName("cardBg");
//    if (!cardBg) {
//        return;
//    }
//
//    // ��ȡ��Ƭ���������ʵ����ʾ����
//    auto rect = cardBg->getBoundingBox();
//    rect.origin = Vec2(0, 0); // ����Ϊ��Ƭ�ڵ�����ϵ
//
//    if (rect.containsPoint(pos) && _callback) {
//        _callback(this);
//    }
//}


bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    // ��鴥���Ƿ��ڿ�Ƭ������
    if (isTouchInside(touch)) {
        // ������ʼ����
        return true;
    }
    return false;
}

//bool CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
//    if (isTouchInside(touch)) {
//        // �����������ڿ�Ƭ�����ڣ����ûص�
//        if (_callback) {
//            _callback(this);
//        }
//    }
//    return true;
//}


bool CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (!isVisible()) { // ��������һ��isEnabled()����
        return false;
    }

    if (isTouchInside(touch)) {
        // ʹ�������û�������鿨���Ƿ���Ч
        if (_callback) {
            // �ڵ��ûص�ǰ�ٴ�ȷ������״̬
            if (getParent()) {
                _callback(this);
            }
        }
    }
    return true;
}
// �����������жϴ������Ƿ��ڿ�Ƭ������
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
    setContentSize(GameConfig::CARD_SIZE);  // ���������˿��Ƴߴ�

    initUI();
    // updateView();
   // setupTouchListener();


     // ��Ӵ����¼�������
     auto listener = EventListenerTouchOneByOne::create();
     listener->setSwallowTouches(true);
     listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
     listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
     _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void CardView::initUI() {
    // �������Ʊ���
    _cardBg = Sprite::create("card_general.png");
    _cardBg->setContentSize(GameConfig::CARD_SIZE);
    addChild(_cardBg);
    // this->_cardBg->addChild(_cardBg);

     // ��ӻ�ɫͼ��
    auto suitSprite = Sprite::create(getSuitResourceName());
    suitSprite->setPosition(getContentSize().width - 150, getContentSize().height - 150);
    suitSprite->setScale(0.4f);
    addChild(suitSprite);
    //this->_cardBg->addChild(suitSprite);

    // ���С����
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
    // ���ݿ���״̬������ͼ
    if (_model.isSelected()) {
        _cardBg->setColor(Color3B::YELLOW);  // ѡ��״̬
    }
    else {
        _cardBg->setColor(Color3B::WHITE);   // ����״̬
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
//            this->setScale(1.1f);  // ����Ч��
//            return true;
//        }
//        return false;
//    };
//
//    listener->onTouchEnded = [this](Touch* touch, Event* event) {
//        this->setScale(1.0f);  // �ָ�Ч��
//
//        if (this->getBoundingBox().containsPoint(touch->getLocation()) && _callback) {
//            _callback(this);  // �����ص�
//        }
//    };
//
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//}