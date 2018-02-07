#include "SceneMain.h"
#include "SceneDialogueMenu.h"
#include "SceneDynamicDialogue.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine
#include "Helpers.h"
#include "Platform.h"
#include "PlatformCocos.h"

#ifdef ANDROID
#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	95
#endif

/////////////////////////////////////////////////////////////////////
// SceneDynamicDialogue Constant Member Initialization
/////////////////////////////////////////////////////////////////////

// We start writing the text at 1/10 the width of the screen. We want
// a similar margin on the right, so let's wrap at 8/10 of the width:
#define DEMO_DLG_MAX_CAPTION_WIDTH ( INTEGRATIONDEMO_SCREEN_CHAR_WIDTH * 8 / 10 )

const char* SceneDynamicDialogue::CAPTIONS[] =
{
    "",
    "Test 1 - Playing a simple dynamic sequence (using IDs).",
    "Test 2 - Playing a simple dynamic sequence (using strings).",
    "Test 3 - Add an item during playback.",
    "Test 4 - Insert an item into the list during playback.",
    "Test 5 - Add an item to an empty list during playback.",
    "Test 6 - Using the Stop call.",
    "Test 7 - Using the Break call.",
    "Test 8 - Using the Pause and Resume calls.",
    "Test 9 - Using a Delay when queueing to a playlist.",
    "Test 10 - Clearing the playlist during playback.",
    "Test 11 - Stopping the playlist and clearing it.",
    "Test 12 - Breaking the playlist and clearing it.",
    "Test 13 - Pausing the playlist and clearing it.",
    "Test 14 - Using a callback with custom parameters.",
    "Test 15 - Using a callback to cancel after 3 items play.",
    "Test 16 - Using a callback to play 2 items in sequence.",
    "Test 17 - Checking playlist content during playback.",
    "Test 18 - Using events with Dynamic Dialogue."
};

USING_NS_CC;

namespace
{
    template <typename Layer>
    void replaceScene(Ref*) {
	Director::getInstance()->replaceScene(Layer::createScene());
    }
}

SceneDynamicDialogue::SceneDynamicDialogue()
: SceneBase("Dynamic Dialogue Demos Menu", 
    "The \"Dynamic Dialogue\" demo runs through a series of "
    "tests using Wwise's Dynamic Dialogue features. Each "
    "test demonstrates a different control flow so that you "
    "can hear the effect it would produce.\n\n"
    "Press the \"Start Tests\" button to start the tests.\n"
    "Pressing the button again will allow the test "
    "currently running to complete and then pause testing.\n"
    "You can also press the \"Restart Tests\" button to "
    "start over from the beginning.",
    SceneDialogueMenu::createScene)
{
    m_PlayingID = 0;
    m_label[0] = m_label[1] = m_label[2] = NULL;
    for (int i = 0; i < (NUM_TESTS + 1); i++)
    {
	m_captions[i] = IntegrationDemoHelpers::WordWrapString(CAPTIONS[i], DEMO_DLG_MAX_CAPTION_WIDTH);
    }
}

//SceneDynamicDialogue::~SceneDynamicDialogue()
void SceneDynamicDialogue::onRelease()
{
    m_label[0] = m_label[1] = m_label[2] = NULL;
    // Ends any tests that might currently be running.
    StopAndResetTests();
    // Unregister the "Markers" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_RADIO);
    // Unload the sound bank
    AK::SoundEngine::UnloadBank("DynamicDialogue.bnk", NULL);
}

Scene* SceneDynamicDialogue::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = SceneDynamicDialogue::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneDynamicDialogue::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneBase::init())
    {
	return false;
    }
    cocos2d::Size windowSize = Director::getInstance()->getWinSize();
    float descriptionPosY = windowSize.height * SCREEN_POS_SCALER_DESCRIPTION_Y;
    float selectButtonPosX = g_isLandscape ? windowSize.width * SCREEN_POS_SCALER_SELBUTTON_X : windowSize.height * SCREEN_POS_SCALER_SELBUTTON_X;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    {
	int y = descriptionPosY;

	// Add button linking to the Start Tests
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneDynamicDialogue::onStart, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    //m_label[0] = addLabel("Start Tests", selectItem->getPosition().x + selectItem->getContentSize().width * 6, selectItem->getPosition().y, FONT_SIZE_MENU);
	    MenuItemLabel* pItem = addLabelEx("Start Tests", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this,
		CC_CALLBACK_1(SceneDynamicDialogue::onStart, this));
	    m_label[0] = (Label*)pItem->getChildren().at(0);
	}

	//auto resetItem;
	int x = 0, yy=0;
	// Add button linking to Reset
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneDynamicDialogue::onReset, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    addLabelEx("Reset", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this,
		CC_CALLBACK_1(SceneDynamicDialogue::onReset, this));
	    x = selectItem->getPosition().x + selectItem->getContentSize().width/* * 6*/;
	    yy = selectItem->getContentSize().height;
	}

	// Add button linking to Reset
	{
	    y -= yy;
	    m_label[1] = addLabel(" ", x, y, FONT_SIZE_MENU, this);
	    y -= yy;
	    m_label[2] = addLabel(" ", x, y, FONT_SIZE_MENU-2, this);
	}
    }

    // Load the sound bank
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("DynamicDialogue.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetLoadFileErrorMessage("DynamicDialogue.bnk.");
	return false;
    }

    // Register the "Markers" game object
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_RADIO, "Radio");

    // Initialize Demo-specific variables
    m_pNextFunction = NULL;
    m_uTicksToWait = 0;
    m_uSetIndex = 0;
    m_bPause = false;
    m_bTestInProgress = false;
    scheduleUpdate();

    return true;
}


