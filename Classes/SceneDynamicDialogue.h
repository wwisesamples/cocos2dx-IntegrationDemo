#pragma once
#include "SceneBase.h"
#include <AK/SoundEngine/Common/AkDynamicDialogue.h>
#include <AK/SoundEngine/Common/AkDynamicSequence.h>

class SceneDynamicDialogue : public SceneBase
{
public:
    SceneDynamicDialogue();
    //~SceneDynamicDialogue();

    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void update(float dt);
    virtual void onRelease();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void onStart(cocos2d::Ref* pSender);
    void onReset(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(SceneDynamicDialogue);

    /// Stops tests and resets the page back to the first test.
    void StopAndResetTests();

    /// Sets the time to wait before proceeding on to the next testing function.
    void Wait(
	AkUInt32 in_uTimeMS		///< - Time to wait in Milliseconds
	);

    /// True if the testing should be paused and false otherwise.
    bool m_bPause;

    /// True if a test is in progress.
    bool m_bTestInProgress;

    /// Pointer to the next testing function to run. Null when tests are stopped.
    void (SceneDynamicDialogue::*m_pNextFunction)();

    /// The number of ticks to wait - set by using Wait() between testing function calls.
    AkUInt32 m_uTicksToWait;

    /// Holds the index of the test set currently playing
    AkUInt8 m_uSetIndex;

    /// The playing ID of the dynamic sequence.
    AkPlayingID m_PlayingID;

    /// The number of tests that are being run.
    static const int NUM_TESTS = 18;

    /// Captions displayed for each test set ( +1 for blank ).
    static const char* CAPTIONS[NUM_TESTS + 1];

    /// Captions displayed for each test set, wrapped to
    /// platform-specific max width ( +1 for blank ).
    std::string m_captions[NUM_TESTS + 1];

    /// Game Object ID for the "Radio".
    static const AkGameObjectID GAME_OBJECT_RADIO = 100;

    cocos2d::Label * m_label[3];


    //////////////////////////////////////////////////////////////
    //         ##############   README   ##############
    //
    // The Dynamic Dialogue demo consists of 18 individual tests
    // that demonstrate how to implement various features and
    // what they would sound like.
    //
    // Each test is broken up into steps (ie - methods) which are
    // chained together to form a "set", usually with a waiting
    // period between steps. The wait in between steps is meant
    // to represent regular gameplay that does not affect the 
    // dynamic dialogue sequence.
    //////////////////////////////////////////////////////////////

    //////////////
    //	Test Set 1 - Simple Dynamic Sequence Using IDs.
    //////////////
    void Set1_1_SimpleSequenceUsingID();

    //////////////
    //	Test Set 2 - Simple Dynamic Sequence Using Strings.
    //////////////
    void Set2_1_SimpleSequenceUsingString();

    //////////////
    //	Test Set 3 - Add an item during playback.
    //////////////
    void Set3_1_StartPlayback();
    void Set3_2_AddItemToPlaylist();

    //////////////
    //	Test Set 4 - Insert an item into the list during playback.
    //////////////
    void Set4_1_StartPlayback();
    void Set4_2_InsertItemsToPlaylist();

    //////////////
    //	Test Set 5 - Add an item to an empty playlist during playback.
    //////////////
    void Set5_1_StartEmptyPlaylist();
    void Set5_2_AddItemsToPlaylist();
    void Set5_3_WaitForEmptyListThenAdd();

    //////////////
    //	Test Set 6 - Using the Stop call.
    //////////////
    void Set6_1_StartPlayback();
    void Set6_2_CallingStop();
    void Set6_3_ResumePlayingAfterStop();

    //////////////
    //	Test Set 7 - Using the Break call.
    //////////////
    void Set7_1_StartPlayback();
    void Set7_2_CallingBreak();
    void Set7_3_ResumePlayingAfterBreak();

    //////////////
    //	Test Set 8 - Using the Pause and Resume calls.
    //////////////
    void Set8_1_StartPlayback();
    void Set8_2_CallingPause();
    void Set8_3_CallingResumeAfterPause();

    //////////////
    //	Test Set 9 - Using a Delay when queueing to a playlist.
    //////////////
    void Set9_1_UsingDelay();

    //////////////
    //	Test Set 10 - Clearing the playlist during playback.
    //////////////
    void Set10_1_StartPlayback();
    void Set10_2_ClearPlaylist();

    //////////////
    //	Test Set 11 - Stopping on the playlist and clearing it.
    //////////////
    void Set11_1_StartPlayback();
    void Set11_2_StopAndClearPlaylist();

    //////////////
    //	Test Set 12 - Breaking the playlist and clearing it.
    //////////////
    void Set12_1_StartPlayback();
    void Set12_2_BreakAndClearPlaylist();

    //////////////
    //	Test Set 13 - Pausing the playlist and clearing it.
    //////////////
    void Set13_1_StartPlayback();
    void Set13_2_PausePlaylist();
    void Set13_3_ClearAndResumePlaylist();

    //////////////
    //	Test Set 14 - Using a callback with custom parameters.
    //////////////
    void Set14_1_StartPlaybackWithCallback();
    static void Set14_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);
    static int   mSet14_iParamIndex;
    static void* mSet14_CustomParams[3];

    //////////////
    //	Test Set 15 - Using a callback to cancel playback after 3 items have played.
    //////////////
    void Set15_1_StartPlaybackWithCallback();
    static void Set15_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);
    static int mSet15_iItemsPlayed;

    //////////////
    //	Test Set 16 - Using a callback to play 2 items in sequence.
    //////////////
    void Set16_1_StartPlaybackWithCallback();
    static void Set16_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);
    static AkPlayingID mSet16_Seq1PlayingID;
    static AkPlayingID mSet16_Seq2PlayingID;

    //////////////
    //	Test Set 17 - Checking playlist content during playback.
    //////////////
    void Set17_1_StartPlaybackWithCallback();
    void Set17_2_ValidatePlaylist();
    static void Set17_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);
    static bool  mSet17_bDonePlaying;
    static void* mSet17_CustomParams[6];

    //////////////
    //	Test Set 18 - Using events with Dynamic Dialogue.
    //////////////
    void Set18_1_StartPlayback();
    void Set18_2_PostPauseEvent();
    void Set18_3_PostResumeEvent();
    void Set18_4_PostStopEvent();
    void Set18_5_PlayRestOfSequence();
};

