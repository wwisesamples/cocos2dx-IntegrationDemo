#include "WwiseWrapper.h"
#include "SceneBase.h"
#include "SceneMain.h"
#include "SceneRTPCCarEngine.h"
#include "Platform.h"
#include "PlatformCocos.h"
#include <AK/Tools/Common/AkPlatformFuncs.h>

#define  LOG_TAG    "SceneBase"

extern int g_menu_ix;

USING_NS_CC;

namespace
{
    template <typename Layer>
    void replaceScene(Ref*) {
	Director::getInstance()->replaceScene(Layer::createScene());
    }
}

//////////////////////////////
// initialize static members
string SceneBase::m_szError = "";
int SceneBase::m_lastMenuIx = 0;;


// on "init" you need to initialize your instance
bool SceneBase::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Layer::init())
    {
	return false;
    }

    cocos2d::Size windowSize = Director::getInstance()->getWinSize();
    LOGAK("<ScenBase> windowSize x:y =(%d:%d)", (int)windowSize.width, (int)windowSize.height);
    float descriptionPosX = windowSize.width  * 0.50f;
#if defined(AK_APPLE)
    float descriptionPosY = windowSize.height * 0.95f;
#else
    float descriptionPosY = windowSize.height * 0.90f;
#endif    
     
    // Menu title
    {
	if (!m_szPageTitle.empty())
	  addLabel(m_szPageTitle, descriptionPosX, descriptionPosY, FONT_SIZE_TITLE, this/*, TextHAlignment::CENTER*/, Vec2::ANCHOR_MIDDLE);
    }

    // Press F1/START for help...
#if !defined(AK_ANDROID) && !defined(AK_IOS)
    {
	auto label = addLabel("(Press F1/START for help...)", 0, 0, FONT_SIZE_MENU, this, Vec2::ANCHOR_BOTTOM_RIGHT);
	label->setPosition(windowSize.width - label->getContentSize().width, 0);
    }
#endif // AK_ANDROID

    // Scene back to parent Button
    {
	if (m_funcMainScene) {
	    auto *item = MenuItemImage::create(
		"SELECT_SCENE_Normal.png",
		"SELECT_SCENE_Push.png",
		CC_CALLBACK_1(SceneBase::menuCloseCallback, this));
	    if (item){
#if defined(AK_APPLE)
  #if defined(AK_IOS)
                item->setPosition(cocos2d::Point(windowSize.width / 2.00f, item->getContentSize().height / 2.00f));
  #else
                item->setPosition(cocos2d::Point(windowSize.width - (item->getContentSize().height * 2.0f),
                                                 item->getContentSize().height * 3.0f));
  #endif
#else
		item->setPosition(cocos2d::Point(windowSize.width / 2.00f, item->getContentSize().height));
#endif	      
		auto menu = Menu::create(item, NULL);
		menu->setPosition(cocos2d::Point::ZERO);
		this->addChild(menu, 1);
	    }
	}
    }


    if (!m_key_listener){
	m_key_listener = EventListenerKeyboard::create();
	m_key_listener->onKeyPressed = CC_CALLBACK_2(SceneBase::onKeyPressed, this);
	m_key_listener->onKeyReleased = CC_CALLBACK_2(SceneBase::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_key_listener, this);
    }
    return true;
}

void SceneBase::SetLoadFileErrorMessage(const char* fileName)
{
    char errorMessage[INTEGRATIONDEMO_MAX_MESSAGE];
#if defined (AK_IOS) || defined (AK_MAC_OS_X)
    char soundBankPath[AK_MAX_PATH];
    snprintf(soundBankPath, AK_MAX_PATH, "%s", SOUND_BANK_PATH);
    sprintf(errorMessage, "Failed to load file \"%s\" under Soundbank path: \n\n%s", fileName, soundBankPath);
#else
    char* soundBankPath = NULL;
    CONVERT_OSCHAR_TO_CHAR(SOUND_BANK_PATH, soundBankPath);
    sprintf(errorMessage, "Failed to load file \"%s\" under Soundbank path: \n\n%s", fileName, soundBankPath);
#endif //#if defined (AK_IOS) || defined (AK_MAC_OS_X)
    SetErrorMessage(errorMessage);
}

void SceneBase::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    LOGAK("<onKeyPressed> keyCode:%d", keyCode);
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_F1:
    case EventKeyboard::KeyCode::KEY_MENU:  // for Android
	if (m_szHelp.empty())
	    cocos2d::MessageBox("No help is available for this page", "Help");
	else
	    cocos2d::MessageBox(m_szHelp.c_str(), "Help");
	break;

    case EventKeyboard::KeyCode::KEY_ESCAPE:
	menuCloseCallback(NULL);
	return;

    case EventKeyboard::KeyCode::KEY_UP_ARROW:
	if (m_lastMenuIx > 0) m_lastMenuIx--;
	setMenuColor(m_lastMenuIx);
	break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	if (m_lastMenuIx < (int)m_menu.size()) m_lastMenuIx++;
	setMenuColor(m_lastMenuIx);
	break;
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
	}
	else {
	    onNotImplemented(NULL);
	}
    }
    break;

    default:
	//SceneBase::onKeyPressed(keyCode, event);
	return;
    }
}

