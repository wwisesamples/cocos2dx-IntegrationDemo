# Wwise and cocos2d-x IntegrationDemo
This is a simple demo project that integrated Wwise in cocos2d-x platform. The Integration Demo application contains a series of demonstrations that show how to integrate various features of the sound engine in your game.

# Beta version released on Jan 8, 2016
Project has created & tested with
- Wwise 2015.1.4 Build 5497 
  https://www.audiokinetic.com/download/
- cocos2d-x 3.8.1
  http://www.cocos2d-x.org/download
  
# Building and Running the Demo
The Integration Demo binaries are available in the "$(Platform)\[Debug|Profile|Release]\bin" directory. If you would like to rebuild the application yourself, follow these steps:

- Windows
Confirm that the version of the DirectX SDK installed on your machine matches the one mentioned in Platform Requirements . 
Generate the Integration Demo SoundBanks for Windows in their default paths. 
Open the solution found in "samples\IntegrationDemo\Windows" and build using the desired configuration. 
To run the Integration Demo, simply launch the executable found in the directory mentioned above.

- Mac
Confirm that the version of XCode installed on your machine matches the one mentioned in Platform Requirements . 
Open the Xcode project found in "samples\IntegrationDemo\Mac" and build using the desired configuration. 
To run the Integration Demo, simply launch the executable found in the directory "Mac\[Debug|Profile|Release]\bin". Note.gif  Note: The banks are not included in the installer and have to be generated using the authoring tool.  

- iOS
Confirm that the version of the iOS/tvOS SDK installed on your machine matches the one mentioned in Platform Requirements . 
Open the Xcode project found in "samples\IntegrationDemo\iOS" or "samples\IntegrationDemo\tvOS" and build using the desired configuration. Note.gif  Note: The banks are not included in the installer and have to be generated using the authoring tool.  

- Android
Confirm the versions of SDK and tools for Android on your machine matches the one mentioned in Platform Requirements. 
Open the Wwise project in "samples\IntegrationDemo\WwiseProject" and generate the SoundBanks for Android in their default paths. 
Normally, banks should be packaged inside of your apk executable file. However, for simplicity sake, the banks are not packaged inside the IntegrationDemo apk. You will need to copy the android banks to the folder /sdcard/IntegrationDemo on your android device. If you don't have a SD card you can modify the path found in the file SDK/samples/IntegrationDemo/Android/platform.h to suit your needs. 
Load the eclipse project into your Eclipse workspace ( File > Import > Existing Project) 
Select "SDK/samples/IntegrationDemo/Android" as the root to the project 
Right-click on the integration demo project and select "Run as Android Application". 
Note.gif  Note: You will need to use the software keyboard or hardware keyboard to interact with the integration demo. The software keyboard can be opened by holding the menu button for 2 seconds.  

# Usage Instructions
You can navigate through the Integration Demo on Windows using either the keyboard, a connected controller or any DirectInput compatible device.

- To navigate between controls on a page, use the UP and DOWN arrow keys or the UP and DOWN buttons on a gamepad's directional pad. 
- To activate the selected control, hit the Enter key or the A/X button on a gamepad. 
- To go back a page in the menu, hit the Escape key or the B/O button on a gamepad. 
Certain controls (eg. Toggle Controls and Numeric Sliders) allow you to change values. To change their values, hit the LEFT and RIGHT arrow keys or the LEFT and RIGHT buttons on a gamepad's directional pad.

Tip.gif  Tip: The application has an online help feature! To access the Help page, press F1 on the keyboard or the START button on a gamepad.  

# The Demos
The code behind each demonstration can be found in the "samples\IntegrationDemo\DemoPages" directory. For example, the code for the Localization demo will be in the DemoLocalization.h and DemoLocalization.cpp files in that directory.

Tip.gif  Tip: Pertinent information about each demo can also be found in the Integration Demo application's online help.  

- Localization
This demo shows how to implement localized audio. Localized sound objects are found in language-specific SoundBanks in subdirectories in the SoundBank generation directory. We achieve the localization effect by unloading the current SoundBank and reloading the desired language-specific Soundbank.

Use the "Language" Toggle control to switch the current language and then press the "Say Hello" button to hear a greeting in the selected language.

For more information about languages and localization, see Integration Details - Languages and Voices

- Dynamic Dialogue
The Dynamic Dialogue demo runs through a series of tests that use Wwise's Dynamic Dialogue features. Each of these tests demonstrates a different control flow so that you can hear the effect it produces:

