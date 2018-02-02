#include "AppDelegate.h"
#include "SceneMain.h"
#include "WwiseWrapper.h"

#if  __APPLE__
#include "TargetConditionals.h"
#endif


#include <AK/Plugin/AkRoomVerbFXFactory.h>
#include <AK/Plugin/AkStereoDelayFXFactory.h>

USING_NS_CC;

//static cocos2d::Size designResolutionSize = cocos2d::Size(640, 960);	//h
#ifdef WIN32
    static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 960);	//h
#elif  CC_TARGET_OS_MAC
    static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 960);	//h
#elif  TARGET_IPHONE
    static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
#else
    static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
#endif

static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);
int g_menu_ix = 0;
int g_menu_sub_ix = 0;

#ifdef AK_APPLE
bool g_bEnableMicDemo = false;
#endif

AppDelegate::AppDelegate() 
{

}

AppDelegate::~AppDelegate() 
{
   WWISE::terminate();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() 
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("IntegrationDemo", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("IntegrationDemo");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    cocos2d::Size frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    #ifdef AK_ANDROID
	AAssetManager* mgr = FileUtilsAndroid::getAssetManager();
	__android_log_print(ANDROID_LOG_DEBUG, "AppDelegate::applicationDidFinishLaunching", "AAssetManager:0x%x", mgr);
	#else
	void*		mgr = NULL;
    #endif
    WWISE::initialize(mgr);
    // create a scene. it's an autorelease object
    auto scene = SceneMain::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
