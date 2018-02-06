#pragma once
#include "SceneBase.h"

class SceneBGM : public SceneBase
{
public:
    SceneBGM();
    ~SceneBGM();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onRelease();

    void Recordable_LabelPressed(cocos2d::Ref* pSender);
    void NonRecordable_LabelPressed(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneBGM);

    bool m_bPlayLicensed;
    bool m_bPlayCopyright;
};

