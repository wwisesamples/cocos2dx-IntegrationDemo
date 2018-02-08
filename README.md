# Wwise and cocos2d-x IntegrationDemo

Based on [the original community demo of Wwise Cocos2d-x engine integration](https://github.com/wwisesamples/cocos2dx-IntegrationDemo), this fork aims to:

1. Update the demo to work with a recent Wwise and Cocos2d-x version.
2. Add a few missing features, e.g., background music, additional SoundBank paths. 
3. Refactor the code and file/folder structure for a more maintainable and standard demonstration.

Compared to [the Wwise SDK IntegrationDemo](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html), [the original author](https://github.com/wwisesamples) implemented more intuitive touch screen UX for a few demos. The author should have the full credits for the content of that repo.

The demo supports the following platforms:

- Android
- iOS
- Mac
- Windows

## Update: February 9, 2018

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

  
## Building and Running the Demo

### Demo Installation

The demo code depends on partial code of [the Wwise SDK IntegrationDemo](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html), [the original author](https://github.com/wwisesamples). So you need to install a supported Wwise version, and copy the demo into the SDK IntegrationDemo's folder. When it's done,  your clone's root folder (e.g., cocos2dx-demo) should be under `/path/to/Wwise/SDK/samples/IntegrationDemo/`. So it will be at:

`/path/to/Wwise/SDK/samples/IntegrationDemo/cocos2dx-demo`

Since the demo comes with a complete Cocos2d-x engine build, you could quickly test it by building the demo for any supported platform. 

You need to install Wwise with Wwise SDK of the supported version, and build the demo from source code.

If you want to work with your own version of Cocos2d-x, you'll need to replace the `cocos2d` folder with your own version and [set up your project](https://github.com/cocos2d/cocos2d-x/blob/v3/README.md#how-to-start-a-new-game).

The Debug build can be remotely profiled, but is not optimized. The Release build is optimized but disables profiling.

### Building on Windows

Prerequisites

  - Windows 7 or higher 
  - Visual Studio 2017
  - [DirectX SDK](https://www.microsoft.com/en-ca/download/details.aspx?id=6812)
  - Wwise SDK installed with the following component: SDK (Windows, Visual Studio 2017)

Notes on DirectX:

- Install the version of DirectX SDK mentioned in [Platform Requirements](https://www.audiokinetic.com/library/edge/?source=SDK&id=reference__platform.html) of Wwise documentation.
- Restart your Windows Explorer or reboot your machine to ensure the environment variable *DXSDK_DIR* is in effect. The Visual Studio project depends on this variable.

How to build

- Build the solution under `/path/to/Wwise/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.win32/IntegrationDemo-cocos2d-x.sln` with your Visual Studio.
- Open the IntegrationDemo's Wwise project with the target Wwise version.
- Generate the SoundBanks for Windows in their default paths.
- Launch the demo's executable just built.


### Building on Mac

Prerequisites

- Xcode with developer credential filled in.
- Wwise SDK installed with the following component: SDK (macOS)

How to build

- Open `/path/to/Wwise/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.ios_mac/IntegrationDemo-cocos.xcodeproj` with Xcode.
- Build the Scheme `IntegrationDemo-cocos-desktop`.
- Generate the project's SoundBank for Mac.
- Run the executable.

### Building on iOS

Prerequisites

- Xcode with developer credential filled in.
- Wwise SDK installed with the following component: SDK (iOS)


How to build

- Open `/path/to/Wwise/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.ios_mac/IntegrationDemo-cocos.xcodeproj` with Xcode.
- Build the Scheme `IntegrationDemo-cocos-mobile`.
- Generate the project's SoundBank for iOS.
- Run the executable.


### Building on Android

Prerequisites

- Android SDK/NDK/Apache ANT
- Go under cocos2dx-demo/proj.android to make it as current working directory.
- Run Cocos to set up Android environment variables.
- Compile the project
	- Debug   -> cocos compile -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1
	- Release -> cocos compile -p android　
- Generate the project's SoundBank for Android.
- Build & run 
	- Debug   -> cocos run -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1
	- Release -> cocos run -p android


## How to interact the demo?

The demo has a menu system where you can navigate to individual small demos. You can use keyboard, mouse, and touch control for playing it.


## The Demos

Check out the `The Demos` section of [the original documentation](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html).



## The Wwise Project

The Wwise project for this program is also available in /`path/to/Wwise/SDK/samples/IntegrationDemo/WwiseProject`.

The SoundBanks for this project are installed with the SDK in the "samples\IntegrationDemo\WwiseProject\GeneratedSoundBanks" folder.

To regenerate the SoundBanks, so
å
- Check all banks in the SoundBanks list 
- Check off all platforms that are being tested 
- Check all languages in the Languages list 

Once these settings are in place, you can click on Generate in the SoundBank Manager to generate the banks.

## The Demo Code and File Structure.

The Demo Code and folder structure have been restructured to better demonstrate how to integrate Wwise in a more maintainable fashion.

- `Classes/WwiseWrapper.h` and `Classes/WwiseWrapper.cpp` handle cross-platform Wwise life/cycle. 
- `<platform>/WwisePlatformHelper.h` and `<platform>/WwisePlatformHelper.cpp` handle platform-specific code for Wwise life/cycle.
- `PlatformCocos.h` and `PlatformCocos.cpp` define platform-specific variables.
- All platform-specific code are under the folders named after them.
	- `cocos2dx-demo/proj.android/`
	- `cocos2dx-demo/proj.ios_mac/ios/`
	- `cocos2dx-demo/proj.ios_mac/mac/
	- `cocos2dx-demo/proj.win32/