void SceneDynamicDialogue::menuCloseCallback(Ref* pSender)
{
    //// Ends any tests that might currently be running.
    //StopAndResetTests();
    //// Unregister the "Markers" game object
    //AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_RADIO);
    //// Unload the sound bank
    //AK::SoundEngine::UnloadBank("DynamicDialogue.bnk", NULL);

    //auto director = Director::getInstance();
    //director->replaceScene(SceneDialogueMenu::createScene());
    //this->removeAllChildren();
}

void SceneDynamicDialogue::onStart(cocos2d::Ref* pSender)
{
    if (m_label[1]) m_label[1]->setString("");
    if (m_label[2]) m_label[2]->setString("");

    if (!m_pNextFunction && !m_bTestInProgress)
    {
	// No tests are playing.. start playing tests!
	m_uSetIndex = 0;
	m_pNextFunction = &SceneDynamicDialogue::Set1_1_SimpleSequenceUsingID;
	m_bPause = false;
	m_label[0]->setString("Pause After Current Test");
    }
    else
    {
	// Tests are playing, user wants to Pause/Resume
	if (m_bPause)
	{
	    // Play is paused, user wants to resume
	    m_label[0]->setString("Pause After Current Test");
	}
	else
	{
	    // User wants to Pause
	    m_label[0]->setString("Resume Tests");
	}
	m_bPause = !m_bPause;
    }

}

void SceneDynamicDialogue::onReset(cocos2d::Ref* pSender)
{
    // Ends any tests that might currently be running.
    StopAndResetTests();
}

void SceneDynamicDialogue::StopAndResetTests()
{
    // Clear out the playlist if anything's left in it.
    if (m_uSetIndex == 15)
    {
	// Set 16 requires a special clear as it uses different Sequence IDs.
	AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(mSet16_Seq1PlayingID);
	if (pPlaylist)
	{
	    AK::SoundEngine::DynamicSequence::Stop(mSet16_Seq1PlayingID);
	    pPlaylist->RemoveAll();
	    AK::SoundEngine::DynamicSequence::UnlockPlaylist(mSet16_Seq1PlayingID);
	    AK::SoundEngine::DynamicSequence::Close(mSet16_Seq1PlayingID);
	}
	pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(mSet16_Seq2PlayingID);
	if (pPlaylist)
	{
	    AK::SoundEngine::DynamicSequence::Stop(mSet16_Seq2PlayingID);
	    pPlaylist->RemoveAll();
	    AK::SoundEngine::DynamicSequence::UnlockPlaylist(mSet16_Seq2PlayingID);
	    AK::SoundEngine::DynamicSequence::Close(mSet16_Seq2PlayingID);
	}
    }
    else if (m_PlayingID != 0)
    {
	AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
	if (pPlaylist)
	{
	    AK::SoundEngine::DynamicSequence::Stop(m_PlayingID);
	    pPlaylist->RemoveAll();
	    AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
	    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
	}
    }

    m_uSetIndex = 0;
    m_pNextFunction = NULL;
    m_bPause = false;
    m_bTestInProgress = false;
/*
    if (m_Controls.size() > 0)
    {
	m_Controls[0]->SetLabel("Start Tests");
    }*/
    if (m_label[0]) m_label[0]->setString("Start Tests");
    if (m_label[1]) m_label[1]->setString("");
    if (m_label[2]) m_label[2]->setString("");

    m_uTicksToWait = 0;
}

void SceneDynamicDialogue::Wait(AkUInt32 in_uTimeMS)
{
    m_uTicksToWait = /*m_pParentMenu->GetFrameRate()*/DESIRED_FPS * in_uTimeMS / 1000;
}


void SceneDynamicDialogue::update(float dt)
{
    SceneBase::update(dt);

    // Tests are have started playing, check if we're waiting or not.
    if (m_uTicksToWait)
    {
	--m_uTicksToWait;
    }
    else
    {
	// The timer is up.. If the game is paused, don't move on to the next test yet.
	// If however the game is paused but we're in the middle of a particular test, continue calling
	// the functions of that test.
	if ((!m_bPause || m_bTestInProgress) && m_pNextFunction)
	{
	    (this->*m_pNextFunction)();
	}
    }

    // If there is an error, don't draw anything further.
    if (m_szError.size())
    {
	return;
    }

    // If tests are playing, show which test is currently playing.
    if (m_uSetIndex > 0)
    {
	//int iWidth = m_pParentMenu->GetWidth();
	//int iHeight = m_pParentMenu->GetHeight();

	//DrawTextOnScreen("Now Playing:", iWidth / 10, iHeight / 2);
	//DrawTextOnScreen(m_captions[m_uSetIndex].c_str(), iWidth / 10, 6 * iHeight / 10, DrawStyle_Text);

	if (m_label[1]) m_label[1]->setString("Now Playing:");
	if (m_label[2]) m_label[2]->setString(m_captions[m_uSetIndex].c_str());
    }
}


