# Dependencies for Lugdunum

## Introduction

Lugdunum depends on many different libraries / projects in order to work properly.
You can find [here](https://github.com/Lugdunum3D/Lugdunum-ThirdParty "third party lugdunum repository") compiled versions, ready to use to compile Lugdunum and get started quickly.

## List of dependencies

* [Assimp](http://assimp.org/):  _Open Asset Import Library (short name: Assimp) is a portable Open Source library to import various well-known 3D model formats in a uniform manner._
* [Fmt](http://fmtlib.net/latest/index.html):  _fmt (formerly cppformat) is an open-source formatting library. It can be used as a safe alternative to printf or as a fast alternative to C++ IOStreams._
* [Vulkan](https://www.khronos.org/vulkan/):  _Vulkan is a new generation graphics and compute API that provides high-efficiency, cross-platform access to modern GPUs used in a wide variety of devices from PCs and consoles to mobile phones and embedded platforms._

## Optional dependencies

All our code is covered by different tests through the Googletest / Googlemock framework.
You can find the sources of the framework at the [following link](https://github.com/google/googletest) or in our [third party repository](https://github.com/Lugdunum3D/Lugdunum-ThirdParty)


## How it works

All the dependencies can be found in our [ThirdParty repository](https://github.com/Lugdunum3D/Lugdunum-ThirdParty), which is added as a submodule of the main repository.

It is planned to add an utility script to update and compile all the dependencies at once, but as of now, it is still a manual task.
