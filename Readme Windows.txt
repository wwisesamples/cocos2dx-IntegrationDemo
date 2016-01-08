Setting up IntegrationDemo & cocos2d-x for Windows build environment on Windows 

1. Environment Requirements
  - Windows 7 or higher 
  - Visual Studio 2015
  - Wwise SDK
  * Refer http://www.cocos2d-x.org/wiki/Win32_Installation_and_Setup
  
2. Make sure that you have Wwise SDK installed with following component
   - SDK (Common)
   - SDK (Windows, Visual Studio 2015)
3. Download IntegrationDemo-cocos folder from ftp.audiokinetic.com
4. Copy 2. over to <Wwise SDK>\SDK\samples
   e.g.)  2.  -> c:\Wwise_v2015.1.4_build_5497\SDK\samples
5. Build 
   1) Launch Visual Studio 2015
   2) Open solution file (<Wwise SDK>\SDK\samples\IntegrationDemo\cocos2d-x\proj.win32\IntegrationDemo-cocos2d-x.sln)
   3) From menu. [Build],[Build Solution]
   