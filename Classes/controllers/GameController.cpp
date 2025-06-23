#include "GameController.h"
#include "../configs/loaders/CardConfigLoader.h"
#include "../views/CardView.h"
#include <configs/models/GameConfig.h>


GameController::GameController(GameView* view, GameModel* model)
    : m_view(view), m_model(model) {


}

GameController::~GameController() {

}

void GameController::loadCardsFromFile(const std::string& filePath) {
    if (!CardConfigLoader::loadFromFile(filePath, *m_model)) {
        cocos2d::log("Failed to load card config from file: %s", filePath.c_str());
        return;
    }

    // 根据模型数据创建视图
    for (const auto& cardModel : m_model->playfieldCards) {
        auto card = CardView::create(CardModel(cardModel.getSuit(), cardModel.getFace()));
        card->setPosition(cardModel.getX(), cardModel.getY()+GameConfig::STACK_RECT.size.height);  // 屏幕中心（以960x640为例）


        
        //card->setCallback([this](CardView* card) {  // 显式指定参数类型为CardView*
        //    this->onExchangeTopCard(card);
        //    });

        card->setCallback(CC_CALLBACK_1(GameController::onTouchCard,this));
        m_view->addCardToPlayfield(card);
    }

    for (const auto& cardModel : m_model->stackCards) {
        auto card = CardView::create(CardModel(cardModel.getSuit(), cardModel.getFace()));
        //card->setPosition(480, 320);  // 
        //card->setAnchorPoint(cocos2d::Vec2(0, 0));  // 

        //card->setCallback([this](CardView* card) {  // 显式指定参数类型为CardView*
        //      this->onExchangeTopCard(card);
        //    });

        card->setCallback(CC_CALLBACK_1(GameController::onTouchCard, this));

        m_view->addCardToStack(card);
    }

    arrangeStackCards();
}

void GameController::arrangeStackCards() {
  //  m_undoManager->recordCurrentState(m_view->getStackCards(), m_view->getPlayfieldCards());


    
    const float paddingX = -50*(1+ m_model->stackCards.size() *0.2); // 卡牌间距
    const float cardWidth = GameConfig::CARD_SIZE.width;
    const float totalWidth = m_model->stackCards.size() * cardWidth + (m_model->stackCards.size() - 1) * paddingX;
    float startX = (GameConfig::STACK_RECT.size.width - totalWidth) / 2;
    float yPos = GameConfig::STACK_RECT.size.height / 2;

    auto stackCards = m_view->getStackCards();
    for (size_t i = 0; i < m_model->stackCards.size()-1; i++) {
        float x = startX + i * (cardWidth + paddingX);
        stackCards[i]->setAnchorPoint(cocos2d::Vec2(0, 0));

        //  m_stackCards[i]->runAction(MoveTo::create(0.5f, Vec2(PokerCard::CARD_SIZE.width / 2, PokerCard::CARD_SIZE.height / 2)));
        stackCards[i]->setPosition(cocos2d::Vec2(x, yPos));
        stackCards[i]->setLocalZOrder(static_cast<int>(i));
            //runAction(MoveTo::create(0.5f, Vec2(x, yPos)));
    }
    stackCards[stackCards.size() - 1]->setAnchorPoint(cocos2d::Vec2(0, 0));


    float endX = GameConfig::STACK_RECT.size.width - cardWidth;
    //  m_stackCards[i]->runAction(MoveTo::create(0.5f, Vec2(PokerCard::CARD_SIZE.width / 2, PokerCard::CARD_SIZE.height / 2)));
    stackCards[stackCards.size() - 1]->setPosition(cocos2d::Vec2(endX, yPos));

}

void GameController::recordCurrentStackPositions() {
    UndoAction action;
    auto  m_stackCards = m_view->getStackCards();
    for (auto* card : m_stackCards) {
        action.push_back({ card, card->getPosition(), SingleCardUndo::Zone::STACK });
    }

    auto  m_playfieldCards = m_view->getPlayfieldCards();
    for (auto* card : m_playfieldCards) {
        action.push_back({ card, card->getPosition(), SingleCardUndo::Zone::PLAYFIELD });
    }

    m_undoStack.push_back(action);
}

void GameController::onTouchCard(CardView* card) {
    if (!card) {
        cocos2d::log("Error: Card pointer is null!");
        return;
    }

    if (!m_view) {
        cocos2d::log("Error: m_view is null!");
        return;
    }

        card->setSelected(!card->_isSelected);


        auto  m_stackCards = m_view->getStackCards();

       cocos2d::log("size=%d\n", m_stackCards.size());

    auto it = std::find(m_stackCards.begin(), m_stackCards.end(), card);
    if (it != m_stackCards.end()) {
        // 点击的是手牌区卡牌，且不是顶牌
        if (m_stackCards.size() > 1 && card != m_stackCards.back()) {
            auto itCard = it;
            auto itTop = m_stackCards.end() - 1;
            recordCurrentStackPositions();
            // 交换堆牌顺序
            std::iter_swap(itCard, itTop);

            
            // 重新排列所有手牌
            arrangeStackCards();
        }
    }

    auto  m_playfieldCards = m_view->getPlayfieldCards();
        auto itPlay = std::find(m_playfieldCards.begin(), m_playfieldCards.end(), card);
    if (itPlay != m_playfieldCards.end()) {
        if (m_stackCards.empty()) return;

        CardView* topCard = m_stackCards.back();
        int topFace = static_cast<int>(topCard->getModel().getFace());
        int selectedFace = static_cast<int>(card-> getModel().getFace());

        // 判断是否可匹配（点数差1）
        if (std::abs(topFace - selectedFace) == 1) {
            // 获取目标位置
            cocos2d::Vec2 topPos = topCard->getPosition();


            recordCurrentStackPositions();
            card->setPosition(topPos);

            // 替换顶牌
            m_stackCards.push_back(*itPlay);

            // 从桌面牌区移除
            m_playfieldCards.erase(itPlay);

            // 重新排列手牌
            arrangeStackCards();



        }
        else {
            cocos2d::log("点数不匹配，无法替换顶牌");
        }
    }
}

void  GameController::onUndo() {


    UndoAction action = m_undoStack.back();
    m_undoStack.pop_back();

    for (const auto& entry : action) {
        // 恢复位置
        entry.card->stopAllActions();
        entry.card->setPosition(entry.originalPos);


        auto  m_stackCards = m_view->getStackCards();
        // 从两个集合中移除这张牌（避免重复）
        auto inStack = std::find(m_stackCards.begin(), m_stackCards.end(), entry.card);
        if (inStack != m_stackCards.end()) m_stackCards.erase(inStack);


        auto  m_playfieldCards = m_view->getPlayfieldCards();
        auto inPlay = std::find(m_playfieldCards.begin(), m_playfieldCards.end(), entry.card);
        if (inPlay != m_playfieldCards.end()) m_playfieldCards.erase(inPlay);

        // 放回原来的集合
        if (entry.originalZone == SingleCardUndo::Zone::STACK) {
            m_stackCards.push_back(entry.card);
        }
        else {
            m_playfieldCards.push_back(entry.card);
        }
    }

    // 重新整理堆牌
   // arrangeStackCards();

}