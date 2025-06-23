#pragma once
#include "cocos2d.h"
#include "../models/CardModel.h"

class CardView : public cocos2d::Node{
public:
    static CardView* create(const CardModel& model);
    virtual bool init(const CardModel& model);

    // ���õ���ص�
    void setCallback(const std::function<void(CardView*)>& callback) {
        _callback = callback;
    }

   
    // ���¿���״̬
    void updateView();

    // ��������
    const CardModel& getModel() const { return _model; }
    void setSelected(bool selected);
  cocos2d::Sprite* _cardBg = nullptr;
  bool _isSelected = false;
protected:
    CardView() = default;
    virtual ~CardView() {
       
            cocos2d::log("~~~~");
        
    };

    // UI�����ʼ��
    void initUI();
    cocos2d::Color3B getSuitColor() const;
    std::string getSuitResourceName() const;
    std::string getSmallFaceResourceName() const;
    std::string getBigFaceResourceName() const;
private:
    CardModel _model;
    std::function<void(CardView*)> _callback=nullptr;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
   /* void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);*/
    bool onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event); 
    bool isTouchInside(cocos2d::Touch* touch);
  
};