Test 1: Shows how to play a simple dynamic sequence using Wwise IDs 
Test 2: Like test 1, but uses strings instead of IDs. 
Test 3: Shows how to add an item to a dynamic playlist during playback. 
Test 4: Shows how to insert an item into the dynamic playlist during playback, 
Test 5: Shows what happens when an item is added to an empty playlist. 
Test 6: Shows how to use the "Stop" call on a dynamic sequence. 
Test 7: Shows how to use the "Break" call on a dynamic sequence. 
Test 8: Shows how to use the "Pause" and "Resume" calls on a dynamic sequence. 
Test 9: Shows how to use the "Delay" call when enqueuing an item to a dynamic sequence. 
Test 10: Shows how to clear a playlist during playback. 
Test 11: Shows what happens when the playlist is stopped and cleared. 
Test 12: Shows what happens when "Break" is called on a playlist and it cleared. 
Test 13: Shows what happens when a playlist is paused and cleared. 
Test 14: Shows how to use a callback function with custom parameters when working with dynamic dialogue. 
Test 15: Shows how to use a callback to perform tasks (in this case, to cancel playback after 3 items have played). 
Test 16: Shows how to use a callback to perform tasks (in this case, play a second sequence after the first sequence ends). 
Test 17: Show how to use Wwise events in conjunction with dyanamic dialogue. 
For more information about Dynamic Dialogue, see Integration Details - Dynamic Dialogue

- RTPC (Car Engine)
This demo shows how to use RTPCs. The RPM numeric slider is linked with an RTPC value (RPM) associated with the engine. Press the "Start Engine" button to start/stop car engine audio and use the RPM slider to change the RTPC value and hear the effect.

For more information about RTPCs, see Integration Details - RTPCs

- Footsteps
This demo shows various ways to implement footsteps in a game. It also show surface-driven bank management to minimize both media and metadata memory when a surface isn't in use. Finally, this demo also shows a very simple case of environmental effects.

In this example, the footstep sounds are modified by 3 variables: surface, walking speed and walker weight.

Surface issues (Surface switch) The surface will change the actual properties of the sounds so it can't be simulated. However, footsteps being impact sounds, we chose to use SoundSeed Impact for most of the surface types. Each surface has very specific resonance characteristics that SS Impact can modulate. SS Impact will give a large variety of sounds out of a small subset of real sounds, thus saving space. 
Walking Speed issues (Footstep_Speed RTPC) This project supports a smooth transition from walking to running in almost all cases. For this variable, we assume the following: the faster you walk, the shorter is the footstep and the harder you hit the ground. This translates in Pitch and Volume changes respectively. Look for RTPC on these parameters in the project. The Speed RTPC is driven directly by the joystick displacement in this demo. 
Walker Weight issues (Footstep_Weight RTPC) The footstep structure supports various walker weights. We assume that in real life, a heavier walker will have a longer footstep and that it will be more muffled. This translates in Pitch and LPF changes respectively. Look for RTPC on these parameters in the project. 
With each surface, we show a different way of dealing with the sound samples and variables. These are only suggestions and ideas that you can use in your own structure.

Gravel Our gravel samples are very noisy so they don't go well with SoundSeed Impact. Also, they are very similar, so it won't give us anything more to have a lot of samples of this surface. More variation is obtained with a bit of Volume, LPF and Pitch randomizations. The Weight influance is done through the EQ effect with its gain parameters driven by the Weight RTPC. For light footsteps, the higher frequencies are boosted and the reverse for heavy footsteps. Note the RTPCs effect on pitch and volume. 
Metal The metal surface is a textbook example of SoundSeed Impact usage; there is lots of resonance. In our samples, we could easily identify a heel impact followed by the toe impact. In order to have more variation, we split each sample in two. This allows us to independantly randomize the pitch of each section. We recombine both using a sample-accurate transition sequence. This gives us 25 basic combinations out of our 5 original samples. Add some pitch randomization and the natural randomization of SS Impact and we get a good variety of sounds. The Weight and Speed RTPC, drive the SS Impact parameters as well as the basic Pitch and LPF. 
Wood For the wood surface, the walking and running samples were very different. Same also for Heavy and Light footsteps. So this was organized in a more traditional switch hierarchy. Both switch containers are driven by a RTPC-driven switch (look in the GameSync tab, Footstep_Gait and Footstep_Weight). The wood surface works good with SS Impact as well. 
Dirt Samples for walking and running on this surface were somewhat similar so we decided to do the transition with a Blend container. RTPCs on Pitch and Volume were used to take the Weight in account. 

