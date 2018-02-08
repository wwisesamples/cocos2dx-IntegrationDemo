#include "SceneMusicCallbacks.h"
#include "SceneMain.h"
#include "WwiseWrapper.h"
#include "Helpers.h"
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

extern  int g_menu_sub_ix;


SceneMusicCallbacksRoot::SceneMusicCallbacksRoot()
:SceneBase("Music Callbacks Demos",
    "This demo shows how to use the different music callbacks.\n\n",
    SceneMain::createScene)
{
    m_menuix = g_menu_sub_ix;
}

SceneMusicCallbacksRoot::~SceneMusicCallbacksRoot()
{
}

void SceneMusicCallbacksRoot::onRelease()
{
    m_lastMenuIx = 4;
}

Scene* SceneMusicCallbacksRoot::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneMusicCallbacksRoot::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneMusicCallbacksRoot::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneBase::init())
    {
        return false;
    }

    cocos2d::Size windowSize = Director::getInstance()->getWinSize();
    float descriptionPosY = windowSize.height * 0.80f;
    float selectButtonPosX = g_isLandscape ? windowSize.width  * 0.30f : windowSize.height * 0.50f;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    int y = descriptionPosY;
    //TTFConfig ttfConfig("fonts/arial.ttf", 24);

    // for "Music Sync Callback Demo" Menu
    MenuItemLabel* label = addLabelEx("Music Sync Callback Demo", selectButtonPosX, y, FONT_SIZE_MENU, this);
    label->setCallback(replaceScene<SceneMusicSyncCallbacks>);
    m_menu[0] = (Label*)label->getChildren().at(0);
    m_funcEnt.push_back(SceneMusicSyncCallbacks::createScene);

    // for "Music Playlist Callback Demo" Menu
    y -= m_menu[0]->getContentSize().height * 2;
    label = addLabelEx("Music Playlist Callback Demo", selectButtonPosX, y, FONT_SIZE_MENU, this);
    label->setCallback(replaceScene<ScenePlayListCallbacks>);
    m_menu[1] = (Label*)label->getChildren().at(0);
    m_funcEnt.push_back(ScenePlayListCallbacks::createScene);

    // for "MIDI Callback Demo" Menu
    y -= m_menu[0]->getContentSize().height * 2;
    label = addLabelEx("MIDI Callback Demo", selectButtonPosX, y, FONT_SIZE_MENU, this);
    label->setCallback(replaceScene<SceneMIDICallbacks>);
    m_menu[2] = (Label*)label->getChildren().at(0);
    m_funcEnt.push_back(SceneMIDICallbacks::createScene);

    //m_label[0] = (Label*)label->getChildren().at(0);

    setMenuColor(m_menuix);

    return true;
}

void SceneMusicCallbacksRoot::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
	if (m_menuix > 0) m_menuix--;
	setMenuColor(m_menuix);
	break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	if (m_menuix < 3) m_menuix++;
	setMenuColor(m_menuix);
	break;
    case EventKeyboard::KeyCode::KEY_KP_ENTER:
	g_menu_sub_ix = m_menuix;
	if (m_menuix == 0) 
	    Director::getInstance()->replaceScene(SceneMusicSyncCallbacks::createScene());
	else if (m_menuix == 1)
	    Director::getInstance()->replaceScene(ScenePlayListCallbacks::createScene());
	else
	    Director::getInstance()->replaceScene(SceneMIDICallbacks::createScene());
	this->removeAllChildren();
	break;

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
}

void SceneMusicCallbacksRoot::setMenuColor(int index)
{
    for (int i = 0; i < 3; i++)
	m_menu[i]->setTextColor(i== (int)index? cocos2d::Color4B::YELLOW : cocos2d::Color4B::WHITE);

}


// *********************************************************************************
// Base class
// *********************************************************************************
SceneMusicCallbacksBase::SceneMusicCallbacksBase(string strTitle, string strHelp, FUNC_MAIN_SCENE pfnc)
    :SceneBase(strTitle, strHelp, SceneMusicCallbacksRoot::createScene)
    , m_bIsPlaying(false)
    , m_bStopPlaylist(false)
    , m_iPlayingID(0)
    , m_label(0)
    , m_label2(0)
{
}

