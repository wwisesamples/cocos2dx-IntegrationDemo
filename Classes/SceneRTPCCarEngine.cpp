#include "SceneRTPCCarEngine.h"
#include "SceneMain.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine
#include "../WwiseProject/GeneratedSoundBanks/Wwise_IDs.h"		// IDs generated by Wwise
#include "WwiseWrapper.h"
#include "Platform.h"
#include "PlatformCocos.h"

USING_NS_CC;

#define  LOG_TAG    "SceneRTPCCarEngine"


SceneRTPCCarEngine::SceneRTPCCarEngine()
    :SceneBase("RTPC Demo (Car Engine)",
	"This page demonstrates the type of effects that can be "
	"produced by using an RTPC value. Press the \"Start "
	"Engine\" button to simulate the sound of an active car "
	"engine. Then select the \"RPM\" slider and press LEFT "
	"or RIGHT on the <<DIRECTIONAL_TYPE>> to change its "
	"value.\n\n"
	"The RPM slider is linked with an RTPC value associated "
	"with the engine sound. Modifying the value changes the "
	"properties on the sound object in real time.",
	SceneMain::createScene)
    , m_startEngineLabel(NULL)
    , m_RPMLabel(NULL)
    , m_slider(NULL)
{
}

SceneRTPCCarEngine::~SceneRTPCCarEngine()
{
}

Scene* SceneRTPCCarEngine::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneRTPCCarEngine::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneRTPCCarEngine::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneBase::init())
    {
	return false;
    }

    cocos2d::Size windowSize = Director::getInstance()->getWinSize();
    LOGAK("<SceneRTPCCarEngine> windowSize x:y =(%d:%d)", (int)windowSize.width, (int)windowSize.height);
    cocos2d::Size window_size = Director::getInstance()->getVisibleSize();
    LOGAK("<SceneRTPCCarEngine> windowVisibleSize x:y =(%d:%d)", (int)window_size.width, (int)window_size.height);
    //windowSize = window_size;

    float descriptionPosX = windowSize.width  * 0.50f;
    float selectButtonPosX = g_isLandscape ? windowSize.width * SCREEN_POS_SCALER_SELBUTTON_X : windowSize.height * SCREEN_POS_SCALER_SELBUTTON_X;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    {
	// for Start Engine button
	{
	    // SelectButton
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneRTPCCarEngine::onStartEngine, this));
	    addItem(selectItem, selectButtonPosX, windowSize.height / 2.00f + selectItem->getContentSize().height * 2.00f, this);
	    cocos2d::MenuItemLabel* pMenu = addLabelEx("Start Engine", selectItem->getPosition().x + selectItem->getContentSize().width/* * 2*/, selectItem->getPosition().y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneRTPCCarEngine::onStartEngine, this));
	    m_startEngineLabel = (Label*)pMenu->getChildren().at(0);
	}

	// Add Slider
	{
	    // 1000 - 10000
	    auto slider = cocos2d::ui::Slider::create();
	    slider->loadBarTexture("Slider_base.png");
	    slider->loadSlidBallTextures("Slider_button.png", "Slider_button.png", "");
	    slider->setScale9Enabled(true);
	    slider->setCapInsets(cocos2d::Rect(0, 0, 0, 0));
#if defined(AK_ANDROID) || defined(AK_IOS)
	    LOGAK("<SceneRTPCCarEngine> Slider size=(%d:%d) pos=(%d:%d)", (int)(windowSize.width*0.50f), (int)19, (int)(windowSize.width / 2.00f), (int)(windowSize.height *0.50f));
	    slider->setContentSize(cocos2d::Size(windowSize.height*0.50f/*250.0f*/, 19.00f)); 
#else
	    slider->setContentSize(cocos2d::Size(windowSize.width*0.800f/*250.0f*/, 19.00f));
#endif
	    slider->setPosition(Vec2(windowSize.width / 2.00f, windowSize.height *0.50f));
	    slider->addEventListener(CC_CALLBACK_2(SceneRTPCCarEngine::slideEvent, this));
	    slider->setPercent(50);
	    addChild(slider);
	    m_slider = slider;
	}

	// for RPM
	{
	    // SelectButton
	    cocos2d::MenuItemLabel* pMenu = addLabelEx("RPM:1000", descriptionPosX, windowSize.height *0.40f, FONT_SIZE_MENU, this, Vec2::ANCHOR_MIDDLE_BOTTOM);
	    m_RPMLabel = (Label*)pMenu->getChildren().at(0);
	    //cocos2d::Size sz = m_RPMLabel->getContentSize();
	    //m_RPMLabel->setPositionX(descriptionPosX /*+ (sz.width / 2)*/);
	}
    }

    // Load the sound bank
    AkBankID bankID; // Not used
    AKRESULT hr = AK::SoundEngine::LoadBank("Car.bnk", AK_DEFAULT_POOL_ID, bankID);
    if (hr != AK_Success)
    {
	SetLoadFileErrorMessage("Car.bnk");
	return false;
    }

    // Register the "Car" game object
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_CAR, "Car");

    // Initialize the page
    m_bPlayingEngine = false;

    scheduleUpdate();

    return true;
}

