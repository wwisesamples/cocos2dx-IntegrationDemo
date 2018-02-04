#pragma once

#include "SceneBase.h"
#ifndef ANDROID
#include "MovableChip.h"
#endif

#ifndef AK_VITA
#define _DEMOFOOTSTEPS_DYNAMIC_BANK_LOADING
#endif // AK_VITA


class SceneFootsteps : public SceneBase
{
public:
    SceneFootsteps();
    ~SceneFootsteps();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);
    virtual void onRelease();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event);
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event);

    void onMouseMove(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
    void onMouseDown(cocos2d::Event* event);

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    // implement the "static create()" method manually
    CREATE_FUNC(SceneFootsteps);

private:
    /// Delegate function for the "m_ctrlWeight" Numeric
//    void Weight_ValueChanged(void* in_pSender, ControlEvent* in_pEvent);
    void ManageSurfaces(int x, int y, int in_GameObject);
    void ManageEnvironement(int x, int y, int in_GameObject);
    void UpdateGameObjPos(float x, float y);

#if defined( _DEMOFOOTSTEPS_DYNAMIC_BANK_LOADING )
    int ComputeUsedBankMask(int x, int y);
#endif // defined( _DEMOFOOTSTEPS_DYNAMIC_BANK_LOADING )

//    MovableChip *m_pChip;
    cocos2d::EventListenerMouse*	    m_mouseListener;
    cocos2d::EventListenerTouchOneByOne*    m_touchListener;
    cocos2d::EventKeyboard::KeyCode	    m_key;

    float m_weight;
    float m_LastX;
    float m_LastY;

    int m_maskCurrentBanks;
    int m_iSurface;
    int m_iLastFootstepTick;
    int m_x, m_y;
    bool m_bPressed;
    bool m_bPressedMouse;

    bool m_bIsTouch;

    cocos2d::CCSprite* m_sprite;
    cocos2d::Label * m_label;
    //cocos2d::Label * m_labelPos;

    /// Whether or not the car engine sound is currently playing.
    bool m_bPlayingEngine;

    /// Game Object ID for the "Car".
    static const AkGameObjectID GAME_OBJECT_CAR = 100;
};