/////////////////////////////////////////////////////////////////////
// SceneDynamicDialogue Test Sets
/////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set1_1_SimpleSequenceUsingID()
{
    // Starting Test Set 1
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    // Open a dynamic sequence on the GAME_OBJECT_ID_DYNAMICDIALOGUE game object
    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    // Locking the playlist for edition
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	// In this section we will be adding 3 items in the playlist: Comm_In, Speech, Comm_Out

	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	// Resolve the dialogue Event
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	// Enqueue the Audio node in the playlist
	pPlaylist->Enqueue(audioNodeID);

	// Set up the arguments array
	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK_FALLBACK_ARGUMENTVALUE_ID,
	    AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED };

	// Resolve the dialogue Event using the arguments array. 
	// Note: It's very important to have the same argument order in Wwise and in the code.
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    // Play the dynamic sequence
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Close the dynamic sequence
    // NOTE: It will continue to play until the end, but you won't be able to modify it anymore.
    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 1 - Wait a short while and move on to Set 2
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set2_1_SimpleSequenceUsingString;
    Wait(5000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set2_1_SimpleSequenceUsingString()
{
    // Starting Test Set 2
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    // Open a dynamic sequence on the GAME_OBJECT_RADIO game object
    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    // Locking the playlist for editting
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	// Adding 3 items in the playlist: Comm_In, Speech, Comm_Out
	const AkOSChar*  szWalkieTalkieInArgument[] = { AKTEXT("Comm_In") };
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AKTEXT("WalkieTalkie"), szWalkieTalkieInArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	const AkOSChar*  underAttackStringArguments[] = { AKTEXT("Unit_A"),
	    AKTEXT("Gang"),
	    AKTEXT("Hangar") };

	// Resolve the dialogue Event using the arguments strings array.
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AKTEXT("Unit_Under_Attack"), underAttackStringArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	const AkOSChar*  szWalkieTalkieOutArgument[] = { AKTEXT("Comm_Out") };
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AKTEXT("WalkieTalkie"), szWalkieTalkieOutArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);
    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 2 - Wait a short while and move on to Set 3
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set3_1_StartPlayback;
    Wait(5000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set3_1_StartPlayback()
{
    // Starting Test Set 3
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID underAttackArguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::HOSTILE::STATE::BUM,
	    AK::STATES::LOCATION::STATE::STREET };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, underAttackArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started the playback. Wait a bit and go to step 2.
    Wait(500);
    m_pNextFunction = &SceneDynamicDialogue::Set3_2_AddItemToPlaylist;
}

void SceneDynamicDialogue::Set3_2_AddItemToPlaylist()
{
    m_pNextFunction = NULL;

    // Lock the playlist to add item during playback
    // NOTE: You should not keep the playlist locked for too long when the sequence is playing. 
    //       It could result in a stall in the Sound Engine.
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	// Add new item in the playlist during the playback of the dynamic sequence
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE,
	    AK::STATES::OBJECTIVESTATUS::STATE::FAILED };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	// Unlock the playlist as soon as possible to prevent problems
	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 3 - Wait a short while and move on to Set 4
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set4_1_StartPlayback;
    Wait(5000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set4_1_StartPlayback()
{
    // Starting Test Set 4
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID underAttackArguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::HOSTILE::STATE::BUM,
	    AK::STATES::LOCATION::STATE::STREET };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, underAttackArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	underAttackArguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	underAttackArguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	underAttackArguments[2] = AK_FALLBACK_ARGUMENTVALUE_ID;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, underAttackArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	underAttackArguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	underAttackArguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	underAttackArguments[2] = AK::STATES::LOCATION::STATE::HANGAR;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, underAttackArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started the playback. Wait a bit and go to Step 2.
    Wait(500);
    m_pNextFunction = &SceneDynamicDialogue::Set4_2_InsertItemsToPlaylist;
}

void SceneDynamicDialogue::Set4_2_InsertItemsToPlaylist()
{
    m_pNextFunction = NULL;

    // Here we will add items to the front or middle of the playlist during playback.
    // Lock the playlist to add items during playback.
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);

    if (pPlaylist)
    {

	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);

	// Insert a new item in front of the playlist
	// Make sure to initialize all members of the PlaylistItem struct
	AK::SoundEngine::DynamicSequence::PlaylistItem* pPlaylistItem = pPlaylist->Insert(0);
	pPlaylistItem->audioNodeID = audioNodeID;
	pPlaylistItem->msDelay = 0;
	pPlaylistItem->pCustomInfo = NULL;

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE,
	    AK::STATES::OBJECTIVESTATUS::STATE::FAILED };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);

	// Insertion in the middle of the playlist
	pPlaylistItem = pPlaylist->Insert(1);
	pPlaylistItem->audioNodeID = audioNodeID;
	pPlaylistItem->msDelay = 0;
	pPlaylistItem->pCustomInfo = NULL;

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);

	// Insertion in the middle of the playlist
	pPlaylistItem = pPlaylist->Insert(2);
	pPlaylistItem->audioNodeID = audioNodeID;
	pPlaylistItem->msDelay = 0;
	pPlaylistItem->pCustomInfo = NULL;

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 4 - Wait a short while and move on to Set 5
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set5_1_StartEmptyPlaylist;
    Wait(10000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set5_1_StartEmptyPlaylist()
{
    // Starting Test Set 5
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);
    // Start the playback right away.. we will add new items directly in the already playing
    // Dynamic sequence
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started the playback. Wait a bit and go to Step 2
    Wait(500);
    m_pNextFunction = &SceneDynamicDialogue::Set5_2_AddItemsToPlaylist;
}

