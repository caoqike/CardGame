#include "UndoManager.h"
#include "../views/CardView.h"

UndoManager::UndoManager(UndoModel* model) : m_model(model) {}

void UndoManager::recordCurrentState(const std::vector<CardView*>& stackCards,
    const std::vector<CardView*>& playfieldCards) {
    UndoActionModel action;

    // ��¼����״̬
    for (auto card : stackCards) {
        SingleCardUndoModel undo;
        undo.card = CardModel(card->getModel().getSuit(), card->getModel().getFace());
        undo.originalPos = card->getPosition();
        undo.originalZone = SingleCardUndoModel::Zone::STACK;
        action.push_back(undo);
    }

    // ��¼��Ϸ��״̬
    for (auto card : playfieldCards) {
        SingleCardUndoModel undo;
        undo.card = CardModel(card->getModel().getSuit(), card->getModel().getFace());
        undo.originalPos = card->getPosition();
        undo.originalZone = SingleCardUndoModel::Zone::PLAYFIELD;
        action.push_back(undo);
    }

    // ����ǰ״̬��ӵ�����ջ
    m_model->undoStack.push_back(action);
}

void UndoManager::undo(GameView* view) {
    if (m_model->undoStack.empty()) {
        cocos2d::log("No undo actions available");
        return;
    }

    // ��ȡ���һ�β�����״̬
    UndoActionModel lastAction = m_model->undoStack.back();
    m_model->undoStack.pop_back();

    // �ָ�״̬
    for (const auto& undo : lastAction) {
        // �ҵ���Ӧ�Ŀ�����ͼ
        CardView* card = nullptr;
        for (auto c : view->getAllCards()) {
            if (c->getModel().getSuit() == undo.card.getSuit() &&
                c->getModel().getFace() == undo.card.getFace()) {
                card = c;
                break;
            }
        }

        if (card) {
            // �ָ�λ��
            card->setPosition(undo.originalPos);

            // �����Ҫ���ƶ�����ȷ������
            if (undo.originalZone == SingleCardUndoModel::Zone::STACK) {
                view->moveCardToStack(card);
            }
            else {
                view->moveCardToPlayfield(card);
            }
        }
    }
}