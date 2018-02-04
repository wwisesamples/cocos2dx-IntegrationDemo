#include "SceneExternalSources.h"
#include "SceneMain.h"
#include "WwiseWrapper.h"
#include "Platform.h"
#include "PlatformCocos.h"

#define  LOG_TAG    __FILE__

USING_NS_CC;

//The game object ID used for this demo, it can be anything (except 0 and 1, as stated in the RegisterGameObj() doc)
#define MICRO_GAME_OBJECT 1234

SceneExternalSources::SceneExternalSources()
: SceneBase("External Sources Demo",
    "This page shows how to use external sources.\n\n"
    "The same sequence container is used to play either "
    "\"1 2 3\" or \"4 5 6\". The files 1, 2, 3, 4, 5 and "
    "6 are selected dynamically at run-time.",  SceneMain::createScene)
{
    m_uPackageID = 0;
}

SceneExternalSources::~SceneExternalSources()
{
}

Scene* SceneExternalSources::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = SceneExternalSources::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SceneExternalSources::init()
{
    //////////////////////////////
    // 1. super init first
    if (!SceneBase::init())
    {
	return false;
    }
    cocos2d::Size windowSize = Director::getInstance()->getWinSize();
#if defined(AK_ANDROID) || defined(AK_IOS)
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

	int y = descriptionPosY;
	// Add button linking to the Say "Hello"
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneExternalSources::onPlay123_Pressed, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    cocos2d::MenuItemLabel* pMenu = addLabelEx("Play \"1 2 3\"", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneExternalSources::onPlay123_Pressed, this));
	}

	// Add button linking to RTPC Demo (Car Engine)
	{
	    auto selectItem = MenuItemImage::create("PlayNormal.png", "PlayPush.png", CC_CALLBACK_1(SceneExternalSources::onPlay456_Pressed, this));
	    y -= selectItem->getContentSize().height;
	    addItem(selectItem, selectButtonPosX, y, this);
	    cocos2d::MenuItemLabel* pMenu = addLabelEx("Play \"4 5 6\"", selectItem->getPosition().x + selectItem->getContentSize().width, selectItem->getPosition().y, FONT_SIZE_MENU, this, CC_CALLBACK_1(SceneExternalSources::onPlay456_Pressed, this));
	}
    }

    if (Wwise::Instance().IOManager().LoadFilePackage(AKTEXT("ExternalSources.pck"), m_uPackageID) != AK_Success)
    {
	SetLoadFileErrorMessage("ExternalSources.pck");
	return false;
    }

    // Load the sound bank that contains our event and sound structure
    AkBankID bankID; // Not used
    if (AK::SoundEngine::LoadBank("ExternalSources.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	SetLoadFileErrorMessage("ExternalSources.bnk");
	return false;
    }

    // Register the "Human" game object
    AK::SoundEngine::RegisterGameObj(GAME_OBJECT_HUMAN, "Human");

    scheduleUpdate();

    return true;
}

void SceneExternalSources::onRelease()
{
    AK::SoundEngine::StopAll();

    // Unregister the "Human" game object
    AK::SoundEngine::UnregisterGameObj(GAME_OBJECT_HUMAN);

    // Unload the sound bank
    AK::SoundEngine::UnloadBank("ExternalSources.bnk", NULL);

    // Unload the file package containing the external sources
    // Note that CAkFilePackageLowLevelIO's file package loading is not thread safe.
    // We need to ensure that there is no more I/O occurring before unloading the 
    // file package. Let's wait a few milliseconds to ensure StopAll() has executed.
    AKPLATFORM::AkSleep(25);
    Wwise::Instance().IOManager().UnloadFilePackage(m_uPackageID);
    m_lastMenuIx = 8;
}

void SceneExternalSources::onPlay123_Pressed(cocos2d::Ref* pSender)
{
    // Set up external source data for the 3 external sources contained 
    // in each sound of the sequence container.
    // All 3 sounds are streamed (specified by file name).
    // They are streamed from a file package, which was loaded in Init().
    AkExternalSourceInfo sources[3];

    // The cookie is the hash of the name of the external source, declared in
    // the sound's Contents Editor.
    sources[0].iExternalSrcCookie = AK::SoundEngine::GetIDFromString("Extern_1st_number");
    // IMPORTANT: The extension HAS to be ".wem". In our external source definition
    // file (WwiseProject/myExternalSources/ExtSourceList.wsources), we may rename the converted 
    // file, but we cannot change the extension. The file is ultimately indexed in the packaged file
    // with its full name, including the extension .wem.
    sources[0].szFile = (AkOSChar*)AKTEXT("01.wem");
    sources[0].idCodec = CODECTYPE_STANDARD;	// Defined in Platform.h (cross-platform ADPCM)

    sources[1].iExternalSrcCookie = AK::SoundEngine::GetIDFromString("Extern_2nd_number");
    sources[1].szFile = (AkOSChar*)AKTEXT("02.wem");
    sources[1].idCodec = CODECTYPE_STANDARD;

    sources[2].iExternalSrcCookie = AK::SoundEngine::GetIDFromString("Extern_3rd_number");
    sources[2].szFile = (AkOSChar*)AKTEXT("03.wem");
    sources[2].idCodec = CODECTYPE_STANDARD;

    AK::SoundEngine::PostEvent(
	AK::EVENTS::PLAY_THREE_NUMBERS_IN_A_ROW,
	GAME_OBJECT_HUMAN,
	0,
	NULL,
	NULL,
	3,
	sources);
}

void SceneExternalSources::onPlay456_Pressed(cocos2d::Ref* pSender)
{
    AkExternalSourceInfo sources[3];

    // Here, the external source data is set up to play "4", "5" and "6".
    sources[0].iExternalSrcCookie = AK::SoundEngine::GetIDFromString("Extern_1st_number");
    sources[0].szFile = (AkOSChar*)AKTEXT("04.wem");
    sources[0].idCodec = CODECTYPE_STANDARD;

    sources[1].iExternalSrcCookie = AK::SoundEngine::GetIDFromString("Extern_2nd_number");
    sources[1].szFile = (AkOSChar*)AKTEXT("05.wem");
    sources[1].idCodec = CODECTYPE_STANDARD;

    sources[2].iExternalSrcCookie = AK::SoundEngine::GetIDFromString("Extern_3rd_number");
    sources[2].szFile = (AkOSChar*)AKTEXT("06_high.wem");
    sources[2].idCodec = AKCODECID_PCM;	// This one uses conversion shareset External_HighQuality, 
					// (see WwiseProject/myExternalSources/ExtSourceList.wsources). This conversion setting is in PCM.

    AK::SoundEngine::PostEvent(
	AK::EVENTS::PLAY_THREE_NUMBERS_IN_A_ROW,
	GAME_OBJECT_HUMAN,
	0,
	NULL,
	NULL,
	3,
	sources);
}
