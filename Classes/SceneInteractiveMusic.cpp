#include "SceneInteractiveMusic.h"
#include "SceneMain.h"
#include "WwiseWrapper.h"
#include "Helpers.h"
#include "Platform.h"
#include "PlatformCocos.h"

USING_NS_CC;

extern int g_menu_sub_ix;

struct tbl_menu 
{
    string str;
    const AkUniqueID id;
};
tbl_menu g_tbl_menu[] = {
    { "Explore",		AK::EVENTS::IM_EXPLORE},
    { "Begin communication",	AK::EVENTS::IM_COMMUNICATION_BEGIN },
    { "They are hostile",	AK::EVENTS::IM_THEYAREHOSTILE },
    { "Fight one enemy",	AK::EVENTS::IM_1_ONE_ENEMY_WANTS_TO_FIGHT },
    { "Fight two enemies",	AK::EVENTS::IM_2_TWO_ENEMIES_WANT_TO_FIGHT },
    { "Surrounded by enemies",	AK::EVENTS::IM_3_SURRONDED_BY_ENEMIES },
    { "Death is coming",	AK::EVENTS::IM_4_DEATH_IS_COMING },
    { "Game Over",		AK::EVENTS::IM_WINTHEFIGHT },
    { "Win the fight",		AK::EVENTS::IM_GAMEOVER }
};

#define SIZEOF(x)  (sizeof(x) / sizeof((x)[0]))

SceneInteractiveMusic::SceneInteractiveMusic()
: SceneBase("Interactive Music Demo",
    "This example uses a Music Switch container. "
    "Try switching states by triggering the event "
    "listed in the demo page. Switching state might produce "
    "a result that is immediate or occur at the time "
    "specified in the rules of the music containers.",
    SceneMain::createScene)
, m_iPlayingID(0)
, m_label(0)
{
   m_menuix = 0/*g_menu_sub_ix*/;
}

SceneInteractiveMusic::~SceneInteractiveMusic()
{
}

Scene* SceneInteractiveMusic::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create(); 
    // 'layer' is an autorelease object
    auto layer = SceneInteractiveMusic::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneInteractiveMusic::init()
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
#if  defined(AK_IOS)
    float selectButtonPosX = windowSize.height * 0.50f;
#else
    float selectButtonPosX = windowSize.width  * 0.30f;
#endif
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    TTFConfig ttfConfig("fonts/arial.ttf", 24);
    
    int y = descriptionPosY;
    for (int i = 0; i < SIZEOF(g_tbl_menu); i++) {
        MenuItemLabel* label = SceneBase::addLabelEx(g_tbl_menu[i].str, selectButtonPosX, y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneInteractiveMusic::PostEvent_, this, i));
        m_menu[i] = (Label*)label->getChildren().at(0);
        y -= m_menu[i]->getContentSize().height;
    }

    setMenuColor(m_menuix);

    // Add text labels 
    m_label = addLabel("Position: 10000\nSegment duration : 12000\nPre - Entry duration : 0\nPost - Exit duration 0\n", 0, 0, FONT_SIZE_MENU, this/*, Vec2::ANCHOR_MIDDLE*/);
    m_label->setPosition(descriptionPosX - (m_label->getContentSize().width / 2), m_label->getContentSize().height);


    // Load the sound bank
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("InteractiveMusic.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetLoadFileErrorMessage("InteractiveMusic.bnk");
	return false;
    }

    // Register the "Music" game object
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_MUSIC, "Music");

    //Start the interactive music
    m_iPlayingID = AK::SoundEngine::PostEvent(
	AK::EVENTS::IM_START,
	GAME_OBJECT_MUSIC,
	AK_EnableGetMusicPlayPosition);

    scheduleUpdate();

    return true;
}

//void SceneInteractiveMusic::setMenuColor(int index)
//{
//    for (int i = 0; i < TBL_SIZE; i++)
//	m_menu[i]->setTextColor(i == (int)index ? cocos2d::Color4B::YELLOW : cocos2d::Color4B::WHITE);
//}

void SceneInteractiveMusic::PostEvent_(Ref* sender, /*const AkUniqueID id*/int ix)
{
    g_menu_sub_ix = m_menuix  = ix;
    setMenuColor(ix);
    AK::SoundEngine::PostEvent((AkUniqueID)g_tbl_menu[ix].id, GAME_OBJECT_MUSIC);
}

//void SceneInteractiveMusic::menuCloseCallback(Ref* pSender)
//{
//    m_label = NULL;
//    auto director = Director::getInstance();
//    //director->replaceScene(SceneMain::createScene());
//    this->removeAllChildren();
//}

void SceneInteractiveMusic::onRelease()
{
    m_label = NULL;
    // Stop the sound playing
    AK::SoundEngine::StopPlayingID(m_iPlayingID);

    // Unregister the "Music" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_MUSIC);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("InteractiveMusic.bnk", NULL);
    m_lastMenuIx = 5;
}

void SceneInteractiveMusic::update(float dt)
{
    WWISE::update();
    AkSegmentInfo info;

    AK::MusicEngine::GetPlayingSegmentInfo(m_iPlayingID, info);

    //Page::Draw();

    char strBuf[256];

    snprintf(strBuf,
	256,
	"Position: %d\nSegment duration: %d\nPre-Entry duration: %d\nPost-Exit duration %d\n",
	(int)info.iCurrentPosition,
	(int)info.iActiveDuration,
	(int)info.iPreEntryDuration,
	(int)info.iPostExitDuration);

    // Draw the interactive music position
    if (m_label) m_label->setString(strBuf);
}

void SceneInteractiveMusic::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* unused_event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
	if (m_menuix > 0) m_menuix--;
	setMenuColor(m_menuix);
	break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	if (m_menuix < TBL_SIZE) m_menuix++;
	setMenuColor(m_menuix);
	break;
    case EventKeyboard::KeyCode::KEY_KP_ENTER:
	g_menu_sub_ix = m_menuix;
	PostEvent_(NULL, m_menuix);
	break;
//    case EventKeyboard::KeyCode::KEY_ESCAPE:
//	menuCloseCallback(NULL);
//	return;

    default:
	SceneBase::onKeyPressed(keyCode, unused_event);
	return;
    }
}
