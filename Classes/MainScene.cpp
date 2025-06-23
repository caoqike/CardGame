#include "MainScene.h"
#include <cocos/ui/UIButton.h>

// ����ߴ綨��
const cocos2d::Size DESIGN_RESOLUTION = cocos2d::Size(1080, 2080);
const cocos2d::Rect PLAYFIELD_RECT = cocos2d::Rect(0, 580, 1080, 1500); // ������
const cocos2d::Rect STACK_RECT = cocos2d::Rect(0, 0, 1080, 580); // ������

USING_NS_CC;

Scene* MainScene::createScene() {
    return MainScene::create();
}

bool MainScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ������Ʒֱ���
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    glView->setDesignResolutionSize(DESIGN_RESOLUTION.width, DESIGN_RESOLUTION.height, ResolutionPolicy::EXACT_FIT);

    initUI();
    loadCardsFromJson();  // ��JSON���ؿ���

    return true;
}

void MainScene::initUI() {
    // ��������
    auto background = LayerColor::create(Color4B(0, 128, 0, 255)); // ��ɫ����
    this->addChild(background);

    // ��������ָ���
    auto separator = DrawNode::create();
    separator->drawLine(
        Vec2(0, PLAYFIELD_RECT.origin.y),
        Vec2(DESIGN_RESOLUTION.width, PLAYFIELD_RECT.origin.y),
        Color4F::WHITE
    );
    this->addChild(separator);

    // ��������ǩ
    auto playfieldLabel = Label::createWithTTF("Playfield (Main Area)", "fonts/Marker Felt.ttf", 24);
    playfieldLabel->setPosition(DESIGN_RESOLUTION.width / 2, PLAYFIELD_RECT.origin.y + PLAYFIELD_RECT.size.height - 30);
    this->addChild(playfieldLabel);

    auto stackLabel = Label::createWithTTF("Stack (Hand Area)", "fonts/Marker Felt.ttf", 24);
    stackLabel->setPosition(DESIGN_RESOLUTION.width / 2, STACK_RECT.size.height - 30);
    this->addChild(stackLabel);

    // �������Ʋ�
    m_cardLayer = Node::create();
    this->addChild(m_cardLayer);

    // ��ӱ���
    auto title = Label::createWithTTF("Poker Game", "fonts/Marker Felt.ttf", 64);
    title->setPosition(DESIGN_RESOLUTION.width / 2, DESIGN_RESOLUTION.height - 100);
    this->addChild(title);



    auto undoButton = ui::Button::create("back.png");  // ׼��һ�� undo ��ťͼƬ
    undoButton->setPosition(Vec2(DESIGN_RESOLUTION.width - 120, 80));  // ���½�
    undoButton->setScale(0.1f);
    undoButton->addClickEventListener([this](Ref* sender) {
       this->onUndo();
        });
    this->addChild(undoButton, 10);

}

void MainScene::loadCardsFromJson() {
    // JSON����
    const char* jsonData = R"(
    {
        "Playfield": [
            {
                "CardFace": 12,
                "CardSuit": 0,
                "Position": {"x": 250, "y": 1000}
            },
            {
                "CardFace": 2,
                "CardSuit": 0,
                "Position": {"x": 300, "y": 800}
            },
            {
                "CardFace": 2,
                "CardSuit": 1,
                "Position": {"x": 350, "y": 600}
            },
            {
                "CardFace": 2,
                "CardSuit": 0,
                "Position": {"x": 850, "y": 1000}
            },
            {
                "CardFace": 2,
                "CardSuit": 0,
                "Position": {"x": 800, "y": 800}
            },
            {
                "CardFace": 1,
                "CardSuit": 3,
                "Position": {"x": 750, "y": 600}
            }
        ],
        "Stack": [
            {
                "CardFace": 4,
                "CardSuit": 0,
                "Position": {"x": 0, "y": 0}
            },
            {
                "CardFace": 2,
                "CardSuit": 0,
                "Position": {"x": 0, "y": 0}
            },
            {
                "CardFace": 0,
                "CardSuit": 2,
                "Position": {"x": 0, "y": 0}
            },
  {
                "CardFace": 4,
                "CardSuit": 2,
                "Position": {"x": 0, "y": 0}
            },
            {
                "CardFace": 3,
                "CardSuit": 0,
                "Position": {"x": 0, "y": 0}
            }
        ]
    }
    )";

    // ����JSON
    rapidjson::Document doc;
    doc.Parse(jsonData);

    if (doc.HasParseError()) {
        log("JSON��������: %s", doc.GetParseError());
        return;
    }

    // ����������
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& playfieldArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++) {
            createCardFromJson(playfieldArray[i], "Playfield");
        }
    }

    // ���������
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& stackArray = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArray.Size(); i++) {
            createCardFromJson(stackArray[i], "Stack");
        }

        // ������������ƣ����λ��Ϊ0,0��
        arrangeStackCards();
    }
}

