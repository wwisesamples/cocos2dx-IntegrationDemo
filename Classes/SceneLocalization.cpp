#include "SceneLocalization.h"
#include "SceneMain.h"
#include "SceneDialogueMenu.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>    // Sound engine
#include "WwiseWrapper.h"
#include "Platform.h"
#include "PlatformCocos.h"

#define  LOG_TAG    "SceneLocalization"

USING_NS_CC;

SceneLocalization::SceneLocalization()
: SceneBase("Localization Demos Menu",
    "This page shows how to implement localization by "
    "unloading and reloading language-specific SoundBanks.\n\n"
    "Use the \"Language\" control to toggle between the "
    "available languages, then press the \"Say Hello\" "
    "item to hear a greeting in the selected language.",
    SceneDialogueMenu::createScene)
, m_language(NULL)
{
}

SceneLocalization::~SceneLocalization()
{
}

Scene* SceneLocalization::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneLocalization::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneLocalization::init()
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

	//addLabel("Localization Demos Menu", descriptionPosX, descriptionPosY, 30);
	int y = descriptionPosY;

	// Add button linking to the Say "Hello"
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneLocalization::onSayHello, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    addLabelEx("Say \"Hello\"", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneLocalization::onSayHello, this));
	}

	// Add button linking to RTPC Demo (Car Engine)
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneLocalization::onLanguageChanged, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    cocos2d::MenuItemLabel* pMenu = addLabelEx("Language:English(US)", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneLocalization::onLanguageChanged, this));
	    m_language = (Label*)pMenu->getChildren().at(0);
	}
    }

    // Load the sound bank
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("Human.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetErrorMessage("Human.bnk");
	return false;
    }
    LOGAK("<SceneLocalization::init>3");

    // Register the "Human" game object
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_HUMAN, "Human");
    scheduleUpdate();

    return true;
}

void SceneLocalization::onRelease()
{
    // Reset the language to English(US), in case it was changed
    AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

    // Unregister the "Human" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_HUMAN);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("Human.bnk", NULL);
}

void SceneLocalization::menuCloseCallback(Ref* pSender)
{
    // Reset the language to English(US), in case it was changed
    AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

    // Unregister the "Human" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_HUMAN);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("Human.bnk", NULL);

    auto director = Director::getInstance();
    director->replaceScene(SceneDialogueMenu::createScene());
    this->removeAllChildren();
}

void SceneLocalization::onSayHello(cocos2d::Ref* pSender)
{
    LOGAK("<SceneLocalization::onSayHello>");
    AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_HELLO, GAME_OBJECT_HUMAN);
}

void SceneLocalization::onLanguageChanged(cocos2d::Ref* pSender)
{
    // Set global language. Through the Low-Level I/O's file location resolver, 
    // the sound bank file will be opened in <BasePath>/<Language>/.
    string szLang, str;

    if (m_language)
	str = m_language->getString();
    else
	return;

    if (str == "Language:English(US)"){
	szLang = "French(Canada)";
	m_language->setString("Language:French(Canada)");
    }
    else if (str == "Language:French(Canada)") {
	szLang = "English(US)";
	m_language->setString("Language:English(US)");
    }
    else
	return;

    AkOSChar* pOsCharStr;
    CONVERT_CHAR_TO_OSCHAR(szLang.c_str(), pOsCharStr);

    if (AK::StreamMgr::SetCurrentLanguage(pOsCharStr) != AK_Success)
    {
	SetErrorMessage("Error: Invalid language.");
    }

    // Reload the sound bank
    AK::SoundEngine::UnloadBank("Human.bnk", NULL);
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("Human.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetLoadFileErrorMessage("Human.bnk");
    }
}

//void LocalizationScene::update(float dt)
//{
//    WWISE::update();
//}