/*
void SceneBase::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_F1:
	if (m_szHelp.empty())
	    cocos2d::MessageBox("No help is available for this page", "Help");
	else
	    cocos2d::MessageBox(m_szHelp.c_str(), "Help");
	break;
    case EventKeyboard::KeyCode::KEY_ESCAPE:
	menuCloseCallback(NULL);
	return;
    default:
	return;
    }
}*/

void SceneBase::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event)
{
}

void SceneBase::update(float dt)
{
    Wwise::Instance().Update();
    ++m_iTickCount;
}

void SceneBase::menuCloseCallback(cocos2d::Ref* pSender)
{
    onRelease();
    if (m_funcMainScene) {
	Director::getInstance()->replaceScene(m_funcMainScene());
	this->removeAllChildren();
    }
}

void SceneBase::onRelease()
{
}

void SceneBase::addItem(cocos2d::MenuItem* item, const float posx, const float posy, SceneBase* pScence)
{
    item->setPosition(cocos2d::Point(posx, posy));
	CCASSERT(item->getParent() == nullptr, "child already added. It can't be added again");
    Menu *menu = Menu::create(item, NULL);
    menu->setPosition(Vec2::ZERO);
    pScence->addChild(menu, 1);
};

cocos2d::Label* SceneBase::addLabel(const std::string text, const float posx, const float posy, const int size,  SceneBase* pScence,
    const cocos2d::Vec2 point)
{
    cocos2d::TTFConfig ttfConfig("fonts/arial.ttf", size);
    auto playLabel = cocos2d::Label::createWithTTF(ttfConfig, text);
    playLabel->setPosition(cocos2d::Point(posx, posy));
    playLabel->setAnchorPoint(point/*Vec2::ANCHOR_BOTTOM_LEFT*/);
    pScence->addChild(playLabel, 1);
    return playLabel;
};


cocos2d::MenuItemLabel* SceneBase::addLabelEx(const std::string text, const float posx, const float posy, const int size,  SceneBase* pScence,
    const cocos2d::Vec2 point/* = cocos2d::Vec2::ANCHOR_BOTTOM_LEFT*/)
{
    cocos2d::TTFConfig ttfConfig("fonts/arial.ttf", size);
    auto playLabel = cocos2d::Label::createWithTTF(ttfConfig, text);
    auto playItem = cocos2d::MenuItemLabel::create(playLabel);
    playItem->setPosition(cocos2d::Point(posx, posy));
    auto pmenu = Menu::create(playItem, NULL);
    pmenu->setPosition(cocos2d::Point::ZERO);
    pScence->addChild(pmenu, 1);
    playItem->setAnchorPoint(point);
    m_menu.push_back(playItem);
    return playItem;
};

cocos2d::MenuItemLabel* SceneBase::addLabelEx(const std::string text, const float posx, const float posy, const int size,  SceneBase* pScence, const cocos2d::ccMenuCallback& callback,
    const cocos2d::TextHAlignment hAlignment /* = TextHAlignment::CENTER */, const cocos2d::Vec2 point/* = cocos2d::Vec2::ANCHOR_BOTTOM_LEFT*/)
{
    cocos2d::TTFConfig ttfConfig("fonts/arial.ttf", size);
    auto playLabel = cocos2d::Label::createWithTTF(ttfConfig, text, hAlignment);
    auto playItem  = cocos2d::MenuItemLabel::create(playLabel, callback);
    playItem->setPosition(cocos2d::Point(posx, posy));
    auto pmenu = cocos2d::Menu::create(playItem, NULL);
    pmenu->setPosition(cocos2d::Point::ZERO);
    pScence->addChild(pmenu, 1);
    playItem->setAnchorPoint(point);
    pScence->m_menu.push_back(playItem);
    return playItem;
}


void SceneBase::setMenuColor(int index)
{
    for (int i = 0; i < (int)m_menu.size(); i++)
	((cocos2d::Label*)m_menu[i]->getLabel())->setTextColor(i == index ? cocos2d::Color4B::YELLOW : cocos2d::Color4B::WHITE);
}

cocos2d::Scene* SceneBase::NotImplemented()
{
    cocos2d::MessageBox("Sorry, not implemented yet.", "Error");
    return NULL;
}

void SceneBase::onNotImplemented(cocos2d::Ref* pSender)
{
    ((SceneMain*)pSender)->NotImplemented();
}
