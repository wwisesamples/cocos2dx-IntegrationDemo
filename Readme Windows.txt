Setting up IntegrationDemo & cocos2d-x for Windows build environment on Windows 

1. Environment Requirements
  - Windows 7 or higher 
  - Visual Studio 2015
  - Wwise SDK
    * Refer http://www.cocos2d-x.org/wiki/Win32_Installation_and_Setup
  - Microsoft DirectX®: 9.29.952.3111 (June 2010)
  
2. Make sure that you have Wwise SDK installed with following component
   - SDK (Common)
   - SDK (Windows, Visual Studio 2015)
3. Download IntegrationDemo-cocos folder from GitHub
4. Copy 3. over to <Wwise SDK>\SDK\samples
   e.g.)  3.  -> c:\Wwise_v2015.1.4_build_5497\SDK\samples\
                 -> c:\Wwise_v2015.1.4_build_5497\SDK\samples\IntegrationDemo-cocos
5. Build 
   1) Launch Visual Studio 2015
   2) Open solution file (<Wwise SDK>\SDK\samples\IntegrationDemo\cocos2d-x\proj.win32\IntegrationDemo-cocos2d-x.sln)
   3) From menu. [Build],[Build Solution]
   