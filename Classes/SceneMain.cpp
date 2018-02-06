#include "SceneMain.h"
#include "Platform.h"
#include "PlatformCocos.h"
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include "SceneDialogueMenu.h"
#include "SceneRTPCCarEngine.h"
#include "SceneFootsteps.h"
#include "SceneMarkers.h"
#include "SceneMusicCallbacks.h"
#include "SceneInteractiveMusic.h"
#include "WwiseWrapper.h"
#if defined INTEGRATIONDEMO_MICROPHONE
  #include "SceneMicrophone.h"
#endif
#include "ScenePositioning.h"
#include "SceneExternalSources.h"
//#if defined INTEGRATIONDEMO_BGM
  #include "SceneBGM.h"
//#endif


extern int g_menu_ix;

USING_NS_CC;
namespace
{
    template <typename Layer>
    void replaceScene(Ref*) {
	Director::getInstance()->replaceScene(Layer::createScene());
    }
}

SceneMain::SceneMain()
: SceneBase("Wwise Integration Demo", 
    "Welcome to the Wwise Integration Demonstration! This "
    "application contains samples that demonstrate how to "
    "integrate various Wwise features and concepts into a "
    "game.\n\n"
    "Navigation:\n"
    "  - UP / DOWN on the <<DIRECTIONAL_TYPE>>: move between items on the page.\n"
    "  - <<ACTION_BUTTON>>: activate the selected item.\n"
    "  - <<BACK_BUTTON>>: go back a page.\n\n"
    "Controls:\n"
    "  Certain controls (toggles, sliders) allow you to "
    "change values. Press LEFT/RIGHT on the <<DIRECTIONAL_TYPE>> "
    "to change these controls' values.\n\n"
    "Pressing <<HELP_BUTTON>> at any time will display help for the current page.\n",
    NULL)
{
    m_menuix = g_menu_ix;
}

Scene* SceneMain::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneMain::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneMain::init()
{
    //////////////////////////////
    // 1. super init first
    //if (!Layer::init())
    if (!SceneBase::init())
    {
	return false;
    }

    cocos2d::Size windowSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float descriptionPosY = windowSize.height * SCREEN_POS_SCALER_DESCRIPTION_Y;
    float selectButtonPosX = windowSize.height * SCREEN_POS_SCALER_SELBUTTON_X;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    //Menu* menu;
    {
	////////////////////////////////////////////
	// --> MOVED to SceneBase.cpp
	//
	//auto addItem = [&](MenuItem* item, const float& posx, const float& posy) {
	//    item->setPosition(cocos2d::Point(posx, posy));
	//    menu = Menu::create(item, NULL);
	//    menu->setPosition(Vec2::ZERO);
	//    this->addChild(menu, 1);
	//};
	//auto addLabelEx = [&](const std::string& text, const float& posx, const float& posy, const int& size) {
	//    auto playLabel = Label::createWithSystemFont(text, "Arial", size, cocos2d::Size(0, 0),
	//	TextHAlignment::LEFT, TextVAlignment::CENTER);
	//    auto playItem = MenuItemLabel::create(playLabel);
	//    int y = posy - playLabel->getContentSize().height * 1;
	//    playItem->setPosition(cocos2d::Point(posx, posy));
	//    auto pmenu = Menu::create(playItem, NULL);
	//    pmenu->setPosition(cocos2d::Point::ZERO);
	//    this->addChild(pmenu, 1);
	//    playItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//    m_menu.push_back(playItem);
	//    return playItem;
	//};
	// <--

	int x = 0, ix=0;
	int y = descriptionPosY;

	// Add button linking to the Dialogue Demos menu
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneDialogueMenu>);
            ix = selectItem->getContentSize().height;
            y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Dialogue Demos", selectItem->getPosition().x + selectItem->getContentSize().width , selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneDialogueMenu>);
	    m_funcEnt.push_back(SceneDialogueMenu::createScene);
	}

	// Add button linking to RTPC Demo (Car Engine)
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneRTPCCarEngine>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("RTPC Demo (Car Engine)", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneRTPCCarEngine>);
	    m_funcEnt.push_back(SceneRTPCCarEngine::createScene);
	}

	// Add button linking to Footsteps demo
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneFootsteps>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Footsteps Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneFootsteps>);
	    m_funcEnt.push_back(SceneFootsteps::createScene);
	}

	// Add button linking to Markers demo
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneMarkers>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Subtitles/Markers Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneMarkers>);
	    m_funcEnt.push_back(SceneMarkers::createScene);
	}

	// Add button linking to Music Callbacks demo
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneMusicCallbacksRoot>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Music Callbacks Demo", selectItem->getPosition().x + selectItem->getContentSize().width , selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneMusicCallbacksRoot>);
	    m_funcEnt.push_back(SceneMusicCallbacksRoot::createScene);
	}

	// Add button linking to Interactive Music demo
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneInteractiveMusic>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Interactive Music Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneInteractiveMusic>);
	    m_funcEnt.push_back(SceneInteractiveMusic::createScene);
	}

