#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

//#include "cocos2d.h"
//#include <string>
//using std::string;
#include "SceneBase.h"


//class MainScene : public cocos2d::Layer
class SceneMain : public SceneBase
{
public:
    SceneMain();
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    //void setMenuColor(int index);
    void onNotImplemented(cocos2d::Ref* pSender);
    void onDoExit(cocos2d::Ref* pSender);

    static cocos2d::Scene* NotImplemented();
    static cocos2d::Scene* DoExit();


    // implement the "static create()" method manually
    CREATE_FUNC(SceneMain);
    //cocos2d::MenuItemLabel* m_menu[10];
//    std::vector<cocos2d::MenuItemLabel*> m_menu;
//    std::vector<FUNC_MAIN_SCENE> m_funcEnt;
    //FUNC_MAIN_SCENE m_funcEnt;
};

#endif // __MAIN_SCENE_H__
