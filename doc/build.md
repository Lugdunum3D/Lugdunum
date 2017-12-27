---
title: Building Lugdunum
menu:
- title: Documentation
  href: /doc
  class: documentation button button-green align-right
---

# Dependencies for Lugdunum

## Introduction

Lugdunum depends on many different libraries and projects in order to work properly. Hopefully, we put in place a system, [Thirdparty-Builder](https://github.com/Lugdunum3D/ThirdParty-Builder), that allows you to build quickly and automatically all the required depdendencies in order to get started.

On top of that, we already compile our dependencies on the CIs for the tree platform we support: Linux, Android and Windows.

Our build system (via CMake macros) is already configured to download automatically the up-to-date versions. Below is a documentation on how to use our thirdparty management system and how to build Lugdunum.


## List of dependencies

For an updated list of dependencies, you can consult the file [`thirdparty.yml`](https://github.com/Lugdunum3D/Lugdunum/blob/dev/thirdparty.yml) at the root for the repository. It is however designed to machine-readable first, so below is a more human-friendly list of what libraries we depend on:

### Libraries we use

* [ShaderC](https://github.com/google/shaderc): A collection of tools, libraries and tests for shader compilation. We use it to compile _glsl_ shaders at run time.
* [Fmt](http://fmtlib.net/latest/index.html): An open-source C++ formatting library. We use it in our Logger.
* [gltf2-loader](https://github.com/Lugdunum3D/glTF2-loader): One of our own libraries. We use it to load _glTF 2.0_ models in Lugdunum.
* [imgui](https://github.com/ocornut/imgui): A bloat-free Immediate Mode Greetype.org): The popUlafose i tanderingce for C++ with minimal dependencies. We fully support _imgui_ in our rendering engine. The end user has full access to _imgui_'s API.
* [imgui_club](https://github.com/ocornut/imgui_club): An _imgui_ companion library. It is used to have a nicer font rendering, mostly.
* [FreeType](https://www.fraphical user interface library for C++.
* [Vulkan](https://www.khronos.org/vulkan/): This is a new generation graphics and compute API that provides high-efficiency, cross-platform access to modern GPUs.

# Preparing your system

Building with Vulkan means building with the latest toolchains and the latest technologies available. In this section, we describe how to upgrade your system to a working development environment.

## <img src="https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg" width="24"> Linux

### Supported toolchains 

Target | Toolchain
------------ | -------------
Linux | gcc >= 6
Linux | clang >= 3.8

### Distribution specific prerequisites 

#### <img src="https://upload.wikimedia.org/wikipedia/commons/a/ab/Logo-ubuntu_cof-orange-hex.svg" width="16"> Ubuntu

These instructions were tested for Ubuntu 16.04 LTS. You can add the _ubuntu-toolchain-r/test_ repository on your machine to be able to install the most recent GCC version with the following commands:

```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
```

You can now install the dependencies needed to build Lugdunum: gcc-6, CMake and the development version of the X11 libraries:

```bash
sudo apt install gcc-6 cmake libxrandr-dev
```

As of July 2017, there is still not a Vulkan SDK package on Ubuntu, so you have to download and install it yourself. A complete documentation is already available [on the LunarG website](https://vulkan.lunarg.com/doc/sdk/latest/linux/getting_started.html), so we won't get into details here. Just make sure you have the `VULKAN_SDK` environment variable set, [as described here](https://vulkan.lunarg.com/doc/sdk/latest/linux/getting_started.html#user-content-set-up-the-runtime-environment), with the `x86_64` architecture.

#### <img src="https://upload.wikimedia.org/wikipedia/commons/a/a5/Archlinux-icon-crystal-64.svg" width="16"> Arch Linux

On Arch Linux, nice people packaged everything needed in the [vulkan-devel](https://www.archlinux.org/groups/x86_64/vulkan-devel/), so all you have to do is:

```
pacman -S vulkan-devel base-devel cmake
```

## <img src="https://upload.wikimedia.org/wikipedia/commons/e/ee/Windows_logo_%E2%80%93_2012_%28dark_blue%29.svg" width="24"> Windows

### Supported toolchains

Target | Toolchain
------------ | -------------
Windows 10 | [Visual Studio 2017](https://www.visualstudio.com/downloads/)

:::info
Visual Studio 2015 is **NOT** supported anymore, Visual Studio 2017 is the only supported toolchain.
::: 

To build Lugdunum on Windows, you'll need [CMake](https://cmake.org/download/). CMake will generate a Visual Studio solution that you can then open, and build.

## <img src="https://upload.wikimedia.org/wikipedia/commons/d/d7/Android_robot.svg" width="24"> Android

### Supported toolchains

Target | Toolchain
------------ | -------------
Android | NDK >= r14 ; clang ; Gradle >= 2.2


* [Android NDK r14+](https://developer.android.com/ndk/index.html)
* [Android Studio 3+](https://developer.android.com/studio/index.html)

The NDK and CLang can be added [in the SDK Manager in Android Studio](https://developer.android.com/studio/intro/update.html#sdk-manager).

:::info
At the time this documentation was written, Android was tested with the NDK v16.0.4442984, Android Studio 3.0.0.18 (canary), Gradle 4.3 (plugin version 3.0.0x) and CMake 3.6.4111459.
:::

Please note that [arm64-v8a](https://developer.android.com/ndk/guides/abis.html#arm64-v8a) is the only supported ABI and that we only support Android N (android-24) and up.


### About the Android NDK

As the GCC toolchain is now deprecated by Android's developers, the clang toolchain will be the only one supported in this project. Please note that we're also using [Unified Headers](https://github.com/android-ndk/ndk/wiki/Changelog-r14) from Android NDK 14.


# Cloning the repository

First, clone Lugdunum repository:

```
git clone git@github.com:Lugdunum3D/Lugdunum.git
```

Then, set your working directory to the cloned repository.

CMake will automatically download all the required dependencies (third-party libraries, models) as long as you specify `LUG_ACCEPT_DL=ON`. If this is set to true then CMake will check if you are missing third party libraries or models and will download them as needed.
If for some reason you wish to force CMake to retrieve the latest dependencies then you can simply delete the following folders _`thirdparty/`_, _`resources/models/`_ (which should hopefully be in the directory directly "above" _`build/`_) as well as _`build/models/`_.


# Building Lugdunum

## <img src="https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg" width="24"> Linux

Create a "build" directory (out-of-source build), `cd` in it and run `cmake`, as the list of commands bellow suggests:

```
mkdir build
cd build
cmake
    -DCMAKE_INDSTALL_PREFIX=<wanted_install_dir>
    -DLUG_ACCEPT_DL=ON
    ../
make install
```

:::info
If you installed multiple _gcc_ versions, you might want to add the `-DCMAKE_C_COMPILER=gcc-6 -DCMAKE_CXX_COMPILER=g++-6` flags to CMake to explicitely requires the most recent compilers.
Of course, `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` can be set to clang and clang++
:::

:::info
You can use `make -j$(nproc)` to make the compilation faster by running it on all your CPU cores.
:::

## <img src="https://upload.wikimedia.org/wikipedia/commons/e/ee/Windows_logo_%E2%80%93_2012_%28dark_blue%29.svg" width="24"> Windows

To build Lugdunum on Windows, you'll need [CMake](https://cmake.org/download/). CMake will generate a Visual Studio solution that you can then open, and build the project from.

By using the command line interface, you can generate the solution with:

```
mkdir build
cd build
cmake
    -G"Visual Studio 2017 15 Win64"
    -DLUG_ACCEPT_DL=ON
    ../
```

Then, open the generated `Lugdunum.sln` with Visual Studio and compile it.

## <img src="https://upload.wikimedia.org/wikipedia/commons/d/d7/Android_robot.svg" width="24"> Android

* Set the environment variable `ANDROID_SDK` to the location Android Studio downloaded the SDK.SDK
* Set the environment variable `ANDROID_NDK` to the path of the NDK. If you downloaded it via Android Studio, it should be located in `${ANDROID_SDK}/ndk-bundle`

For example:

```bash
export ANDROID_SDK=~/Android
export ANDROID_NDK=${ANDROID_SDK}/ndk-bundle
```

From a command line prompt, navigate to the folder you cloned Lugdunum in, then run the following commands:

```
~/Android/Sdk/cmake/3.6.4111459/bin/cmake \ 
    -G "Android Gradle - Unix Makefiles" \
    -DANDROID=true \
    -DANDROID_PLATFORM=android-24 \
    -DANDROID_STL=c++_shared \
    -DCMAKE_BUILD_TYPE=Debug \
    -DLUG_ACCEPT_DL=ON
    -DCMAKE_INSTALL_PREFIX=${ANDROID_NDK}/sources/lugdunum \
    -DCMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake
make install
```

:::info
Here the CMake path might be different of the one displayed in the command above, please double-check before executing the command and/or filing a bug report.
:::

# Building the samples

To build the samples, it is mandatory to have built and installed Lugdunum.

## <img src="https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg" width="24"> Linux

Navigate to the folder `./samples` and as earlier, create a "build" directory.

```
cd samples/
mkdir build
cd build
cmake
    -DLUG_ROOT=<path_where_you_installed_lugdunum>
    -DLUG_THIRDPARTY_DIR=../../thirdparty_THIRDPARTY_DIR
    ../
make
```

Each sample is built in the current directory. For example, the sample "hello" is located in the directory `samples/build/hello`.


## <img src="https://upload.wikimedia.org/wikipedia/commons/e/ee/Windows_logo_%E2%80%93_2012_%28dark_blue%29.svg" width="24"> Windows

As for Lugdunum's build, we are using CMake to generate a Visual Studio solution.

By using the command line interface, you can generate the solution that contains all the samples with:

```
cd samples
mkdir build
cd build
cmake -G"Visual Studio 15 2017 Win64" ..
```
Then, open the generated `samples.sln` with Visual Studio and compile them.


## <img src="https://upload.wikimedia.org/wikipedia/commons/d/d7/Android_robot.svg" width="24"> Android


Open the folder `samples/compiler/android` with Android Studio and let Gradle configure and sync the project.

:::info
If the NDK isn’t configured properly, you’ll have to tell Android Studio where to find it :
_`File > Project Structure > SDK Location > Android NDK Location`_
:::

The samples should now be available as targets and be buildable from Android Studio.

