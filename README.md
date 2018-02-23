# Wwise Integration Demo for cocos2d-x 

Based on [the original community demo of Wwise Cocos2d-x engine integration](https://github.com/wwisesamples/cocos2dx-IntegrationDemo), this fork aims to:

1. Temporarily fix compatibility with recent Wwise and Cocos2d-x versions, with no guaranteed long-term maintenance.
2. Add missing features added in recent IntegrationDemo. 
3. Keep the code clean for a better sample.

Compared to [The Wwise SDK IntegrationDemo](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html), which adopts a control scheme using virtual joysticks, [the original author](https://github.com/wwisesamples) improves the user experience by implementing a more intuitive touch screen UX, exemplified by the footsteps and positioning demos.

The demo supports the following platforms:

- Android
- iOS
- Mac
- Windows

## Update: February 9, 2018

Content

- Updated to support Wwise 2017.1 and Cocos2d-x 3.16.
- Added previously disabled Background Music demo.
- Added new plugin registration including Wwise Reflect and Wwise Recorder.
- Fixed: Crashes when quitting Footstep demo.
- Simplified source file and folder structure.
- Lots of refactoring.

Tested with the following software versions:

- [Wwise 2017.1.4.6407](https://www.audiokinetic.com/download/)
- [cocos2d-x 3.16](http://www.cocos2d-x.org/download)
- Android 6.0.1, on Huawei Honor 5X (KIW-AL10)
- iOS 11.2.5, iPhone 6
- macOS Sierra 10.12.6
- Windows 10
- Xcode 9.2
- Visual Studio 2017
- JDK 1.8, Android SDK API 24, NDK r12b, Apache ANT 1.8.2

## Installing the Demo

First, clone the demo.

```
git clone https://github.com/cocos2dx-IntegrationDemo.git wwise-cocos-demo
```
This is a pre-configured cocos application project.

Then, because the demo code depends on partial code of [the Wwise SDK IntegrationDemo](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html), you need to install a supported Wwise version with Wwise SDK of the target platforms。

Finally, copy the demo into the SDK IntegrationDemo's folder. After that, your clone's root folder (e.g., `wwise-cocos-demo`) should be placed under `<Wwise Root>/SDK/samples/IntegrationDemo/`, e.g.:

`<Wwise Root>/SDK/samples/IntegrationDemo/wwise-cocos-demo`

  
## Building the Demo

To run the demo, you need to build it from source first.

For each supported platform, there is a ready-to-use IDE solution or project. A full Cocos2d-x engine distribution is also available as part of the clone, as expected with a cocos project. You can build all the demos for your chosen platforms with a single click or a single command on top of the bundled Cocos2d-x.

If you want to work with your own version of Cocos2d-x, you'll need to replace the `cocos2d` folder with the corresponding folder in your Cocos distribution and [set up your project by hand](https://github.com/cocos2d/cocos2d-x/blob/v3/README.md#how-to-start-a-new-game).

Two build configurations are prepared for each supported platform. The *Debug* build can be remotely profiled using Wwise Authoring Tool, but is not optimized. The *Release* build is optimized but disables profiling.

### Building on Windows

Prerequisites

  - Windows 7 or higher 
  - Visual Studio 2017
  - [DirectX SDK](https://www.microsoft.com/en-ca/download/details.aspx?id=6812)
  - Wwise SDK installed with the following component: SDK (Windows, Visual Studio 2017)

Notes on DirectX:

- Install the DirectX SDK version mentioned in Wwise's [Platform Requirements](https://www.audiokinetic.com/library/edge/?source=SDK&id=reference__platform.html).
- Restart your Windows Explorer or reboot your machine to ensure the environment variable *DXSDK_DIR* is in effect. The Visual Studio project depends on this variable.

How to build

- Build the solution under `<Wwise Root>/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.win32/IntegrationDemo-cocos2d-x.sln` with your Visual Studio.
- Open the IntegrationDemo's Wwise project with the compatible Wwise version.
- Generate the SoundBanks for Windows under their default path.


### Building on Mac

Prerequisites

- Xcode with a developer credential.
- Wwise SDK installed with the following component: SDK (macOS)

How to build

- Open `<Wwise Root>/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.ios_mac/IntegrationDemo-cocos.xcodeproj` with Xcode.
- Build the Scheme `IntegrationDemo-cocos-desktop`.
- Open the IntegrationDemo's Wwise project with the compatible Wwise version.
- Generate the SoundBanks for Mac under their default path.


### Building on iOS

Prerequisites

- Xcode with a developer credential.
- Wwise SDK installed with the following component: SDK (iOS)


How to build

- Open `<Wwise Root>/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.io`s_mac/IntegrationDemo-cocos.xcodeproj` with Xcode.
- Build the Scheme `IntegrationDemo-cocos-mobile`.
- Open the IntegrationDemo's Wwise project with the compatible Wwise version.
- Generate the SoundBanks for iOS under their default path.


### Building on Android

Prerequisites

- Android SDK/NDK/Apache ANT
- Python 2.7.x
- Wwise SDK installed with the following component: SDK (Android)

How to build

- Open Terminal/Command Prompt: `cd cocos2dx-demo/proj.android`.
- [Run Cocos command to set up Android environment variables](https://github.com/cocos2d/cocos2d-x/blob/v3/README.md#how-to-start-a-new-game).
- Compile the project
	- Debug: `cocos compile -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1`
	- Release: `cocos compile -p android`
- Open the IntegrationDemo's Wwise project with the compatible Wwise version.
- Generate the SoundBanks for Android under their default path.
- Build the APK and run
	- Debug: `cocos run -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1`
	- Release: `cocos run -p android`


## How to interact with the demo?

Use the demo's menu system to navigate to individual demo topics. You can use keyboard, mouse, or touch screen to control the demo.


## The Demos

Browse the `The Demos` section of [the original documentation](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html) to get to know how each demo works. The major difference here is in user input scheme. On mobile platforms, this demo uses touch screen gestures instead of virtual joysticks, thanks to Cocos2d-x's facility; on desktop, it supports  mouse gestures while the SDK IntegrationDemo does not.


## The Wwise Project

You need to generate the SoundBanks of your target platform for the demo to work on that platform. SoundBanks are the output of a Wwise project. The Wwise project for this demo is the same one for the SDK IntegrationDemo, available at `<Wwise Root>/SDK/samples/IntegrationDemo/WwiseProject`.

Read Wwise documentation to learn how to generate SoundBanks. Once that's done, by default they will be in `GeneratedSoundBanks` sub-folder under the Wwise project folder.


## The Demo Code and File Structure

Compared to [the community demo repo](https://github.com/wwisesamples/cocos2dx-IntegrationDemo),  the Wwise integration code has been refactored to make it easier to follow what needs to be done for each platform integration as a whole. This was achieved mainly by separating extract cross-platform code and separate it from the platform-specific code. Of course, ad-hoc platform branching code is still inevitable, but that is  now at a bare minimum. 

A few pointers for you to walk around the code base:

- `Classes/Scene*.h` and `Classes/Scene*.cpp` along with the cocos template AppDelegate code are the demo application code.
- `Classes/WwiseWrapper.h` and `Classes/WwiseWrapper.cpp` handle cross-platform Wwise sound engine lifecycle.
- All the platform-specific code and IDE solution or project files are under the folders named after those platforms.
	- `cocos2dx-demo/proj.android/`
	- `cocos2dx-demo/proj.ios_mac/ios/`
	- `cocos2dx-demo/proj.ios_mac/mac/`
	- `cocos2dx-demo/proj.win32/`
- `<platform>/WwisePlatformHelper.h` and `<platform>/WwisePlatformHelper.cpp` handle platform-specific code for Wwise sound engine lifecycle.
- `<platform>/PlatformCocos.h` and `<platform>PlatformCocos.cpp` define platform-specific global variables and functions. They complements the `Platform.h` and `Platform.cpp` of the SDK IntegrationDemo.

## License and Support

This forked demo shares the same license as the [original project](https://github.com/wwisesamples/cocos2dx-IntegrationDemo). 

You may raise issues here at the Issues section but there is no guarantee that they will be answered soon enough.
