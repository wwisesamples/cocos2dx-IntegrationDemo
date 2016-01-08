#pragma once
#include "SceneBase.h"

class SceneDialogueMenu : public SceneBase
{
public:
    SceneDialogueMenu();
    ~SceneDialogueMenu();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onRelease();
   // virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
 //   void setMenuColor(int index);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneDialogueMenu);

    cocos2d::Label * m_label;
//    std::vector<cocos2d::MenuItemLabel*> m_menu;
//    std::vector<FUNC_MAIN_SCENE> m_funcEnt;
};

