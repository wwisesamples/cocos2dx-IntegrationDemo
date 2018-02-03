#ifndef __RTPC_SCENE_H__
#define __RTPC_SCENE_H__
#include "SceneBase.h"

class SceneRTPCCarEngine : public SceneBase
{
public:
    SceneRTPCCarEngine();
    ~SceneRTPCCarEngine();

    static cocos2d::Scene* createScene();

    virtual bool init();
    //virtual void update(float dt);
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event);
    virtual void onRelease();

    void slideEvent(Ref* pSender, cocos2d::ui::Slider::EventType type);
    // a selector callback
    void onStartEngine(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);


    // implement the "static create()" method manually
    CREATE_FUNC(SceneRTPCCarEngine);

private:
    cocos2d::ui::Slider*    m_slider;
    cocos2d::Label *	    m_startEngineLabel;
    cocos2d::Label *	    m_RPMLabel;
    /// Whether or not the car engine sound is currently playing.
    bool m_bPlayingEngine;

    /// Game Object ID for the "Car".
    static const AkGameObjectID GAME_OBJECT_CAR = 100;
};

#endif // __RTPC_SCENE_H__
