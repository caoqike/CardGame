#include "GameView.h"
#include <cocos/ui/UIButton.h>
#include "../configs/GameConfig.h"

#include "../controllers/GameController.h"
USING_NS_CC;

cocos2d::Scene* GameView::createScene() {
    return GameView::create();
}

bool GameView::init() {
    if (!Scene::init()) {
        return false;
    }

    // ������Ʒֱ���
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    glView->setDesignResolutionSize(GameConfig:: DESIGN_RESOLUTION.width, GameConfig::DESIGN_RESOLUTION.height, ResolutionPolicy::EXACT_FIT);

    initUI();
    return true;
}
void GameView::setController(GameController* controller) {
    m_controller = controller;
}
void GameView::initUI() {

    // ��������
    auto background = LayerColor::create(Color4B(0, 128, 0, 255)); // ��ɫ����
    this->addChild(background);

    // ��������ָ���
    auto separator = DrawNode::create();
    separator->drawLine(
        Vec2(0, GameConfig::PLAYFIELD_RECT.origin.y),
        Vec2(GameConfig::DESIGN_RESOLUTION.width, GameConfig::PLAYFIELD_RECT.origin.y),
        Color4F::WHITE
    );
    this->addChild(separator);

    // ��������ǩ
    auto playfieldLabel = Label::createWithTTF("Playfield (Main Area)", "fonts/Marker Felt.ttf", 24);
    playfieldLabel->setPosition(GameConfig::DESIGN_RESOLUTION.width / 2, GameConfig::PLAYFIELD_RECT.origin.y + GameConfig::PLAYFIELD_RECT.size.height - 30);
    this->addChild(playfieldLabel);

    auto stackLabel = Label::createWithTTF("Stack (Hand Area)", "fonts/Marker Felt.ttf", 24);
    stackLabel->setPosition(GameConfig::DESIGN_RESOLUTION.width / 2, GameConfig::STACK_RECT.size.height - 30);
    this->addChild(stackLabel);

    // �������Ʋ�
    m_cardLayer = Node::create();
    this->addChild(m_cardLayer);

    // ��ӱ���
    auto title = Label::createWithTTF("Poker Game", "fonts/Marker Felt.ttf", 64);
    title->setPosition(GameConfig::DESIGN_RESOLUTION.width / 2, GameConfig::DESIGN_RESOLUTION.height - 100);
    this->addChild(title);



    auto undoButton = ui::Button::create("back.png");  // ׼��һ�� undo ��ťͼƬ
    undoButton->setPosition(Vec2(GameConfig::DESIGN_RESOLUTION.width - 120, 80));  // ���½�
    undoButton->setScale(0.1f);
    undoButton->addClickEventListener([this](Ref* sender) {
        
        if (m_controller) {
            m_controller->onUndo(); // ���� GameController �еĳ����߼�
        }
        });
    this->addChild(undoButton, 10);
   
}

void GameView::addCardToPlayfield(CardView* card) {

    m_cardLayer->addChild(card);
    m_playfieldCards.push_back(card);
}

void GameView::addCardToStack(CardView* card) {
    m_cardLayer->addChild(card);
    m_stackCards.push_back(card);
}
//
void GameView::moveCardToPlayfield(CardView* card) {
    // ��stack�Ƴ�
    auto it = std::find(m_stackCards.begin(), m_stackCards.end(), card);
    if (it != m_stackCards.end()) {
        m_stackCards.erase(it);
    }

    // ��ӵ�playfield
    m_playfieldCards.push_back(card);

    // ����ZOrder
    card->setLocalZOrder(10);
}

void GameView::moveCardToStack(CardView* card) {
    // ��playfield�Ƴ�
    auto it = std::find(m_playfieldCards.begin(), m_playfieldCards.end(), card);
    if (it != m_playfieldCards.end()) {
        m_playfieldCards.erase(it);
    }

    // ��ӵ�stack
    m_stackCards.push_back(card);

    // ����ZOrder
    card->setLocalZOrder(5);
}
void GameView::onExit() {
    CCLOG("GameView::onExit");
    Scene::onExit();
}


std::vector<CardView*> GameView::getStackCards()  {

    cocos2d::log("in getStackCards");

    for (auto p : m_stackCards)
    {
        cocos2d::log("%p getStackCards",p);
    }
    cocos2d::log("in getStackCards return ");
    return m_stackCards;
}

std::vector<CardView*> GameView::getPlayfieldCards() const {
    return m_playfieldCards;
}

std::vector<CardView*> GameView::getAllCards() const {
    std::vector<CardView*> allCards;
    allCards.insert(allCards.end(), m_stackCards.begin(), m_stackCards.end());
    allCards.insert(allCards.end(), m_playfieldCards.begin(), m_playfieldCards.end());
    return allCards;
}