bool SceneMusicCallbacksBase::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneBase::init())
    {
	return false;
    }

    /* Scene label*/
    {
	int iPosX = m_pParentMenu->GetWidth()  / 2;
	int iPosY = m_pParentMenu->GetHeight() * 0.6;
	auto label = Label::createWithSystemFont("", "Arial", FONT_SIZE_MENU, cocos2d::Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	label->setPosition(cocos2d::Point(iPosX, iPosY));
	this->addChild(label, 1);
	m_label = label;
    }
    /* Scene label2*/
    {
	int iPosX = m_pParentMenu->GetWidth()  / 2;
	int iPosY = m_pParentMenu->GetHeight() * 0.5;
	auto label = Label::createWithSystemFont("", "Arial", FONT_SIZE_MENU, cocos2d::Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	label->setPosition(cocos2d::Point(iPosX, iPosY));
	this->addChild(label, 1);
	m_label2 = label;
    }

    // Load the sound bank
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("MusicCallbacks.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetLoadFileErrorMessage("MusicCallbacks.bnk");
	return false;
    }

    // Register the "Markers" game object
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_MUSIC, "Music");

    // Initialize the marker playback variables
    m_bIsPlaying = true;

    return true;
}

void SceneMusicCallbacksBase::update(float dt)
{
    SceneBase::update(dt);
    
    if (!m_bIsPlaying)
    {
	if (m_label) m_label->setString("Test Finished");
    }
}


void SceneMusicCallbacksBase::onRelease()
{
    m_label = m_label2 = NULL;
    // Make sure we do not get called back after page destruction
    AK::SoundEngine::CancelEventCallback(m_iPlayingID);

    // Stop the sound playing
    AK::SoundEngine::StopPlayingID(m_iPlayingID);

    // Unregister the "Music" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_MUSIC);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("MusicCallbacks.bnk", NULL);
}


///////////////////////////
// SceneMusicSyncCallbacks
///////////////////////////
SceneMusicSyncCallbacks::SceneMusicSyncCallbacks()
    : SceneMusicCallbacksBase(
	"Music Sync Callback Demo",
	"Beat and Bar notifications are generated from "
	"playing music's tempo and time signature info.",
	NULL)
    , m_uiBeatCount(0)
    , m_uiBarCount(0)
{
}

Scene* SceneMusicSyncCallbacks::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneMusicSyncCallbacks::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneMusicSyncCallbacks::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneMusicCallbacksBase::init())
    {
	return false;
    }

    m_iPlayingID = AK::SoundEngine::PostEvent(
	AK::EVENTS::PLAYMUSICDEMO1,
	GAME_OBJECT_MUSIC,
	AK_EnableGetSourcePlayPosition | AK_MusicSyncBeat | AK_MusicSyncBar | AK_MusicSyncEntry | AK_MusicSyncExit | AK_EndOfEvent,
	&SceneMusicSyncCallbacks::MusicCallback,
	this
	);

    scheduleUpdate();
    return true;
}

void SceneMusicSyncCallbacks::MusicCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    SceneMusicSyncCallbacks* pPage = (SceneMusicSyncCallbacks*)in_pCallbackInfo->pCookie;

    if (in_eType == AK_MusicSyncBar)
    {
	pPage->m_uiBeatCount = 0;
	pPage->m_uiBarCount++;
    }
    else if (in_eType == AK_MusicSyncBeat)
    {
	pPage->m_uiBeatCount++;
    }
    else if (in_eType == AK_EndOfEvent)
    {
	pPage->m_bIsPlaying = false;
	pPage->m_uiBeatCount = 0;
	pPage->m_uiBarCount = 0;
    }
}

void SceneMusicSyncCallbacks::update(float dt)
{
    SceneMusicCallbacksBase::update(dt);
    if (m_bIsPlaying)
    {
	char strBuf[50];

	AkTimeMs uPosition;

	// Get the current play position and store it in a string buffer
	AK::SoundEngine::GetSourcePlayPosition(m_iPlayingID, &uPosition);
	snprintf(strBuf, 50, "Bar: %d\nBeat: %d\nPosition=%d", (int)m_uiBarCount, (int)m_uiBeatCount, (int)uPosition);

	// Draw the play position and subtitles
	if (m_label) m_label->setString(strBuf);
    }
}


///////////////////////////
// ScenePlayListCallbacks
///////////////////////////

ScenePlayListCallbacks::ScenePlayListCallbacks()
: SceneMusicCallbacksBase("Music Playlist Callback Demo",
    "This example to force a random playlist to select its next item sequentially. "
    "The playlist item may be stopped via the callback as well.",
    NULL)
, m_uiPlaylistItem(0)
{
}

Scene* ScenePlayListCallbacks::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = ScenePlayListCallbacks::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ScenePlayListCallbacks::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneMusicCallbacksBase::init())
    {
	return false;
    }

    m_iPlayingID = AK::SoundEngine::PostEvent(
	AK::EVENTS::PLAYMUSICDEMO2,
	GAME_OBJECT_MUSIC,
	AK_MusicPlaylistSelect | AK_EndOfEvent,
	&ScenePlayListCallbacks::MusicCallback,
	this
	);

    scheduleUpdate();
    return true;
}