void MainScene::createCardFromJson(const rapidjson::Value& cardData, const std::string& zone) {
    // ��JSON��ȡ��������
    int face = cardData["CardFace"].GetInt();
    int suit = cardData["CardSuit"].GetInt();

    // ��ȡλ����Ϣ
    float x = cardData["Position"]["x"].GetFloat();
    float y = cardData["Position"]["y"].GetFloat();

    // ��������
    auto card = PokerCard::create(
        static_cast<CardSuitType>(suit),
        static_cast<CardFaceType>(face)
    );

    // ���ÿ���λ��
    if (x == 0 && y == 0) {
        // λ��Ϊ0,0ʱʹ��Ĭ��λ�ã�������������ж�������
        card->setPosition(0, 0);
    }
    else {
        // ����Y���꣬��ΪJSON�е���������������ĻΪ��׼��
        if (zone == "Playfield") {
            y += STACK_RECT.size.height; // ������Y������Ҫ���϶������߶�
        }
        card->setPosition(x, y);
    }

    m_cardLayer->addChild(card);
    m_deck.push_back(card);

    // ����������ӵ���ͬ�ļ���
    if (zone == "Playfield") {
        m_playfieldCards.push_back(card);
    }
    else {
        m_stackCards.push_back(card);
    }

    card->setCallback(CC_CALLBACK_1(MainScene::onExchangeTopCard, this));
}





void MainScene::arrangeStackCards() {
    // ��������������
    if (m_stackCards.empty()) return;

    const float paddingX = -50*(1+ m_stackCards.size()*0.2); // ���Ƽ��
    const float cardWidth = PokerCard::CARD_SIZE.width;
    const float totalWidth = m_stackCards.size() * cardWidth + (m_stackCards.size() - 1) * paddingX;
    float startX = (STACK_RECT.size.width - totalWidth) / 2;
    float yPos = STACK_RECT.size.height / 2;

    for (size_t i = 0; i < m_stackCards.size()-1; i++) {
        float x = startX + i * (cardWidth + paddingX);
        m_stackCards[i]->setAnchorPoint(Vec2(0, 0));

        //  m_stackCards[i]->runAction(MoveTo::create(0.5f, Vec2(PokerCard::CARD_SIZE.width / 2, PokerCard::CARD_SIZE.height / 2)));
        m_stackCards[i]->setPosition(Vec2(x, yPos));
        m_stackCards[i]->setLocalZOrder(static_cast<int>(i));
            //runAction(MoveTo::create(0.5f, Vec2(x, yPos)));
    }
    m_stackCards[m_stackCards.size() - 1]->setAnchorPoint(Vec2(0, 0));


    float endX = STACK_RECT.size.width - cardWidth;
    //  m_stackCards[i]->runAction(MoveTo::create(0.5f, Vec2(PokerCard::CARD_SIZE.width / 2, PokerCard::CARD_SIZE.height / 2)));
    m_stackCards[m_stackCards.size() - 1]->setPosition(Vec2(endX, yPos));
       // ->runAction(MoveTo::create(0.5f, Vec2(startX+ (m_stackCards.size() - 1) * cardWidth, yPos)));
}