void SceneDynamicDialogue::Set5_2_AddItemsToPlaylist()
{
    m_pNextFunction = NULL;

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID underAttackArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK::STATES::HOSTILE::STATE::GANG,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, underAttackArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	// As soon as you unlock the playlist the new items will start to play
	// NOTE: Do not keep the playlist locked after the insertion! To trigger the playback at the right time,
	//       stop the dynamic sequence before adding the item and play it at the right time.
	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    // Item added, go to Step 3
    m_pNextFunction = &SceneDynamicDialogue::Set5_3_WaitForEmptyListThenAdd;
}

void SceneDynamicDialogue::Set5_3_WaitForEmptyListThenAdd()
{
    // Wait until the playlist is empty (at which point the last item will be playing),
    // then add new items that will start to play right away
    // NOTE: The newly added items are played with sample accuracy if the last item is still playing.

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist && pPlaylist->IsEmpty())
    {
	// The playlist is empty we can add new items and they will play right away.
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::OBJECTIVE::STATE::DEFUSEBOMB,
	    AK::STATES::OBJECTIVESTATUS::STATE::FAILED };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);

	AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
	m_PlayingID = 0;

	// End of Test Set 5 - Wait a short while and move on to Set 6
	m_bTestInProgress = false;
	m_pNextFunction = &SceneDynamicDialogue::Set6_1_StartPlayback;
	Wait(5000);
    }
    else
    {
	if (pPlaylist)
	{
	    // Don't keep the playlist locked.
	    AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
	}

	// Keep waiting until the playlist is empty.
	Wait(200);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set6_1_StartPlayback()
{
    // Starting Test Set 6
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID arguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::HOSTILE::STATE::BUM,
	    AK::STATES::LOCATION::STATE::STREET };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::LOCATION::STATE::HANGAR;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started the playback. Wait a bit and go to Step 2
    Wait(1000);
    m_pNextFunction = &SceneDynamicDialogue::Set6_2_CallingStop;
}

void SceneDynamicDialogue::Set6_2_CallingStop()
{
    m_pNextFunction = NULL;

    // Stop the playback immediatly
    AK::SoundEngine::DynamicSequence::Stop(m_PlayingID);

    // Wait a bit and go to Step 3
    Wait(2000);
    m_pNextFunction = &SceneDynamicDialogue::Set6_3_ResumePlayingAfterStop;
}