#undef AK_MOTION // for now...
#if defined AK_MOTION
	// Add button linking to Motion demo
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneMain::onNotImplemented, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Motion Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(CC_CALLBACK_1(SceneBase::onNotImplemented, this));
	    m_funcEnt.push_back(SceneBase::NotImplemented);
	}
#endif //defined AK_MOTION

#if defined INTEGRATIONDEMO_MICROPHONE
#ifdef AK_IOS
	if (g_bEnableMicDemo)
	{
#endif // #if AK_IOS
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneMicrophone>);  
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Microphone Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneMicrophone>);
	    m_funcEnt.push_back(SceneMicrophone::createScene);
#ifdef AK_IOS
	}
#endif // #if AK_IOS
#endif // defined INTEGRATIONDEMO_MICROPHONE

	// Positioning
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<ScenePositioning>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Positioning Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<ScenePositioning>);
	    m_funcEnt.push_back(ScenePositioning::createScene);
	}

	// External Sources 
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneExternalSources>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("External Sources Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneExternalSources>);
	    m_funcEnt.push_back(SceneExternalSources::createScene);
	    x = selectItem->getPosition().x + selectItem->getContentSize().width;
//	    y -= selectItem->getContentSize().height;
	}

#if defined AK_PS3
	// 
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneMain::onNotImplemented, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y);
	    auto label = addLabelEx("RSX Demo", selectItem->getPosition().x + selectItem->getContentSize().width * 5, selectItem->getPosition().y, FONT_SIZE_MENU);
	    label->setCallback(CC_CALLBACK_1(SceneMain::onNotImplemented, this));
	    m_funcEnt.push_back(SceneBase::NotImplemented);
	}
#endif

//#undef INTEGRATIONDEMO_BGM // disable for now.... hideo
//#if defined INTEGRATIONDEMO_BGM
	//
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneBGM>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Background Music/DVR Demo", 
		selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
		//selectItem->getPosition().x + selectItem->getContentSize().width * 5, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneBGM>);
	    m_funcEnt.push_back(SceneBGM::createScene);
	    y -= selectItem->getContentSize().height;
	}
//#endif

	// Exit button and label
	{
	    auto selectItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(SceneMain::menuCloseCallback, this));
#ifdef AK_APPLE
	    y = y - selectItem->getContentSize().height  - ix;
#endif
	    addItem(selectItem, selectButtonPosX, y, this);
#ifdef AK_APPLE
	    auto label = addLabelEx("Exit", x, y - (FONT_SIZE_MENU / 2), FONT_SIZE_MENU, this);
#else
	    auto label = addLabelEx("Exit", x, y, FONT_SIZE_MENU, this);
#endif
	    label->setCallback(CC_CALLBACK_1(SceneMain::onDoExit, this));
	    m_funcEnt.push_back(SceneMain::DoExit);
	}
	
	setMenuColor(m_lastMenuIx);
    }

    return true;
}

void SceneMain::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    //SceneBase::onKeyPressed(keyCode, event);
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_KP_ENTER:
	{
	    g_menu_ix = m_lastMenuIx;
	    if ((int)m_funcEnt.size()>m_lastMenuIx)
	    {
		auto scene = m_funcEnt[m_lastMenuIx]();
		if (scene) {
		    Director::getInstance()->replaceScene((cocos2d::Scene *)scene);
		    this->removeAllChildren();
		}
	    } else {
		onNotImplemented(NULL);
	    }
	}
	break;

    default:
		SceneBase::onKeyPressed(keyCode, event);
	return;
    }
}

cocos2d::Scene* SceneMain::NotImplemented()
{
    cocos2d::MessageBox("Sorry, not implemented yet.", "Error");
    return NULL;
}

void SceneMain::onNotImplemented(cocos2d::Ref* pSender)
{
    ((SceneMain*)pSender)->NotImplemented();
}

////////////
// Exit
cocos2d::Scene* SceneMain::DoExit()
{
#if defined AK_IOS
    WWISE::terminate();
#endif
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    return NULL;
}

void SceneMain::menuCloseCallback(Ref* pSender)
{
    ((SceneMain*)pSender)->DoExit();
}

void SceneMain::onDoExit(cocos2d::Ref* pSender)
{
    menuCloseCallback(NULL);
}

