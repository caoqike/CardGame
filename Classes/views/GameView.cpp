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

    // 设置设计分辨率
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

    // 创建背景
    auto background = LayerColor::create(Color4B(0, 128, 0, 255)); // 绿色桌布
    this->addChild(background);

    // 绘制区域分隔线
    auto separator = DrawNode::create();
    separator->drawLine(
        Vec2(0, GameConfig::PLAYFIELD_RECT.origin.y),
        Vec2(GameConfig::DESIGN_RESOLUTION.width, GameConfig::PLAYFIELD_RECT.origin.y),
        Color4F::WHITE
    );
    this->addChild(separator);

    // 添加区域标签
    auto playfieldLabel = Label::createWithTTF("Playfield (Main Area)", "fonts/Marker Felt.ttf", 24);
    playfieldLabel->setPosition(GameConfig::DESIGN_RESOLUTION.width / 2, GameConfig::PLAYFIELD_RECT.origin.y + GameConfig::PLAYFIELD_RECT.size.height - 30);
    this->addChild(playfieldLabel);

    auto stackLabel = Label::createWithTTF("Stack (Hand Area)", "fonts/Marker Felt.ttf", 24);
    stackLabel->setPosition(GameConfig::DESIGN_RESOLUTION.width / 2, GameConfig::STACK_RECT.size.height - 30);
    this->addChild(stackLabel);

    // 创建卡牌层
    m_cardLayer = Node::create();
    this->addChild(m_cardLayer);

    // 添加标题
    auto title = Label::createWithTTF("Poker Game", "fonts/Marker Felt.ttf", 64);
    title->setPosition(GameConfig::DESIGN_RESOLUTION.width / 2, GameConfig::DESIGN_RESOLUTION.height - 100);
    this->addChild(title);



    auto undoButton = ui::Button::create("back.png");  // 准备一张 undo 按钮图片
    undoButton->setPosition(Vec2(GameConfig::DESIGN_RESOLUTION.width - 120, 80));  // 右下角
    undoButton->setScale(0.1f);
    undoButton->addClickEventListener([this](Ref* sender) {
        
        if (m_controller) {
            m_controller->onUndo(); // 调用 GameController 中的撤销逻辑
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
    // 从stack移除
    auto it = std::find(m_stackCards.begin(), m_stackCards.end(), card);
    if (it != m_stackCards.end()) {
        m_stackCards.erase(it);
    }

    // 添加到playfield
    m_playfieldCards.push_back(card);

    // 调整ZOrder
    card->setLocalZOrder(10);
}

void GameView::moveCardToStack(CardView* card) {
    // 从playfield移除
    auto it = std::find(m_playfieldCards.begin(), m_playfieldCards.end(), card);
    if (it != m_playfieldCards.end()) {
        m_playfieldCards.erase(it);
    }

    // 添加到stack
    m_stackCards.push_back(card);

    // 调整ZOrder
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