void SceneDynamicDialogue::Set6_3_ResumePlayingAfterStop()
{
    m_pNextFunction = NULL;

    // Play the rest of the sequence
    // NOTE: The sequence will restart with the item that was in the playlist
    //       after the item that was stopped.
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 6 - Wait a short while and move on to Set 7
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set7_1_StartPlayback;
    Wait(4000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set7_1_StartPlayback()
{
    // Starting Test Set 7
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID arguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::HOSTILE::STATE::BUM,
	    AK::STATES::LOCATION::STATE::STREET };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::LOCATION::STATE::HANGAR;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback. Wait a bit go to Step 2.
    Wait(1000);
    m_pNextFunction = &SceneDynamicDialogue::Set7_2_CallingBreak;
}

void SceneDynamicDialogue::Set7_2_CallingBreak()
{
    m_pNextFunction = NULL;

    // Break the sequence, it will stop after the current item.
    AK::SoundEngine::DynamicSequence::Break(m_PlayingID);

    // Leave time for the item to finish, go to Step 3.
    Wait(4000);
    m_pNextFunction = &SceneDynamicDialogue::Set7_3_ResumePlayingAfterBreak;
}

void SceneDynamicDialogue::Set7_3_ResumePlayingAfterBreak()
{
    m_pNextFunction = NULL;

    // Play the rest of the sequence
    // NOTE: The sequence will play the item that was right after the one breaked.
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 7 - Wait a short while and move on to Set 8
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set8_1_StartPlayback;
    Wait(5000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set8_1_StartPlayback()
{
    // Starting Test Set 8
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID arguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::HOSTILE::STATE::BUM,
	    AK::STATES::LOCATION::STATE::STREET };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::LOCATION::STATE::HANGAR;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback. Wait a bit and go to Step 2.
    Wait(1000);
    m_pNextFunction = &SceneDynamicDialogue::Set8_2_CallingPause;
}

void SceneDynamicDialogue::Set8_2_CallingPause()
{
    m_pNextFunction = NULL;

    // Pause the sequence.
    AK::SoundEngine::DynamicSequence::Pause(m_PlayingID);

    // Wait a bit and go to Step 3.
    Wait(2000);
    m_pNextFunction = &SceneDynamicDialogue::Set8_3_CallingResumeAfterPause;
}

void SceneDynamicDialogue::Set8_3_CallingResumeAfterPause()
{
    m_pNextFunction = NULL;

    // Resume the sequence.
    // NOTE: The paused item will resume playing, followed by the rest of the sequence.
    AK::SoundEngine::DynamicSequence::Resume(m_PlayingID);

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 8 - Wait a short while and move on to Set 9
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set9_1_UsingDelay;
    Wait(8000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set9_1_UsingDelay()
{
    // Starting Test Set 9
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK_FALLBACK_ARGUMENTVALUE_ID,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	// Add a delay between the Comm_In click and the start of the speech
	// NOTE: Delays are in milliseconds.
	pPlaylist->Enqueue(audioNodeID, 300);

	AkArgumentValueID underAttackArguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::HOSTILE::STATE::GANG,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, underAttackArguments, 3);
	// Add a delay between the 2 speeches
	pPlaylist->Enqueue(audioNodeID, 1500);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	// Add a small delay before the Comm_Out
	pPlaylist->Enqueue(audioNodeID, 400);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 9 - Wait a short while and move on to Set 10
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set10_1_StartPlayback;
    Wait(7000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set10_1_StartPlayback()
{
    // Starting Test Set 10
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    // Specifying DynamicSequenceType_NormalTransition prevents the next sound in the playlist from 
    // being prepared in advance and removed from the playlist before the current sound finishes playing.
    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(
	GAME_OBJECT_RADIO,
	0,
	NULL,
	NULL,
	AK::SoundEngine::DynamicSequence::DynamicSequenceType_NormalTransition);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID arguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK::STATES::HOSTILE::STATE::GANG,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::LOCATION::STATE::ALLEY;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	arguments[1] = AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE;
	arguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback. Wait a bit and go to Step 2.
    Wait(1000);
    m_pNextFunction = &SceneDynamicDialogue::Set10_2_ClearPlaylist;
}

void SceneDynamicDialogue::Set10_2_ClearPlaylist()
{
    m_pNextFunction = NULL;

    // Clear the playlist
    // NOTE: The item currently playing will continue to play until it finishes.
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	pPlaylist->RemoveAll();
	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 10 - Wait a short while and move on to Set 11
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set11_1_StartPlayback;
    Wait(3500);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set11_1_StartPlayback()
{
    // Starting Test Set 11
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID arguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK::STATES::HOSTILE::STATE::GANG,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::LOCATION::STATE::ALLEY;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	arguments[1] = AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE;
	arguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback. Wait a bit and go to Step 2.
    Wait(1000);
    m_pNextFunction = &SceneDynamicDialogue::Set11_2_StopAndClearPlaylist;
}

void SceneDynamicDialogue::Set11_2_StopAndClearPlaylist()
{
    m_pNextFunction = NULL;

    // Stop the sequence.
    AK::SoundEngine::DynamicSequence::Stop(m_PlayingID);

    // Clear the playlist and play. 
    // NOTE: Nothing should play since the playlist has been cleared.
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	pPlaylist->RemoveAll();
	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 11 - Wait a short while and move on to Set 12
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set12_1_StartPlayback;
    Wait(3500);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set12_1_StartPlayback()
{
    // Starting Test Set 12
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID arguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK::STATES::HOSTILE::STATE::GANG,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::LOCATION::STATE::ALLEY;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	arguments[1] = AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE;
	arguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback. Wait a bit and go to Step 2.
    Wait(500);
    m_pNextFunction = &SceneDynamicDialogue::Set12_2_BreakAndClearPlaylist;
}

void SceneDynamicDialogue::Set12_2_BreakAndClearPlaylist()
{
    m_pNextFunction = NULL;

    // Break the sequence, the currently playing item will finish playing.
    AK::SoundEngine::DynamicSequence::Break(m_PlayingID);

    // Clear the playlist and play.
    // NOTE: Nothing should play since the playlist has been cleared.
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	pPlaylist->RemoveAll();
	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 12 - Wait a short while and move on to Set 13
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set13_1_StartPlayback;
    Wait(5500);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set13_1_StartPlayback()
{
    // Starting Test Set 13
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    // Specifying DynamicSequenceType_NormalTransition prevents the next sound in the playlist from 
    // being prepared in advance and removed from the playlist before the current sound finishes playing.
    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(
	GAME_OBJECT_RADIO,
	0,
	NULL,
	NULL,
	AK::SoundEngine::DynamicSequence::DynamicSequenceType_NormalTransition);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID arguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK::STATES::HOSTILE::STATE::GANG,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	arguments[1] = AK_FALLBACK_ARGUMENTVALUE_ID;
	arguments[2] = AK::STATES::LOCATION::STATE::ALLEY;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	arguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	arguments[1] = AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE;
	arguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, arguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback. Wait a bit and go to Step 2.
    Wait(1000);
    m_pNextFunction = &SceneDynamicDialogue::Set13_2_PausePlaylist;
}

void SceneDynamicDialogue::Set13_2_PausePlaylist()
{
    m_pNextFunction = NULL;

    // Pause the sequence.
    AK::SoundEngine::DynamicSequence::Pause(m_PlayingID);

    // Playback is paused, wait a bit and move on to Step 3.
    Wait(2000);
    m_pNextFunction = &SceneDynamicDialogue::Set13_3_ClearAndResumePlaylist;
}

void SceneDynamicDialogue::Set13_3_ClearAndResumePlaylist()
{
    m_pNextFunction = NULL;

    // Clear the playlist and resume.
    // NOTE: Only the item that was paused will resume playback - the rest
    //       have been cleared.
    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	pPlaylist->RemoveAll();
	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }
    AK::SoundEngine::DynamicSequence::Resume(m_PlayingID);

    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // End of Test Set 13 - Wait a short while and move on to Set 14
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::Set14_1_StartPlaybackWithCallback;
    Wait(4000);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize the static member variables.
int   SceneDynamicDialogue::mSet14_iParamIndex = 0;
void* SceneDynamicDialogue::mSet14_CustomParams[] = { (void*)123, (void*)456, (void*)789 };

void SceneDynamicDialogue::Set14_1_StartPlaybackWithCallback()
{
    // Starting Test Set 14
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    mSet14_iParamIndex = 0;


    // Open a dynamic sequence specifying that we want ot be notified when:
    //   - each item ends (AK_EndOfDynamicSequenceItem) 
    //   - the sequence is finished (AK_EndOfEvent)
    // Specify the callback and the "this" pointer as a cookie.
    // The callback will ensure that the information received matches what is expected.
    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO,
	AK_EndOfDynamicSequenceItem | AK_EndOfEvent,
	SceneDynamicDialogue::Set14_Callback,
	this);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID, 0, mSet14_CustomParams[0]);

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK_FALLBACK_ARGUMENTVALUE_ID,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID, 0, mSet14_CustomParams[1]);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID, 0, mSet14_CustomParams[2]);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // NOTE: Make sure to close the dynamic sequence or you won't receive the end of sequence callback notification.
    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;


    // Started playback - let the callback handle the rest.
}

void SceneDynamicDialogue::Set14_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    SceneDynamicDialogue* pDemoInstance = (SceneDynamicDialogue*)in_pCallbackInfo->pCookie;

    // End of Item callback
    if (in_eType == AK_EndOfDynamicSequenceItem)
    {
	// prepare for the next item
	++mSet14_iParamIndex;
    }
    // End of sequence callback (Only called if the sequence is closed)
    else if (in_eType == AK_EndOfEvent)
    {
	// Done with Test Set 14 - wait a bit and move on to Set 15.
	pDemoInstance->m_bTestInProgress = false;
	pDemoInstance->m_pNextFunction = &SceneDynamicDialogue::Set15_1_StartPlaybackWithCallback;
	pDemoInstance->Wait(2000);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize static member variables.
int SceneDynamicDialogue::mSet15_iItemsPlayed = 0;

void SceneDynamicDialogue::Set15_1_StartPlaybackWithCallback()
{
    // Starting Test Set 15
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;
    mSet15_iItemsPlayed = 0;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO,
	AK_EndOfDynamicSequenceItem | AK_EndOfEvent,
	&SceneDynamicDialogue::Set15_Callback,
	this);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	// Item 1 (Comm_in)
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	// Item 2 (Mission Completed)
	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK_FALLBACK_ARGUMENTVALUE_ID,
	    AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED };
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	// Item 3 (Neutralize Hostile Failed)
	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	// Item 4 (Rescue Hostage Failed)
	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::RESCUEHOSTAGE;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	// Item 5 (Defuse Bomb Failed)
	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::DEFUSEBOMB;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	// Item 6 (Comm_out)
	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback with the callback. Let the callback handle the rest.
}

void SceneDynamicDialogue::Set15_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    SceneDynamicDialogue* pDemoInstance = (SceneDynamicDialogue*)in_pCallbackInfo->pCookie;

    if (in_eType == AK_EndOfDynamicSequenceItem)
    {
	++mSet15_iItemsPlayed;

	if (mSet15_iItemsPlayed == 2)
	{
	    // The second item has finished playing - the third item is now playing.
	    // Clear out the rest of the items on the playlist and readd the Comm_out.
	    // NOTE: Items 4 and 5 should not be heard!
	    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(pDemoInstance->m_PlayingID);
	    if (pPlaylist)
	    {
		AkUniqueID lastAudioNode = pPlaylist->Last().audioNodeID;
		pPlaylist->RemoveAll();

		pPlaylist->Enqueue(lastAudioNode);

		AK::SoundEngine::DynamicSequence::UnlockPlaylist(pDemoInstance->m_PlayingID);
	    }

	    AK::SoundEngine::DynamicSequence::Close(pDemoInstance->m_PlayingID);

	    // Done with Test Set 15 - wait a bit and move on to Set 16.
	    pDemoInstance->m_bTestInProgress = false;
	    pDemoInstance->m_pNextFunction = &SceneDynamicDialogue::Set16_1_StartPlaybackWithCallback;
	    pDemoInstance->Wait(2000);
	}
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize static member variables.
AkPlayingID SceneDynamicDialogue::mSet16_Seq1PlayingID = 0;
AkPlayingID SceneDynamicDialogue::mSet16_Seq2PlayingID = 0;

void SceneDynamicDialogue::Set16_1_StartPlaybackWithCallback()
{
    // Starting Test Set 16
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    // Open Sequence 1
    mSet16_Seq1PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO, AK_EndOfEvent, Set16_Callback, this);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(mSet16_Seq1PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK_FALLBACK_ARGUMENTVALUE_ID,
	    AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(mSet16_Seq1PlayingID);
    }

    // Open Sequence 2
    mSet16_Seq2PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO, AK_EndOfEvent, Set16_Callback);

    pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(mSet16_Seq2PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID underAttackArguments[] = { AK::STATES::UNIT::STATE::UNIT_A,
	    AK::STATES::HOSTILE::STATE::BUM,
	    AK_FALLBACK_ARGUMENTVALUE_ID };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::UNIT_UNDER_ATTACK, underAttackArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(mSet16_Seq2PlayingID);
    }

    // Start playing Sequence 1.
    AK::SoundEngine::DynamicSequence::Play(mSet16_Seq1PlayingID);
    AK::SoundEngine::DynamicSequence::Close(mSet16_Seq1PlayingID);

    // Playback has started with callback. Let the Callback handle the rest.
}

void SceneDynamicDialogue::Set16_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    SceneDynamicDialogue* pDemoInstance = (SceneDynamicDialogue*)in_pCallbackInfo->pCookie;
    AkEventCallbackInfo * pEventInfo = (AkEventCallbackInfo *)in_pCallbackInfo;

    if (in_eType == AK_EndOfEvent && pEventInfo->playingID == mSet16_Seq1PlayingID)
    {
	// The first sequence is done - play the second sequence.
	// NOTE: The transition between the two sequences won't be sample accurate.
	AK::SoundEngine::DynamicSequence::Play(mSet16_Seq2PlayingID);
	AK::SoundEngine::DynamicSequence::Close(mSet16_Seq2PlayingID);

	// End of Test Set 16. Wait a while and move on to Set 17.
	pDemoInstance->m_bTestInProgress = false;
	pDemoInstance->m_pNextFunction = &SceneDynamicDialogue::Set17_1_StartPlaybackWithCallback;
	pDemoInstance->Wait(5000);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// Initialize static member variables.
bool SceneDynamicDialogue::mSet17_bDonePlaying = false;
void* SceneDynamicDialogue::mSet17_CustomParams[] = { (void*)123, (void*)456, (void*)789, (void*)321, (void*)654, (void*)987 };

void SceneDynamicDialogue::Set17_1_StartPlaybackWithCallback()
{
    // Starting Test Set 17
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;
    mSet17_bDonePlaying = false;

    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(GAME_OBJECT_RADIO,
	AK_EndOfDynamicSequenceItem,
	&SceneDynamicDialogue::Set17_Callback,
	this);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID, 0, mSet17_CustomParams[0]);

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK_FALLBACK_ARGUMENTVALUE_ID,
	    AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID, 0, mSet17_CustomParams[1]);

	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID, 0, mSet17_CustomParams[2]);

	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::RESCUEHOSTAGE;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID, 0, mSet17_CustomParams[3]);

	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::DEFUSEBOMB;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID, 0, mSet17_CustomParams[4]);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID, 0, mSet17_CustomParams[5]);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }

    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Started playback with callback. Let the callback handle the rest.
}

