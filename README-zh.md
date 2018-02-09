# Wwise cocos2d-x 集成演示

本 fork 基于[原版社区项目](https://github.com/wwisesamples/cocos2dx-IntegrationDemo)，演示 Wwise 与 Cocos2d-x 引擎的集成方法。本 fork 只有以下目标:

1. 临时修复演示程序与新版 Wwise 和 Cocos2d-x 的兼容性问题，但不保证时效；
2. 适当添加新的 Wwise 官方演示中的功能；
3. 保持代码整洁，起示范效果。

[Wwise SDK 集成演示](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html)在移动平台上采用虚拟手柄的交互方式，在桌面平台采取纯键盘交互，[本 fork 的原项目作者](https://github.com/wwisesamples)改进了用户交互体验，实现了更自然的基于触屏和鼠标手势的互动，Footsteps（脚步）和 Positioning（定位）演示是比较好的例子.

本演示支持以下平台:

- Android
- iOS
- Mac
- Windows

## 更新: 2018年2月9日

内容

- 更新到支持 Wwise 2017.1 和 Cocos2d-x 3.16；
- 添加了此前禁用的背景音乐功能演示；
- 添加了新插件注册，比如 Wwise Reflect and Wwise Recorder；
- 修正了: 退出脚步声演示时的崩溃；
- 简化了 repo 的文件和目录结构；
- 大量的重构，增进了跨平台和平台相关的代码区分度和可读性。

在以下环境中做了测试：

- [Wwise 2017.1.4.6407](https://www.audiokinetic.com/download/)
- [cocos2d-x 3.16](http://www.cocos2d-x.org/download)
- Android 6.0.1，华为荣耀畅玩5X（KIW-AL10）
- iOS 11.2.5，iPhone 6
- macOS Sierra 10.12.6
- Windows 10
- Xcode 9.2
- Visual Studio 2017
- JDK 1.8，Android SDK API 24，NDK r12b，Apache ANT 1.8.2

## 安装演示

首先要克隆本演示。

```
git clone https://github.com/cocos2dx-IntegrationDemo.git wwise-cocos-demo
```

这其实是一个配置好的 Cocos2d-x 应用的工程。

接下来，因为演示代码依赖于[Wwise SDK IntegrationDemo](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html)的部分代码，所以需要安装兼容的 Wwise 版本和目标平台的 Wwise SDK。

最后，将演示拷入 SDK IntegrationDemo 文件夹。完成后，本地克隆的根目录（如`wwise-cocos-demo`）会位于`<Wwise Root>/SDK/samples/IntegrationDemo/`下面，也即其路径会为：

`<Wwise Root>/SDK/samples/IntegrationDemo/wwise-cocos-demo`

  
## 构建本演示

要运行本演示，需要先从源码构建。

各个支持的平台都有对应的 IDE 解决方案或者工程文件。同时克隆里也有一个完整的 Cocos2d-x 引擎版本，毕竟是个标准的 cocos 工程。可以一键或一行命令构建任何平台的演示加上 Cocos2d-x 引擎本身。

如果想用自己的 Cocos2d-x 版本，则需要替换`cocos2d`目录，再[重新手动设置工程](https://github.com/cocos2d/cocos2d-x/blob/v3/README.md#how-to-start-a-new-game).

每个平台有两个构件配置。*Debug*（调试）版可以用 Wwise 设计工具来远程分析，不过是没有编译优化的版本。*Release*（发布）版是有编译优化的版本，但不能连接分析器。

### Windows 平台

必备环境

  - Windows 7 或更高版本
  - Visual Studio 2017
  - [DirectX SDK](https://www.microsoft.com/en-ca/download/details.aspx?id=6812)
  - Wwise SDK，需安装组件：SDK (Windows, Visual Studio 2017)

DirectX 相关备注

- 安装 DirectX SDK，版本以 Wwise 的 [平台需求](https://www.audiokinetic.com/library/edge/?source=SDK&id=reference__platform.html)文档为准。
- 重启 Windows 资源管理器或者直接重启电脑，保证环境变量 *DXSDK_DIR* 生效。演示的 Visual Studio 工程依赖该变量


构建步骤

- 找到`<Wwise Root>/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.win32/IntegrationDemo-cocos2d-x.sln`这个解决方案，并用 Visual Studio 打开构建之；
- 用兼容的 Wwise 版本打开 IntegrationDemo 的 Wwise 工程；
- 沿用默认 SoundBank 路径，生成 Windows 下的 SoundBank。


### Mac 平台

必备环境

- Xcode 兼容版本，并登录了开发者身份；
- Wwise SDK，需安装组件: SDK (macOS)

构建步骤

- 用 Xcode 打开`<Wwise Root>/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.ios_mac/IntegrationDemo-cocos.xcodeproj`工程；
- 构建`IntegrationDemo-cocos-desktop`这个 Scheme；
- 用兼容的 Wwise 版本打开 IntegrationDemo 的 Wwise 工程；
- 沿用默认 SoundBank 路径，生成 Mac 下的 SoundBank。


### iOS 平台

必备环境

- Xcode 兼容版本，并登录了开发者身份；
- Wwise SDK，需安装组件: SDK (iOS)


构建步骤

- 用 Xcode 打开`<Wwise Root>/SDK/samples/IntegrationDemo/cocos2dx-demo/proj.ios_mac/IntegrationDemo-cocos.xcodeproj`工程；
- 构建`IntegrationDemo-cocos-mobile`这个 Scheme；
- 用兼容的 Wwise 版本打开 IntegrationDemo 的 Wwise 工程；
- 沿用默认 SoundBank 路径，生成 iOS 下的 SoundBank。


### Android 平台

必备环境

- Android SDK/NDK/Apache ANT
- Python 2.7.x
- Wwise SDK，需安装组件: SDK (Android)

构建步骤

- 打开终端: `cd cocos2dx-demo/proj.android`；
- [运行 Cocos 命令设置 Android 环境变量](https://github.com/cocos2d/cocos2d-x/blob/v3/README.md#how-to-start-a-new-game);
- 编译工程
	- Debug: `cocos compile -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1`
	- Release: `cocos compile -p android`
- 用兼容的 Wwise 版本打开 IntegrationDemo 的 Wwise 工程；
- 沿用默认 SoundBank 路径，生成 Android 下的 SoundBank。
- 安装 APK 并运行 
	- Debug：`cocos run -p android -m debug NDK_DEBUG=1 V=1 NDK_LOG=1`
	- Release: `cocos run -p android`


## 如何操作演示

用演示的菜单系统前往各个演示项目。可以用键盘、鼠标或者触屏来控制演示。


## 演示内容

浏览[SDK IntegrationDemo 文档](https://www.audiokinetic.com/en/library/edge/?source=SDK&id=soundengine__integration__samplecode.html)的`The Demos` 一节来了解各个演示项目。本演示和官方演示最大的区别在于控制方式。在移动平台上，本演示采用更直接的触屏手势，而非虚拟手柄，感谢 Cocos2d-x 的互动支持；在桌面平台上，本演示支持鼠标互动，而官方 SDK IntegrationDemo 不支持.


## 配套 Wwise 工程

在某目标平台上运行演示前需要生成该平台的 SoundBank。SoundBank 是 Wwise 工程的输出结果。本演示用到的 Wwise 工程和 SDK IntegrationDemo 的是同一个，位于`<Wwise Root>/SDK/samples/IntegrationDemo/WwiseProject`。

如要了解生成 SoundBanks 的细节，可以阅读 Wwise 用户文档。生成完毕之后，在默认路径下能找到 SoundBank，默认在 Wwise 工程下的`GeneratedSoundBanks`子文件夹下面。


## 演示代码和文件结构

和[原社区演示](https://github.com/wwisesamples/cocos2dx-IntegrationDemo)比较，本 Fork 中 Wwise 集成代码已重构过，会更便于理解各个平台需要的基本集成工作。这主要是通过分离跨平台和平台相关代码做到的。虽然仍然会有一些临时的平台分支代码，但量已经很小了。

以下是一些方便阅读代码的线索：

- `Classes/Scene*.h`和`Classes/Scene*.cpp` 加上 Cocos 的 AppDelegate 模版代码是演示的应用层代码。
- `Classes/WwiseWrapper.h`和`Classes/WwiseWrapper.cpp`处理跨平台的 Wwise 引擎生命周期。
- 平台相关代码和 IDE 解决方案或工程文件全部在以平台命名的子目录下面
	- `cocos2dx-demo/proj.android/`
	- `cocos2dx-demo/proj.ios_mac/ios/`
	- `cocos2dx-demo/proj.ios_mac/mac/
	- `cocos2dx-demo/proj.win32/
- `<platform>/WwisePlatformHelper.h`和 `<platform>/WwisePlatformHelper.cpp`处理平台相关部分的 Wwise 引擎生命周期。
- `<platform>/PlatformCocos.h`和`<platform>PlatformCocos.cpp`定义了一些演示中平台相关的全局变量和函数。这些是 SDK IntegrationDemo 中的 `Platform.h`和`Platform.cpp`的补充。

## 授权和技术支持

本 Fork 采用[原版社区演示](https://github.com/wwisesamples/cocos2dx-IntegrationDemo)的相同授权。

可以在 Issues 区提问但不保证能及时回答。
