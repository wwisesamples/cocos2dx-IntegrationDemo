#include "SceneMain.h"
#include "SceneDialogueMenu.h"
#include "SceneLocalization.h"
#include "SceneDynamicDialogue.h"
#include "Platform.h"
#include "PlatformCocos.h"

USING_NS_CC;

namespace
{
    template <typename Layer>
    void replaceScene(Ref*) {
	Director::getInstance()->replaceScene(Layer::createScene());
    }
}


SceneDialogueMenu::SceneDialogueMenu()
: SceneBase("Dialogue Demos Menu", "", SceneMain::createScene)
{
}

SceneDialogueMenu::~SceneDialogueMenu()
{
}

void SceneDialogueMenu::onRelease()
{
    m_lastMenuIx = 0;
}


Scene* SceneDialogueMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneDialogueMenu::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneDialogueMenu::init()
{
    //////////////////////////////
    // 1. super init first
    //if (!Layer::init())
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
    //Menu* menu;
    {
/*
	auto addItem = [&](MenuItem* item, const float& posx, const float& posy){
	    item->setPosition(cocos2d::Point(posx, posy));
	    menu = Menu::create(item, NULL);
	    menu->setPosition(Vec2::ZERO);
	    this->addChild(menu, 1);
	};

	auto addLabel = [&](const std::string& text, const float& posx, const float& posy, const int& size){
	    auto label = Label::createWithSystemFont(text, "Arial", size, cocos2d::Size(0,0),
		TextHAlignment::LEFT, TextVAlignment::CENTER);
	    auto playItem = MenuItemLabel::create(label);
	    //label->setPosition(cocos2d::Point(posx, posy));
	    //this->addChild(label, 1);

	    int y = posy - label->getContentSize().height * 1;
	    playItem->setPosition(cocos2d::Point(posx, posy));
	    auto pmenu = Menu::create(playItem, NULL);
	    pmenu->setPosition(cocos2d::Point::ZERO);
	    this->addChild(pmenu, 1);
	    m_menu.push_back(playItem);
	    return playItem;
	};
*/
	int y = descriptionPosY;

	// Add button linking to the Localization Demos menu
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneLocalization> );
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Localization Demos", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneLocalization>);
	    m_funcEnt.push_back(SceneLocalization::createScene);
	}

	// Add button linking to Dynamic Dialogue Demo
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", replaceScene<SceneDynamicDialogue>);
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    auto label = addLabelEx("Dynamic Dialogue Demo", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this);
	    label->setCallback(replaceScene<SceneDynamicDialogue>);
	    m_funcEnt.push_back(SceneDynamicDialogue::createScene);
	}
    }

    return true;
}


void SceneDialogueMenu::menuCloseCallback(Ref* pSender)
{
    //auto director = Director::getInstance();
    //director->replaceScene(SceneMain::createScene());
    //this->removeAllChildren();
}
/*
void SceneDialogueMenu::setMenuColor(int index)
{
    for (int i = 0; i < (int)m_menu.size(); i++)
	((cocos2d::Label*)m_menu[i]->getLabel())->setTextColor(i == index ? cocos2d::Color4B::YELLOW : cocos2d::Color4B::WHITE);
}

void SceneDialogueMenu::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode) {
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
	//g_menu_ix = m_lastMenuIx;
	if ((int)m_funcEnt.size()>m_lastMenuIx)
	{
	    auto scene = m_funcEnt[m_lastMenuIx]();
	    if (scene) {
		Director::getInstance()->replaceScene((cocos2d::Scene *)scene);
		this->removeAllChildren();
	    }
	}
	else {
	    //onNotImplemented(NULL);
	}
    }
    break;

    default:
	SceneBase::onKeyPressed(keyCode, event);
	return;
    }
}*/