void SceneRTPCCarEngine::slideEvent(Ref* pSender, cocos2d::ui::Slider::EventType type) {
#ifdef ANDROID
    cocos2d::ui::Slider* slider = (cocos2d::ui::Slider*)(pSender);
#else
    auto slider = dynamic_cast<cocos2d::ui::Slider*>(pSender);
#endif
    int percent = slider->getPercent();
    // value should be between 1000 thru 10000
    int value = ((10000 - 1000) * percent / 100) + 1000;
    AK::SoundEngine::SetRTPCValue(AK::GAME_PARAMETERS::RPM, (AkRtpcValue)value, GAME_OBJECT_CAR);

    // update RPM value
    std::ostringstream stream;
    stream << "RPM:" << value << "";
    if (m_RPMLabel) m_RPMLabel->setString(stream.str());
}

void SceneRTPCCarEngine::onStartEngine(Ref* pSender)
{
    if (m_bPlayingEngine)
    {
	if (m_startEngineLabel) m_startEngineLabel->setString("Start Engine");
	AK::SoundEngine::PostEvent(AK::EVENTS::STOP_ENGINE, GAME_OBJECT_CAR);
    }
    else
    {
	if (m_startEngineLabel) m_startEngineLabel->setString("Stop Engine");
	AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_ENGINE, GAME_OBJECT_CAR);
    }

    m_bPlayingEngine = !m_bPlayingEngine;
}

void SceneRTPCCarEngine::menuCloseCallback(Ref* pSender)
{
    // Unregister the Car game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_CAR);

    // Unload the soundbank
    AK::SoundEngine::UnloadBank("Car.bnk", NULL);

    auto director = Director::getInstance();
    director->replaceScene(SceneMain::createScene());
    this->removeAllChildren();
}
void SceneRTPCCarEngine::onRelease()
{
    // Unregister the Car game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_CAR);

    // Unload the soundbank
    AK::SoundEngine::UnloadBank("Car.bnk", NULL);
    m_lastMenuIx = 1;
}

//void SceneRTPCCarEngine::update(float dt)
//{
//	WWISE::update();
//}

void SceneRTPCCarEngine::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event)
{
    int percent = m_slider->getPercent();

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	if (percent > 5)
	    percent -= 5;
	else
	    percent = 0;
	break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	if (percent < 95)
	    percent += 5;
	else
	    percent = 100;
	break;
 //   case EventKeyboard::KeyCode::KEY_ESCAPE:
	//menuCloseCallback(NULL);
	//return;
    default:
	SceneBase::onKeyPressed(keyCode, unused_event);
	return;
    }
    m_slider->setPercent(percent);
    slideEvent(m_slider, cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED);
}