void SceneDynamicDialogue::Set17_Callback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    SceneDynamicDialogue* pDemoInstance = (SceneDynamicDialogue*)in_pCallbackInfo->pCookie;

    if (in_eType == AK_EndOfDynamicSequenceItem && !mSet17_bDonePlaying)
    {
	// An item has finish playing so let's check the playlist content..
	AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(pDemoInstance->m_PlayingID);
	if (pPlaylist)
	{
	    // Check if the playlist is empty
	    if (pPlaylist->IsEmpty())
	    {
		mSet17_bDonePlaying = true;
		// End of Test Set 17. Wait a while and move on to Set 18.
		pDemoInstance->m_bTestInProgress = false;
		AK::SoundEngine::DynamicSequence::Close(pDemoInstance->m_PlayingID);
		pDemoInstance->m_pNextFunction = &SceneDynamicDialogue::Set18_1_StartPlayback;
		pDemoInstance->Wait(200);
	    }
	    else
	    {
		// The items remaining in the list should be the last item added
		// Let's validate using mSet17_CustomParams array.
		int playlistLength = pPlaylist->Length();
		int customParamsIndex = 6 - playlistLength;
		for (int i = 0; i < playlistLength; ++i, ++customParamsIndex)
		{
		    if (mSet17_CustomParams[customParamsIndex] != (*pPlaylist)[i].pCustomInfo)
		    {
			pDemoInstance->SetErrorMessage("Error: Params didn't match up!");
		    }
		}
	    }

	    AK::SoundEngine::DynamicSequence::UnlockPlaylist(pDemoInstance->m_PlayingID);
	}
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void SceneDynamicDialogue::Set18_1_StartPlayback()
{
    // Starting Test Set 18
    m_bTestInProgress = true;
    m_pNextFunction = NULL;
    ++m_uSetIndex;

    // Specifying DynamicSequenceType_NormalTransition prevents the next playlist item to be stopped with
    // the currently playing sound. If not specified, the sample accurate transition would have already prepared 
    // the next item to play and this item would be stopped as well.
    m_PlayingID = AK::SoundEngine::DynamicSequence::Open(
	GAME_OBJECT_RADIO,
	0,
	NULL,
	NULL,
	AK::SoundEngine::DynamicSequence::DynamicSequenceType_NormalTransition);

    AK::SoundEngine::DynamicSequence::Playlist* pPlaylist = AK::SoundEngine::DynamicSequence::LockPlaylist(m_PlayingID);
    if (pPlaylist)
    {
	AkArgumentValueID walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_IN;
	AkUniqueID audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AkArgumentValueID statusArguments[] = { AK::STATES::UNIT::STATE::UNIT_B,
	    AK_FALLBACK_ARGUMENTVALUE_ID,
	    AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED };

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::NEUTRALIZEHOSTILE;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::RESCUEHOSTAGE;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_A;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::DEFUSEBOMB;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::FAILED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	statusArguments[0] = AK::STATES::UNIT::STATE::UNIT_B;
	statusArguments[1] = AK::STATES::OBJECTIVE::STATE::DEFUSEBOMB;
	statusArguments[2] = AK::STATES::OBJECTIVESTATUS::STATE::COMPLETED;

	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::OBJECTIVE_STATUS, statusArguments, 3);
	pPlaylist->Enqueue(audioNodeID);

	walkieTalkieArgument = AK::STATES::WALKIETALKIE::STATE::COMM_OUT;
	audioNodeID = AK::SoundEngine::DynamicDialogue::ResolveDialogueEvent(AK::DIALOGUE_EVENTS::WALKIETALKIE, &walkieTalkieArgument, 1);
	pPlaylist->Enqueue(audioNodeID);

	AK::SoundEngine::DynamicSequence::UnlockPlaylist(m_PlayingID);
    }
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);

    // Playback has started. Wait a while and go to step 2.
    Wait(1000);
    m_pNextFunction = &SceneDynamicDialogue::Set18_2_PostPauseEvent;
}

