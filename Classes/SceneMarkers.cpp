#include "SceneMarkers.h"
#include "SceneMain.h"
#include "WwiseWrapper.h"
#include "Helpers.h"
#ifdef ANDROID
#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	95
#endif
#include "Platform.h"
#include "PlatformCocos.h"

/////////////////////////////////////////////////////////////////////
// DemoMarkers Constant Member Initialization
/////////////////////////////////////////////////////////////////////

// We start writing the text at 1/4 the width of the screen. We want
// a 1/8 margin on the right, so let's wrap at 5/8 of the width:
#define DEMO_MARKERS_MAX_CAPTION_WIDTH (INTEGRATIONDEMO_SCREEN_CHAR_WIDTH * 5 / 8 )

const char* SceneMarkers::SUBTITLES[] =
{
    "",
    "In this tutorial...",
    "...we will look at creating...",
    "...actor-mixers...",
    "...and control buses.",
    "We will also look at the...",
    "...actor-mixer and master-mixer structures...",
    "...and how to manage these structures efficiently."
};


USING_NS_CC;

SceneMarkers::SceneMarkers()
: SceneBase("Subtitles/Markers Demo",
    "This demo shows how to use markers in WAV files to "
    "synchronize subtitles with the audio track. Press the "
    "\"Play Markers\" button to hear the sound and see "
    "markers as they are triggered.",
    SceneMain::createScene)
, m_bPlayingEngine(false)
{
    m_label[0] = m_label[1] = m_label[2] = NULL;

    for (int i = 0; i < (NUM_LINES + 1); i++)
    {
	m_subtitles[i] = IntegrationDemoHelpers::WordWrapString(SUBTITLES[i], DEMO_MARKERS_MAX_CAPTION_WIDTH);
    }
}

SceneMarkers::~SceneMarkers()
{
}

Scene* SceneMarkers::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create(); 
    // 'layer' is an autorelease object
    auto layer = SceneMarkers::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneMarkers::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneBase::init())
    {
	return false;
    }

    cocos2d::Size windowSize = Director::getInstance()->getWinSize();

    float descriptionPosX = windowSize.width  * 0.50f;
    float descriptionPosY = windowSize.height * 0.80f;
    float selectButtonPosX = g_isLandscape ? windowSize.width * 0.30f : windowSize.height * 0.50f;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    {
	int y = 0, yy=0;
	// for "Play Markers" button
	{
	    // SelectButton

	    MenuItemLabel* label = addLabelEx("Play Markers", 0, 0, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneMarkers::onPlayMarkersButton, this));
	    m_label[0] = (Label*)label->getChildren().at(0);
	    y = descriptionPosY - m_label[0]->getContentSize().height * 2;
	    label->setPosition(selectButtonPosX, y);

	    // Add text labels 
	    yy = m_label[0]->getContentSize().height;
	    y -= yy*2;
	    m_label[1] = addLabel(" ", descriptionPosX, y, FONT_SIZE_MENU/*22*/, this, Vec2::ANCHOR_MIDDLE);
	    y -= yy*2;
	    m_label[2] = addLabel(" ", descriptionPosX, y, FONT_SIZE_MENU-2/*18*/, this, Vec2::ANCHOR_MIDDLE);
	    m_label[2]->setMaxLineWidth(windowSize.width - y);
	}
    }


    // Load the sound bank
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("MarkerTest.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetLoadFileErrorMessage("MarkerTest.bnk");
	return false;
    }

    // Register the "Markers" game object
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_NARRATOR, "Narrator");

    // Initialize the marker playback variables
    m_unCurrentLine = 0;
    m_bPlayingMarkers = false;

    scheduleUpdate();

    return true;
}

void SceneMarkers::onPlayMarkersButton(Ref* pSender)
{
    if (!m_bPlayingMarkers)
    {
	m_bPlayingMarkers = true;
	m_label[0]->setString("--Playing--");

	// Post the PLAY_MARKERS_TEST event and ask for marker event notifications
	m_iPlayingID = AK::SoundEngine::PostEvent(
	    AK::EVENTS::PLAY_MARKERS_TEST,
	    GAME_OBJECT_NARRATOR,
	    AK_EndOfEvent | AK_Marker | AK_EnableGetSourcePlayPosition,
	    &SceneMarkers::MarkersCallback,
	    this);
    }
}

void SceneMarkers::menuCloseCallback(Ref* pSender)
{
    m_bPlayingMarkers = false;

    // Make sure we do not get called back after page destruction
    AK::SoundEngine::CancelEventCallback(m_iPlayingID);

    // Stop the sound playing
    AK::SoundEngine::StopPlayingID(m_iPlayingID);

    // Unregister the "Markers" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_NARRATOR);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("MarkerTest.bnk", NULL);

    auto director = Director::getInstance();
    director->replaceScene(SceneMain::createScene());
    this->removeAllChildren();
}

void SceneMarkers::onRelease()
{
    m_lastMenuIx = 3;
    m_bPlayingMarkers = false;

    // Make sure we do not get called back after page destruction
    AK::SoundEngine::CancelEventCallback(m_iPlayingID);

    // Stop the sound playing
    AK::SoundEngine::StopPlayingID(m_iPlayingID);

    // Unregister the "Markers" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_NARRATOR);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("MarkerTest.bnk", NULL);
}

void SceneMarkers::update(float dt)
{
    SceneBase::update(dt);
    if (m_bPlayingMarkers)
    {
	AkTimeMs uPosition;
	char strBuf[50];

	// Get the current play position and store it in a string buffer
	AK::SoundEngine::GetSourcePlayPosition(m_iPlayingID, &uPosition);
	snprintf(strBuf, 50, "Time: %d ms", (int)uPosition);

	// Draw the play position and subtitles
	m_label[1]->setString(strBuf);
	m_label[2]->setString(m_subtitles[m_unCurrentLine]);
    }

}

void SceneMarkers::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event)
{

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	break;
 //   case EventKeyboard::KeyCode::KEY_ESCAPE:
	//menuCloseCallback(NULL);
	//return;

    default:
	SceneBase::onKeyPressed(keyCode, unused_event);
	return;
    }
}

void SceneMarkers::MarkersCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    SceneMarkers* pPage = (SceneMarkers*)in_pCallbackInfo->pCookie;
    if (in_eType == AK_Marker)
    {
	AkMarkerCallbackInfo* pMarkerCallbackInfo = (AkMarkerCallbackInfo*)in_pCallbackInfo;

	pPage->m_unCurrentLine = pMarkerCallbackInfo->uIdentifier;
	if (pPage->m_unCurrentLine >= IntegrationDemoHelpers::AK_ARRAYSIZE(SceneMarkers::SUBTITLES))
	{
	    pPage->m_unCurrentLine = 0;
	}
    }
    else if (in_eType == AK_EndOfEvent)
    {
	pPage->m_label[0]->setString("Play Markers");
	pPage->m_bPlayingMarkers = false;
	pPage->m_unCurrentLine = 0;
    }
    else
    {
	pPage->SetErrorMessage("Unsupported event occurred");
    }
}
