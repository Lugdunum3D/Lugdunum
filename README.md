<p align="center"><img src="https://cdn.rawgit.com/Lugdunum3D/Visual-Identity/e9baec645385732329d83c948c8004aeca5f6b3f/logo/dist/long.svg" width="70%"></p>


---

# Lugdunum3D

A modern cross-platform 3D rendering engine built with Vulkan and modern C++14. Documentation and more are available [on the homepage](https://lugdunum3d.github.io/).

# Building

Our build system is CMake and this repository contains the appropriate build files to build on Linux, Windows and Android using a C++14 compiler (see [tested toolchains](#tested-toolchains)). Third party dependencies are built using [Lugdunum-ThirdParty](https://github.com/Lugdunum3D/Lugdunum-ThirdParty) and the `thirdpary.yml` file located at the root of this repository. [Follow the instructions on our documentation](https://lugdunum3d.github.io/doc/build.html) to build Lugdunum.

## <img src="https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg" width="16"> Linux
[![CircleCI](https://img.shields.io/circleci/project/github/Lugdunum3D/Lugdunum/master.svg?label=build%20(master)&maxAge=300)](https://circleci.com/gh/Lugdunum3D/Lugdunum/tree/master) [![CircleCI](https://img.shields.io/circleci/project/github/Lugdunum3D/Lugdunum/dev.svg?label=build%20(dev)&maxAge=300)](https://circleci.com/gh/Lugdunum3D/Lugdunum/tree/dev)

## <img src="https://upload.wikimedia.org/wikipedia/commons/e/ee/Windows_logo_%E2%80%93_2012_%28dark_blue%29.svg" width="16"> Windows 
[![CircleCI](https://img.shields.io/appveyor/ci/Lugdunum/lugdunum/master.svg?label=build%20(master)&maxAge=300)](https://circleci.com/gh/Lugdunum3D/Lugdunum/tree/master) [![CircleCI](https://img.shields.io/appveyor/ci/Lugdunum/lugdunum/dev.svg?label=build%20(dev)&maxAge=300)](https://circleci.com/gh/Lugdunum3D/Lugdunum/tree/dev)

# Examples

All the examples can be built using the `CMakeLists.txt` in the `./samples` directory.

## [Triangle](./samples/triangle)

<img src="./doc/triangle.jpg" height="96px" align="left">

The simplest of all. Renders a colored triangle.
<br><br><br><br>

## [Cube](./samples/cube)

<img src="./doc/cube.jpg" height="96px" align="left">

This basic example draws a procedurally generated rotating cube.
<br><br><br><br>

## [Sphere PBR](./samples/sphere_pbr)

<img src="./doc/sphere_pbr.jpg" height="96px" align="left">

This example draws a procedurally generated rotating sphere using PBR, which is lit by 4 directional lights.
<br><br><br>

## [Spheres PBR](./samples/spheres_pbr)

<img src="./doc/spheres_pbr.jpg" height="96px" align="left">

This basic examples draws an array of procedurally generated spheres using PBR, the whole array shows a gradient of the PBR parameters.
The spheres are also lit by 4 directional lights, and a free moving camera allows to explore the different materials and reflections.
<br><br>

## [Hello](./samples/hello)

<img src="./doc/hello.jpg" height="96px" align="left">

Displays the classic [*Battle Damaged Sci-fi Helmet - PBR* by theblueturtle_](https://sketchfab.com/models/b81008d513954189a063ff901f7abfe4) with a skybox and a free moving camera.
<br><br><br><br>

# Tests

Tests can be enabled using the `BUILD_TESTS` CMake flag.

# Tested toolchains

| Compiler            | Operating System                     | Architecture | Version String |
|---------------------|--------------------------------------|--------------|----------------|
| Clang 3.8           | Android NDK r14b with Unified Header | arm64-v8a    | Android clang version 3.8.275480  (based on LLVM 3.8.275480) |
| Visual Studio 2017  | Windows 10 Build 16170               | x64          | MSVC 19.10.25017.0 |
| GCC 6.2.0           | Linux Ubuntu 16.04.1 with kernel 4.8 | x64          | gcc version 6.2.0 20160901 |
| GCC 6.2.0           | Linux Tegra-Ubuntu 4.4.15            | aarch64      | gcc version 6.2.0 20160901 |