- Bank management
In this demo, the banks were divided in four media banks (one per surface). We divided the screen in 4 with a buffer zone between each surface where both banks are loaded. This is to avoid a gap in the footsteps due to bank loading. In the bank manager, look at the GameSync tab. Note that each surface bank includes only the corresponding surface switch. This will include only the hierarchy related to that switch in the bank, and nothing else. In a large game, this setup has the advantage of limiting the amount of unused samples in a particular scenario, thus limiting the memory used. For level or section based games, it is easy to identify the surfaces used as they are known from the design stage. For open games, this is more tricky and depend a lot on the organization of your game but can still be acheived. For example, it is useless to keep the "snow and ice" surface sounds in memory if your player is currently in a warm city and won't be moving toward colder settings for a long time.

- Subtitles/Markers
This demo shows how you can set up a callback function to receive notification when markers inside a sound file are hit. For this demonstration, we are using the markers to synchronize subtitles with the audio track.

For more information on markers, see Integrating Markers

- Music Callbacks
Music Sync Callback Demo - This demo shows how to use music callbacks in general. Beat and bar notifications are generated from music tempo and time signature information. Music Playlist Callback Demo - This example to force a random playlist to select its next item sequentially. The playlist item may be stopped via the callback as well. MIDI Callback Demo - Shows MIDI messages the game can receive using callbacks. MIDI messages include the MIDI notes, CC values, Pitch Bend, After Touch and Program Changes.

For more information on music callbacks, refer to Integration Details - Music Callbacks

- Interactive Music
This example uses a Music Switch container. Try switching the states by triggering the event listed in the demo page. Switching state might produce a result that is immediate or occur at the time specified in the rules of the music container.

- Motion
This is a multiplayer demonstration which shows how to integrate Wwise's motion engine into your game.

In this demonstration, each player has the option to either close a door in the environment or to shoot a gun that they are holding. A listener is set for each player which is active on the door game object as well as the player's own gun. This way, if any player closes the door in the environment, all players receive force feedback reactions. However, only the player who fired his weapon receives force feedback for that event.

Note.gif  Note: A player using a keyboard should plug in a gamepad to participate in this demo.  

For more information on the Wwise Motion Engine, see Integrating Wwise Motion

- Microphone/AudioInput
This demo shows how to record audio from a microphone and input it in the Wwise sound engine. In the Integration Demo select the "Microphone Demo". and speak into the microphone to hear your voice played back by the Wwise sound engine. Toggle the "Enable Delay" to hear an example of how audio data fed to the Audio Input plug-in can be processed like any other sound created in Wwise.

Each platform has a very different core API to access the microphone. Check the SoundInput and SoundInputMgr classes in the Integration Demo code to see how they interact with the AudioInput plugin.

The microphone sample was tested using "Logitech USB Microphones" on all platforms.

See also Audio Input Source Plug-in

- External Sources
This demo shows how to use external sources. Both buttons play the same sound structure, but set up at run-time with either sources "1", "2" and "3" or sources "4", "5" and "6".

For more information on the external sources feature, see Integrating External Sources.

Additionnally, the external sources are packaged in a file packager and loaded when opening the demo page. Refer to the Wwise Help for more information on the File Packager, and to the Streaming / Stream Manager chapter for more details on the run-time aspect of file packages.

# The Wwise Project
The Wwise project for this program is also available in "samples\IntegrationDemo\WwiseProject".

Note.gif  Note: The Wwise project for this program uses various audio file conversion formats, some of which may not be available depending on which platforms are supported by your Wwise installation. After opening the project in Wwise, you may see warnings such as:

'\Actor-Mixer Hierarchy\Dialogues\Captain_A\UNA-BM-AL_01\UNA-BM-AL_01' uses the conversion plugin 'XMA', which is not installed.You can make these messages disappear by changing the conversion format for all unavailable platforms to PCM. Refer to the following topic in the Wwise User Guide for more information: Finishing Your Project > Managing Platform and Language Versions > Authoring Across Platforms > Converting Audio Files. 
 

SoundBanks for this project are also installed with the SDK in the "samples\IntegrationDemo\WwiseProject\GeneratedSoundBanks" folder.

To regenerate the SoundBanks, make sure to do the following in the SoundBank Manager:

- Check all banks in the SoundBanks list 
- Check off all platforms that are being tested 
- Check all languages in the Languages list 
Once these settings are correct, you can click on Generate in the SoundBank Manager to generate the banks.


