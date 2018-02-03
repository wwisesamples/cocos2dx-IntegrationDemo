#include "SceneMicrophone.h"
#include "SceneMain.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine
#include "WwiseWrapper.h"
#include "Platform.h"
#include "PlatformCocos.h"
#include "SoundInputMgr.h"

#define  LOG_TAG    "SceneMicrophone"

USING_NS_CC;

//The game object ID used for this demo, it can be anything (except 0 and 1, as stated in the RegisterGameObj() doc)
#define MICRO_GAME_OBJECT 1234

SceneMicrophone::SceneMicrophone()
: SceneBase("Microphone Demo",
    "This demo shows how to use use the AudioInput plugin "
    "to feed the microphone data to Wwise.  Simply play the "
    "microphone event.  There is a delay effect that you "
    "can enable or disable to help distinguish between the "
    "real sound and the recording.", SceneMain::createScene)
{
    m_bPlaying = false;
    m_bDelayed = false;
    m_pInput = &SoundInput::Instance();
    m_pLabelStart = m_pLabelDelay = NULL;
}

SceneMicrophone::~SceneMicrophone()
{
    if (m_pInput)
    {
	m_pInput->InputOff();
    }
}

Scene* SceneMicrophone::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneMicrophone::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneMicrophone::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneBase::init())
    {
	return false;
    }
    cocos2d::Size windowSize = Director::getInstance()->getWinSize();
#if defined(AK_ANDROID) || defined(AK_APPLE)
    float descriptionPosX = windowSize.width  * 0.50f;
    float descriptionPosY = windowSize.height * 0.90f;
    float selectButtonPosX = windowSize.height * 0.50f;
#else
    float descriptionPosX = windowSize.width  * 0.50f;
    float descriptionPosY = windowSize.height * 0.90f;
    float selectButtonPosX = windowSize.width  * 0.15f;
#endif // AK_ANDROID


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    {

	//addLabel("Localization Demos Menu", descriptionPosX, descriptionPosY, 30);
	int y = descriptionPosY;

	// Add button linking to the Say "Hello"
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneMicrophone::onStartRecord, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    cocos2d::MenuItemLabel* pMenu = addLabelEx("Start Recording", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneMicrophone::onStartRecord, this));
	    m_pLabelStart = (Label*)((MenuItemLabel*)pMenu)->getChildren().at(0);
	}

	// Add button linking to RTPC Demo (Car Engine)
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneMicrophone::onDelay, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    cocos2d::MenuItemLabel* pMenu = addLabelEx("Enable Delay", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneMicrophone::onDelay, this));
	    m_pLabelDelay = (Label*)((MenuItemLabel*)pMenu)->getChildren().at(0);
	}
    }

    if (!SoundInputMgr::Instance().Initialize())
    {
	SetErrorMessage(" Unable to Initialize Microphones.");
	return false;
    }

    // Load the sound bank
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("Microphone.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetErrorMessage("Microphone.bnk");
	return false;
    }

    // Register the "Micro" game object on which the sound recorded from the micro will be played
    AK::SoundEngine::RegisterGameObj(MICRO_GAME_OBJECT, "Micro");

    scheduleUpdate();

    return true;
}

void SceneMicrophone::onRelease()
{
    if (m_bPlaying)
	StopRecording();

    SoundInputMgr::Instance().Term();

    // Unregister the "Micro" game object
    AK::SoundEngine::UnregisterGameObj(MICRO_GAME_OBJECT);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("Microphone.bnk", NULL);
    m_lastMenuIx = 6;
}

void SceneMicrophone::onStartRecord(cocos2d::Ref* pSender)
{
    //cocos2d::Label * pLabel = (Label*)((MenuItemLabel*)pSender)->getChildren().at(0);

    if (!m_bPlaying)
    {
	if (m_pInput && m_pInput->InputOn(/*We only support one microphone*/))
	{
	    m_pLabelStart->setString("Stop Recording");
	    StartRecording();
	}else{
	    SetErrorMessage("Could not start recording of microphone");
	    return;
	}
    } else {
	m_pLabelStart->setString("Start Recording");
	StopRecording();
    }

}

void SceneMicrophone::onDelay(cocos2d::Ref* pSender)
{
    //cocos2d::Label * pLabel = (Label*)((MenuItemLabel*)pSender)->getChildren().at(0);
    if (!m_bDelayed)
    {
	m_bDelayed = true;
	m_pLabelDelay->setString("Disable Delay");
	AK::SoundEngine::PostEvent(AK::EVENTS::ENABLE_MICROPHONE_DELAY, MICRO_GAME_OBJECT);
    }   else   {
	m_bDelayed = false;
	m_pLabelDelay->setString("Enable Delay");
	AK::SoundEngine::PostEvent(AK::EVENTS::DISABLE_MICROPHONE_DELAY, MICRO_GAME_OBJECT);
    }
}

void SceneMicrophone::StartRecording()
{
    m_bPlaying = true;

    AkPlayingID playId = AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_MICROPHONE, MICRO_GAME_OBJECT);
    m_pInput->SetPlayingID(playId);
}

void SceneMicrophone::StopRecording()
{
    m_bPlaying = false;

    AK::SoundEngine::PostEvent(AK::EVENTS::STOP_MICROPHONE, MICRO_GAME_OBJECT);

    // Call RenderAudio() here to tell the sound engine to process the STOP_MICROPHONE Event before.
    // InpufOff will be called.
    // Not calling it could cause a plugin to be terminated by the call to input Off, and could 
    // result into Notifications of the plug-in to have exited abnormally.
    AK::SoundEngine::RenderAudio();

    if (m_pInput)
	m_pInput->InputOff();
}
