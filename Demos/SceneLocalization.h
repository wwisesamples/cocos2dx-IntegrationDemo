#pragma once
#include "SceneBase.h"

class SceneLocalization : public SceneBase
{
public:
    SceneLocalization();
    ~SceneLocalization();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onRelease();


    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onSayHello(cocos2d::Ref* pSender);
    void onLanguageChanged(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneLocalization);

    /// Game Object ID for the "Human".
    static const AkGameObjectID GAME_OBJECT_HUMAN = 100;

    cocos2d::Label * m_language;
};

