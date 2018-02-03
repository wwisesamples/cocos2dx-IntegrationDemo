#pragma once
#include "SceneBase.h"

class SceneExternalSources : public SceneBase
{
public:
    SceneExternalSources();
    ~SceneExternalSources();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onRelease();

    // a selector callback
    void onPlay123_Pressed(cocos2d::Ref* pSender);
    void onPlay456_Pressed(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneExternalSources);

    /// Game Object ID for the "Human".
    static const AkGameObjectID GAME_OBJECT_HUMAN = 100;

    /// File package ID. This file package contains all the external sources.
    AkUInt32	m_uPackageID;
};

