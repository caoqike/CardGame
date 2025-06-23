#include "UndoManager.h"
#include "../views/CardView.h"

UndoManager::UndoManager(UndoModel* model) : m_model(model) {}

void UndoManager::recordCurrentState(const std::vector<CardView*>& stackCards,
    const std::vector<CardView*>& playfieldCards) {
    UndoActionModel action;

    // 记录手牌状态
    for (auto card : stackCards) {
        SingleCardUndoModel undo;
        undo.card = CardModel(card->getModel().getSuit(), card->getModel().getFace());
        undo.originalPos = card->getPosition();
        undo.originalZone = SingleCardUndoModel::Zone::STACK;
        action.push_back(undo);
    }

    // 记录游戏区状态
    for (auto card : playfieldCards) {
        SingleCardUndoModel undo;
        undo.card = CardModel(card->getModel().getSuit(), card->getModel().getFace());
        undo.originalPos = card->getPosition();
        undo.originalZone = SingleCardUndoModel::Zone::PLAYFIELD;
        action.push_back(undo);
    }

    // 将当前状态添加到撤销栈
    m_model->undoStack.push_back(action);
}

void UndoManager::undo(GameView* view) {
    if (m_model->undoStack.empty()) {
        cocos2d::log("No undo actions available");
        return;
    }

    // 获取最后一次操作的状态
    UndoActionModel lastAction = m_model->undoStack.back();
    m_model->undoStack.pop_back();

    // 恢复状态
    for (const auto& undo : lastAction) {
        // 找到对应的卡牌视图
        CardView* card = nullptr;
        for (auto c : view->getAllCards()) {
            if (c->getModel().getSuit() == undo.card.getSuit() &&
                c->getModel().getFace() == undo.card.getFace()) {
                card = c;
                break;
            }
        }

        if (card) {
            // 恢复位置
            card->setPosition(undo.originalPos);

            // 如果需要，移动到正确的区域
            if (undo.originalZone == SingleCardUndoModel::Zone::STACK) {
                view->moveCardToStack(card);
            }
            else {
                view->moveCardToPlayfield(card);
            }
        }
    }
}