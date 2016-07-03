Setting up IntegrationDemo & cocos2d-x for Android build environment on Windows 

1. Environment Requirements
  - Windows 7 or higher 
  - Android development tools
  - Wwise (2015.1.4 Build 5497 or higher) 
  - Cocos2d-x (v3.8.1 or higher)
   (For more info, go to http://www.cocos2d-x.org/wiki/Win32_Installation_and_Setup)
  - Android device (Tested witn Nexus 5x(Android 6.0.0))
  
2. Android development tools
 2.1 JDK 
   http://www.oracle.com/technetwork/java/javase/downloads/index.html
   Make sure your system environment variable
    e.g.)
    JAVA_HOME = C:\Program Files\Java\jdk1.8.0_65
   
 2.2 Android　SDK (r24.4.1)
   http://developer.android.com/intl/ja/sdk/index.html
 
 2.3 Eclipse (mars)
   https://eclipse.org/downloads/

 2.4 Android NDK (r10e)
   http://developer.android.com/intl/ja/ndk/downloads/index.html

3. cocos2d-x installation  
 3.1 cocos2d-x (3.8.1)
  http://cocos2d-x.org/download/version#Cocos2d-x
  e.g.) Extract to c:\cocos2d\cocos2d-x-3.8.1
  
 3.2 Python(2.7.1)   
  https://www.python.org/downloads/
  
 3.3 Apache Ant (1.9.5)
  http://ant.apache.org/
  
 3.4 cocos2d-x environment variables 
   Excute setup.py from command prompt
   e.g.) c:\cocos2d\cocos2d-x-3.8.1>setup.py

3. Wwise SDK
  3.1 Install Wwise SDK
    Download and launch Wwise Windows Web installer with SDK(Android) option
	* Make sure no space contain in the installation path
	e.g.) c:\Audiokinetic\Wwise_v2015.1.4_build_5497
	
  3.2 Merge this project with official Wwise SDK IntegrationDemo.
     -> c:\Audiokinetic\Wwise_v2015.1.4_build_5497\SDK\samples\IntegrationDemo
     Copy the cocos2d-x subfolder into the above folder. When files are in conflict, merge this folder's content into the files of the official build.

4. Build and run IntegrationDemo with cocos2d-x
 4.1 Go to command prompt in where IntegrationDemo-cocos android poject
     e.g.) D:\Audiokinetic\Wwise_v2015.1.4_build_5497\SDK\samples\IntegrationDemo\cocos2d-x\proj.android>
 4.2 Compile
     Debug   -> cocos compile -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1
     Release -> cocos compile -p android　
 4.3 Build & run 
     Debug   -> cocos run -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1
     Release -> cocos run -p android　

   
