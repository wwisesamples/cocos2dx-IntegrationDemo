LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_PATH is "proj.android/jni"
#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
#$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
#$(call import-add-path,$(LOCAL_PATH)/../../ocos2d/cocos)

#LOCAL_MODULE := cocos2dcpp_shared
#LOCAL_MODULE_FILENAME := libcocos2dcpp

#hideo added -> 
ifndef AK_CONFIG
AK_CONFIG := Profile
endif

ifneq ($(AK_CONFIG), Debug)
NDK_DEBUG := 1
endif

AK_ANDROID := 1

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


#LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

#include $(BUILD_SHARED_LIBRARY)

#$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

# hideo added

include $(CLEAR_VARS)
#SDK_LIB_DIR := ../../../../../../../$(TARGET_PLATFORM)_$(TARGET_ARCH_ABI)/$(AK_CONFIG)/lib
#SDK_LIB_DIR := $(LOCAL_PATH)/$(TARGET_PLATFORM)_$(TARGET_ARCH_ABI)/$(AK_CONFIG)/lib
SDK_LIB_DIR := ${WWISESDK}/$(TARGET_PLATFORM)_$(TARGET_ARCH_ABI)/$(AK_CONFIG)/lib
$(warning $(SDK_LIB_DIR))
ifneq ($(AK_CONFIG), Release)
	LOCAL_MODULE            := CommunicationCentral
	LOCAL_SRC_FILES         := $(SDK_LIB_DIR)/libCommunicationCentral.a 
	include $(PREBUILT_STATIC_LIBRARY)
	include $(CLEAR_VARS)
else
	LOCAL_EXPORT_CFLAGS 	+= -DAK_OPTIMIZED
endif

LOCAL_MODULE            := AkMemoryMgr
LOCAL_SRC_FILES         := $(SDK_LIB_DIR)/libAkMemoryMgr.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSoundEngine
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSoundEngine.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSpatialAudio
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSpatialAudio.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkMeterFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkMeterFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkRecorderFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkRecorderFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkReflectFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkReflectFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkPeakLimiterFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkPeakLimiterFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkHarmonizerFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkHarmonizerFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkStereoDelayFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkStereoDelayFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkMusicEngine
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkMusicEngine.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkTimeStretchFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkTimeStretchFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := McDSPLimiterFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libMcDSPLimiterFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkConvolutionReverbFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkConvolutionReverbFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkCompressorFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkCompressorFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkFlangerFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkFlangerFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkPitchShifterFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkPitchShifterFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSoundSeedImpactFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSoundSeedImpactFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := McDSPFutzBoxFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libMcDSPFutzBoxFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkRoomVerbFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkRoomVerbFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSineSource
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSineSource.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSilenceSource
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSilenceSource.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSynthOne
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSynthOne.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkParametricEQFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkParametricEQFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkVorbisDecoder
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkVorbisDecoder.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkToneSource
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkToneSource.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkExpanderFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkExpanderFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkGuitarDistortionFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkGuitarDistortionFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSoundSeedWoosh
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSoundSeedWoosh.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkTremoloFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkTremoloFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkMatrixReverbFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkMatrixReverbFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkStreamMgr
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkStreamMgr.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkDelayFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkDelayFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkAudioInputSource
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkAudioInputSource.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkSoundSeedWind
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkSoundSeedWind.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AkGainFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAkGainFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AuroHeadphoneFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAuroHeadphoneFX.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := AuroPannerMixer
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libAuroPannerMixer.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := CrankcaseAudioREVModelPlayerFX
LOCAL_SRC_FILES 		:= $(SDK_LIB_DIR)/libCrankcaseAudioREVModelPlayerFX.a
include $(PREBUILT_STATIC_LIBRARY)

LOCAL_MODULE            := zip
LOCAL_SRC_FILES         := $(SDK_LIB_DIR)/libzip.a 
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
 
LOCAL_ARM_MODE   := arm

#LOCAL_MODULE    := IntegrationDemo
LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libcocos2dcpp


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../proj.android
LOCAL_C_INCLUDES += ${WWISESDK}/samples/SoundEngine/Android
LOCAL_C_INCLUDES += ${WWISESDK}/samples/SoundEngine/Common
LOCAL_C_INCLUDES += ${WWISESDK}/samples/SoundEngine/Android/libzip/lib
LOCAL_C_INCLUDES += ${WWISESDK}/include 
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../Common
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../MenuSystem
LOCAL_C_INCLUDES += ${WWISESDK}/samples/IntegrationDemo/Android
LOCAL_C_INCLUDES += ${WWISESDK}/samples/IntegrationDemo/cocos2d-x/cocos2d/cocos
LOCAL_C_INCLUDES += ${WWISESDK}/samples/IntegrationDemo/cocos2d-x/cocos2d/cocos/audio/include
LOCAL_C_INCLUDES += ${WWISESDK}/samples/IntegrationDemo/cocos2d-x/cocos2d/external
LOCAL_C_INCLUDES += ${WWISESDK}/samples/IntegrationDemo/cocos2d-x/cocos2d/extensions
LOCAL_C_INCLUDES += ${WWISESDK}/samples/IntegrationDemo/cocos2d-x/cocos2d/external/chipmunk/include/chipmunk
LOCAL_C_INCLUDES += ${WWISESDK}/samples/SoundEngine/Android/libzip/lib