void ScenePlayListCallbacks::MusicCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    ScenePlayListCallbacks* pPage = (ScenePlayListCallbacks*)in_pCallbackInfo->pCookie;

    if (in_eType == AK_MusicPlaylistSelect)
    {
	AkMusicPlaylistCallbackInfo* pPlaylistInfo = static_cast<AkMusicPlaylistCallbackInfo*>(in_pCallbackInfo);
	pPlaylistInfo->uPlaylistItemDone = pPage->m_bStopPlaylist;
	pPlaylistInfo->uPlaylistSelection = pPage->m_uiPlaylistItem++;
	if (pPage->m_uiPlaylistItem == pPlaylistInfo->uNumPlaylistItems)
	    pPage->m_uiPlaylistItem = 0;
    }
    else if (in_eType == AK_EndOfEvent)
    {
	pPage->m_bIsPlaying = false;
	pPage->m_uiPlaylistItem = 0;
    }
}

void ScenePlayListCallbacks::update(float dt)
{
    SceneMusicCallbacksBase::update(dt);

    if (m_bIsPlaying)
    {
	char strBuf[50];

	snprintf(strBuf, 50, "Random playlist forced to sequential\nNext Index:%d", (int)m_uiPlaylistItem);

	// Draw the play position and subtitles
	if (m_label) m_label->setString(strBuf);
    }
}



///////////////////////////
// SceneMIDICallbacks
///////////////////////////
SceneMIDICallbacks::SceneMIDICallbacks()
: SceneMusicCallbacksBase("MIDI Callback Demo",
    "Shows MIDI messages the game can receive using callbacks. MIDI messages"
    " include the MIDI notes, CC values, Pitch Bend, After Touch and Program Changes.",
    NULL)
, m_byNote(0)
, m_byVelocity(0)
, m_byCc(0)
, m_byValue(0)
{
}


Scene* SceneMIDICallbacks::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneMIDICallbacks::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneMIDICallbacks::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneMusicCallbacksBase::init())
    {
	return false;
    }
 
    m_iPlayingID = AK::SoundEngine::PostEvent(
	AK::EVENTS::PLAYMUSICDEMO3,
	GAME_OBJECT_MUSIC,
	AK_MIDIEvent | AK_EndOfEvent,
	&SceneMIDICallbacks::MusicCallback,
	this
	);

    scheduleUpdate();
    return true;
}

void SceneMIDICallbacks::MusicCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
    SceneMIDICallbacks* pPage = (SceneMIDICallbacks*)in_pCallbackInfo->pCookie;

    if (in_eType == AK_MIDIEvent)
    {
	AkMIDIEventCallbackInfo* pMidiInfo = static_cast<AkMIDIEventCallbackInfo*>(in_pCallbackInfo);

	if (pMidiInfo->midiEvent.byType == AK_MIDI_EVENT_TYPE_CONTROLLER)
	{
	    pPage->m_byCc = pMidiInfo->midiEvent.Cc.byCc;
	    pPage->m_byValue = pMidiInfo->midiEvent.Cc.byValue;
	}
	else if (pMidiInfo->midiEvent.byType == AK_MIDI_EVENT_TYPE_NOTE_ON )
	{
	    pPage->m_byNote = pMidiInfo->midiEvent.NoteOnOff.byNote;
	    pPage->m_byVelocity = pMidiInfo->midiEvent.NoteOnOff.byVelocity;
	}
    }
    else if (in_eType == AK_EndOfEvent)
    {
	pPage->m_bIsPlaying = false;
    }
}

static const int kNotesPerOctave = 12;
static const char* kNoteArray[kNotesPerOctave] =
{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

// Helper to convert note number to note text
void MidiNoteToString(int in_iNoteNum, char* out_szNote, unsigned int in_uiBufferSize)
{
    int octave = (in_iNoteNum / kNotesPerOctave);
    const char* myNote = kNoteArray[in_iNoteNum % kNotesPerOctave];

    octave -= 1;
    snprintf(out_szNote, in_uiBufferSize, "%s%i", myNote, octave);
}

void SceneMIDICallbacks::update(float dt)
{
    SceneMusicCallbacksBase::update(dt);

    if (m_bIsPlaying)
    {
	char strBuf[50];

	char strNote[10] = { 0 };
	MidiNoteToString(m_byNote, strNote, sizeof(strNote) / sizeof(char));

	snprintf(strBuf, 50, "Last Note ON:%s\nVelocity:%d", strNote, m_byVelocity);

	// Draw the play position and subtitles
	if (m_label) m_label->setString(strBuf);

	snprintf(strBuf, 50, "Last CC%d:%d", m_byCc, m_byValue);

	// Draw the play position and subtitles
	if (m_label2) m_label2->setString(strBuf);
    }
}


