#pragma once
#include "SceneBase.h"
#include "SoundInput.h"

class SceneMicrophone : public SceneBase
{
public:
    SceneMicrophone();
    ~SceneMicrophone();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onRelease();

    // a selector callback
    void onStartRecord(cocos2d::Ref* pSender);
    void onDelay(cocos2d::Ref* pSender);

    void StartRecording();
    void StopRecording();

    // implement the "static create()" method manually
    CREATE_FUNC(SceneMicrophone);

    SoundInput* m_pInput;
    bool m_bPlaying;
    bool m_bDelayed;
    cocos2d::Label * m_pLabelStart;
    cocos2d::Label * m_pLabelDelay;
};