void SceneDynamicDialogue::Set18_2_PostPauseEvent()
{
    m_pNextFunction = NULL;

    // Pause the dynamic dialogue using a Pause_All event
    AK::SoundEngine::PostEvent(AK::EVENTS::PAUSE_ALL, GAME_OBJECT_RADIO);

    // Wait a while and go to step 3.
    Wait(3000);
    m_pNextFunction = &SceneDynamicDialogue::Set18_3_PostResumeEvent;
}

void SceneDynamicDialogue::Set18_3_PostResumeEvent()
{
    m_pNextFunction = NULL;

    // Resume the dynamic dialogue playback using a Resume_All event
    AK::SoundEngine::PostEvent(AK::EVENTS::RESUME_ALL, GAME_OBJECT_RADIO);

    // Wait a while and go to step 4.
    Wait(2000);
    m_pNextFunction = &SceneDynamicDialogue::Set18_4_PostStopEvent;
}

void SceneDynamicDialogue::Set18_4_PostStopEvent()
{
    m_pNextFunction = NULL;

    // Stop the dynamic dialogue playback using a Stop_All event
    AK::SoundEngine::PostEvent(AK::EVENTS::STOP_ALL, GAME_OBJECT_RADIO);

    // Wait a while and go to step 5.
    Wait(2000);
    m_pNextFunction = &SceneDynamicDialogue::Set18_5_PlayRestOfSequence;
}

void SceneDynamicDialogue::Set18_5_PlayRestOfSequence()
{
    m_pNextFunction = NULL;

    // The rest of the sequence will play.
    AK::SoundEngine::DynamicSequence::Play(m_PlayingID);
    AK::SoundEngine::DynamicSequence::Close(m_PlayingID);
    m_PlayingID = 0;

    // All test sets finished. Wait a bit for playback to finish and reset the tests.
    m_bTestInProgress = false;
    m_pNextFunction = &SceneDynamicDialogue::StopAndResetTests;
    Wait(6500);
}

