#pragma once

#include "SceneBase.h"


///////////////////////////
// SceneMusicCallbacksRoot
///////////////////////////
class SceneMusicCallbacksRoot : public SceneBase
{
public:
    SceneMusicCallbacksRoot();
    ~SceneMusicCallbacksRoot();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    virtual void onRelease();

    // implement the "static create()" method manually
    CREATE_FUNC(SceneMusicCallbacksRoot);

    void setMenuColor(int index);

    //int	m_menuix;
    cocos2d::Label * m_menu[3];
};


///////////////////////////
// SceneMusicCallbacksBase
///////////////////////////
class SceneMusicCallbacksBase : public SceneBase
{
public:

    /// DemoMarkers class constructor
    SceneMusicCallbacksBase(string strTitle, string strHelp = NULL, FUNC_MAIN_SCENE pfnc = NULL);

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);
    virtual void onRelease();


    // implement the "static create()" method manually
    //CREATE_FUNC(SceneMusicCallbacksRoot);

    /// Game Object ID for the "Narrator".
    static const AkGameObjectID GAME_OBJECT_MUSIC = 100;

    cocos2d::Label * m_label;
    cocos2d::Label * m_label2;

protected:

    bool m_bIsPlaying;
    bool m_bStopPlaylist;
    AkPlayingID m_iPlayingID;
};



///////////////////////////
// SceneMusicSyncCallbacks
///////////////////////////
class SceneMusicSyncCallbacks : public SceneMusicCallbacksBase
{
public:
    SceneMusicSyncCallbacks();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneMusicSyncCallbacks);

    /// Callback method for the events raised by Wwise while playing the markers demo audio.
    static void MusicCallback(
	AkCallbackType in_eType,			///< - The type of the callback
	AkCallbackInfo* in_pCallbackInfo	///< - Structure containing info about the callback
	);

private:
    AkUInt32 m_uiBeatCount;
    AkUInt32 m_uiBarCount;
};



///////////////////////////
// ScenePlayListCallbacks
///////////////////////////
class ScenePlayListCallbacks : public SceneMusicCallbacksBase
{
public:
    ScenePlayListCallbacks();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);

    // implement the "static create()" method manually
    CREATE_FUNC(ScenePlayListCallbacks);

    /// Callback method for the events raised by Wwise while playing the markers demo audio.
    static void MusicCallback(
	AkCallbackType in_eType,			///< - The type of the callback
	AkCallbackInfo* in_pCallbackInfo	///< - Structure containing info about the callback
	);

private:
    AkUInt32 m_uiPlaylistItem;
};


///////////////////////////
// SceneMIDICallbacks
///////////////////////////
class SceneMIDICallbacks : public SceneMusicCallbacksBase
{
public:
    SceneMIDICallbacks();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneMIDICallbacks);

    /// Callback method for the events raised by Wwise while playing the markers demo audio.
    static void MusicCallback(
	AkCallbackType in_eType,			///< - The type of the callback
	AkCallbackInfo* in_pCallbackInfo	///< - Structure containing info about the callback
	);

private:
    AkMidiNoteNo	m_byNote;
    AkUInt8		m_byVelocity;
    AkUInt8		m_byCc;
    AkUInt8		m_byValue;
};