LOCAL_SRC_FILES := main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/WwiseWrapper.cpp \
                   ${WWISESDK}/samples/SoundEngine/Common/AkFileLocationBase.cpp \
                   ${WWISESDK}/samples/SoundEngine/Common/AkFilePackage.cpp \
                   ${WWISESDK}/samples/SoundEngine/Common/AkFilePackageLUT.cpp \
                   ${WWISESDK}/samples/SoundEngine/Android/AkFileHelpers.cpp \
                   ${WWISESDK}/samples/SoundEngine/Android/AkDefaultIOHookBlocking.cpp \
                   ${WWISESDK}/samples/IntegrationDemo/Common/Helpers.cpp \
                   ../../Classes/SceneBase.cpp \
                   ../../Classes/SceneMain.cpp \
                   ../../Classes/SceneLocalization.cpp \
                   ../../Classes/SceneDialogueMenu.cpp \
                   ../../Classes/SceneDynamicDialogue.cpp \
                   ../../Classes/SceneFootsteps.cpp \
                   ../../Classes/SceneInteractiveMusic.cpp \
                   ../../Classes/SceneMarkers.cpp \
                   ../../Classes/SceneMusicCallbacks.cpp \
                   ../../Classes/SceneRTPCCarEngine.cpp \
                   ../../Classes/ScenePositioning.cpp \
                   ../../Classes/SceneExternalSources.cpp \
                   ../../Classes/SceneBGM.cpp
                    
#                   ../../Classes/SceneMicrophone.cpp \
                    
LOCAL_LDLIBS := -llog -lOpenSLES -landroid -lEGL -lGLESv1_CM
LOCAL_CFLAGS := -DLUA_USE_POSIX
LOCAL_CFLAGS += -DAK_DEBUG -DCOCOS_INTEGRATION
#-DINTEGRATIONDEMO_MICROPHONE
#-DINTEGRATIONDEMO_BGM


####LOCAL_STATIC_LIBRARIES := cocos2dx_static

#LOCAL_STATIC_LIBRARIES := cocos2dx_static
#LOCAL_STATIC_LIBRARIES += android_native_app_glue
#LOCAL_WHOLE_STATIC_LIBRARIES += gnustl_static
#LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# ’Ç‰Á
#LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
#LOCAL_WHOLE_STATIC_LIBRARIES += android_native_app_glue


LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += gnustl_static

ifneq ($(AK_CONFIG), Release)
#	LOCAL_STATIC_LIBRARIES += CommunicationCentral 
	LOCAL_WHOLE_STATIC_LIBRARIES += CommunicationCentral 
endif
#LOCAL_WHOLE_STATIC_LIBRARIES  += AkMusicEngine \

LOCAL_STATIC_LIBRARIES += AkMusicEngine \
AkSoundEngine \
AkAudioInputSource \
AkCompressorFX \
AkConvolutionReverbFX \
AkDelayFX \
AkExpanderFX \
AkFlangerFX \
AkGainFX \
AkGuitarDistortionFX \
AkHarmonizerFX \
AkMatrixReverbFX \
AkMemoryMgr \
AkMeterFX \
AkParametricEQFX \
AkPeakLimiterFX \
AkPitchShifterFX \
AkRecorderFX \
AkReflectFX \
AkRoomVerbFX \
AkSilenceSource \
AkSineSource \
AkSoundSeedImpactFX \
AkSoundSeedWind \
AkSoundSeedWoosh \
AkSpatialAudio \
AkStereoDelayFX \
AkStreamMgr \
AkSynthOne \
AkTimeStretchFX \
AkToneSource \
AkTremoloFX \
AkVorbisDecoder \
AuroHeadphoneFX \
AuroPannerMixer \
McDSPFutzBoxFX \
McDSPLimiterFX \
CrankcaseAudioREVModelPlayerFX \
zip

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

#$(call import-module,android/native_app_glue)
#$(call import-module,2d)
#$(call import-module,3d)
#$(call import-module,audio/android)
#$(call import-module,Box2D)
#$(call import-module,extensions)

