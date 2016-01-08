#pragma once

#include "SceneBase.h"


class SceneMarkers : public SceneBase
{
public:
    SceneMarkers();
    ~SceneMarkers();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event);
    virtual void onRelease();

    // a selector callback
    void onPlayMarkersButton(cocos2d::Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneMarkers);

private:
    cocos2d::Label * m_label[3];
    /// Whether or not the car engine sound is currently playing.
    bool m_bPlayingEngine;

    /// Game Object ID for the "Car".
    static const AkGameObjectID GAME_OBJECT_CAR = 100;

    /// Callback method for the events raised by Wwise while playing the markers demo audio.
    static void MarkersCallback(
	AkCallbackType in_eType,			///< - The type of the callback
	AkCallbackInfo* in_pCallbackInfo	///< - Structure containing info about the callback
	);

    /// Tracks the current line during audio playback to sync audio with captions.
    unsigned int m_unCurrentLine;

    /// Whether the audio is currently playing or not.
    bool m_bPlayingMarkers;

    /// Holds the playing ID of the launched PLAY_MARKERS event.
    AkPlayingID m_iPlayingID;

    /// The number of lines of text to display in the captions.
    static const int NUM_LINES = 7;

    /// The caption text, one extra for a blank entry.
    static const char* SUBTITLES[NUM_LINES + 1];

    /// The caption text, wrapped to platform-specific max width,
    /// with one extra for a blank entry.
    std::string m_subtitles[NUM_LINES + 1];

    /// Game Object ID for the "Narrator".
    static const AkGameObjectID GAME_OBJECT_NARRATOR = 100;

};