void MainScene::onExchangeTopCard(PokerCard* card) {

    card->setSelected(!card->isSelected());

    auto it = std::find(m_stackCards.begin(), m_stackCards.end(), card);
    if (it != m_stackCards.end()) {
        // ����������������ƣ��Ҳ��Ƕ���
        if (m_stackCards.size() > 1 && card != m_stackCards.back()) {
            auto itCard = it;
            auto itTop = m_stackCards.end() - 1;
            recordCurrentStackPositions();
            // ��������˳��
            std::iter_swap(itCard, itTop);

            
            // ���������������ƣ�ͳһ���ƶ���λ�ã�
            arrangeStackCards();

  



        }
    }




    auto itPlay = std::find(m_playfieldCards.begin(), m_playfieldCards.end(), card);
    if (itPlay != m_playfieldCards.end()) {
        if (m_stackCards.empty()) return;

        PokerCard* topCard = m_stackCards.back();
        int topFace = static_cast<int>(topCard->getFace());
        int selectedFace = static_cast<int>(card->getFace());

        // �ж��Ƿ��ƥ�䣨������1��
        if (std::abs(topFace - selectedFace) == 1) {
            // ��ȡĿ��λ��
            Vec2 topPos = topCard->getPosition();


            recordCurrentStackPositions();
            // �����������Ʒɵ�����λ��
           // card->runAction(MoveTo::create(0.3f, topPos));
            card->setPosition(topPos);

            // �Ƴ����ƣ���ѡ���ӽ����Ƴ�/����/������
          //  topCard->removeFromParent();
           // m_stackCards.pop_back();
           
            // �滻����
            m_stackCards.push_back(*itPlay);

            // �����������Ƴ�
            m_playfieldCards.erase(itPlay);



         
            // �����������ƣ�����ͳһ���֣�
            arrangeStackCards();

           
            // Ȼ���ٽ���˳�򡢵��� arrangeStackCards()


        }
        else {
            log("������ƥ�䣬�޷��滻����");
        }
    }
}



void MainScene::recordCurrentStackPositions() {
    UndoAction action;

    for (auto* card : m_stackCards) {
        action.push_back({ card, card->getPosition(), SingleCardUndo::Zone::STACK });
    }

    for (auto* card : m_playfieldCards) {
        action.push_back({ card, card->getPosition(), SingleCardUndo::Zone::PLAYFIELD });
    }

    m_undoStack.push_back(action);
}



void MainScene::onUndo() {
    if (m_undoStack.empty()) {
        log("no back");
        return;
    }

    UndoAction action = m_undoStack.back();
    m_undoStack.pop_back();

    for (const auto& entry : action) {
        // �ָ�λ��
        entry.card->stopAllActions();
        entry.card->setPosition(entry.originalPos);

        // === �ؼ��㣺��ԭ���Ϲ��� ===

        // �������������Ƴ������ƣ������ظ���
        auto inStack = std::find(m_stackCards.begin(), m_stackCards.end(), entry.card);
        if (inStack != m_stackCards.end()) m_stackCards.erase(inStack);

        auto inPlay = std::find(m_playfieldCards.begin(), m_playfieldCards.end(), entry.card);
        if (inPlay != m_playfieldCards.end()) m_playfieldCards.erase(inPlay);

        // �Ż�ԭ���ļ���
        if (entry.originalZone == SingleCardUndo::Zone::STACK) {
            m_stackCards.push_back(entry.card);
        }
        else {
            m_playfieldCards.push_back(entry.card);
        }
    }

    // �����������
   // arrangeStackCards();

    log("back %d cards", static_cast<int>(action.size()));
}
