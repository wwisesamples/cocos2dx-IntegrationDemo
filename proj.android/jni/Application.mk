APP_STL := gnustl_static

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic


ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1 
#  APP_CPPFLAGS += -D_DEBUG
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif

# hideo
APP_PLATFORM := $(ANDROID_PLATFORM)
#APP_ABI := armeabi-v7a x86 arm64-v8a x86_64
APP_ABI := armeabi-v7a
#APP_ABI := arm64-v8a

#APP_ABI := x86
APP_CPPFLAGS += -Wno-error=format-security
APP_PLATFORM := android-9




