---
title: Quick Start
menu:
- title: Documentation
  href: /doc
  class: documentation button button-green align-right
---

# Introduction

Welcome to Lugdunum's quickstart tutorial! In this chapter, we run you through some examples on how to use our 3D engine for your projects.

As a reminder, please note that classes mentioned here are linked (in blue) to our [external documentation](https://lugdunum3d.github.io/doc/doxygen/annotated.html).

Here are a few examples of what our engine is capable of. These screenshots were rendered on desktop, but our engine will produce the same exact result on mobile platforms.

<img src="./doc/Lugdunum/firehydrants.png" width="80%">
> [The FireHydrant model, 4 different shader settings]{#fig:firehydrants}

<img src="./doc/Lugdunum/corsets.png" width="100%">
> [The Corset model, 5 different shader settings]{#fig:corsets}

<img src="./doc/Lugdunum/helmets.png" width="80%">
> [The Helmet model, 6 different shader settings]{#fig:helmets}

:::success
You can download these models online on the [LugBench-Models repository](https://github.com/Lugdunum3D/LugBench-Models). Please credit the original authors if you use them!
:::

## Requirements

* Have followed the "Lugdunum" chapter of our "Build" documentation.
* We will use CMake in this guide, but any build tool should work; we just don't support them.

## Samples

Most of the code snippets below are sourced from our [Samples in the Lugdunum repository](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples).

There are currently 7 samples:

Sample name         | Description
--------------------|--------------------------------------------------------
[Sample "base"](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples/base)| Shows you a basic `CMakeFile.txt` (empty project and boilerplate for your project).
[Sample "window"](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples/window)| This sample demonstrates how to create a window with our API.
[Sample "triangle"](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples/triangle) | Most basic Vulkan usage; an RGB triangle.
[Sample "cube"](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples/cube) | This basic example draws a procedurally generated rotating cube. It demonstrates how to manually add vertices to create basic 3D objects.
[Sample "sphere_pbr"](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples/sphere_pbr) | More advanced samples with texture loading. This example draws a procedurally generated rotating sphere using PBR, which is lit by 4 directional lights.
[Sample "spheres_pbr"](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples/spheres_pbr) | This basic example draws an array of procedurally generated spheres using PBR, the whole array shows a gradient of the PBR parameters. The spheres are also lit by 4 directional lights, and a free moving camera allows to explore the different materials and reflections.
[Sample "hello"](https://github.com/Lugdunum3D/Lugdunum/tree/dev/samples/hello) | Our ultimate sample. It always contains the latest feature we developed and displays the classic [*Battle Damaged Sci-fi Helmet - PBR* by theblueturtle_](https://sketchfab.com/models/b81008d513954189a063ff901f7abfe4) with a skybox and a free moving camera.

### Screnshots

<img src="./images/samples_screenshots.svg" width="100%"> 

### Building

Instructions for building the samples are detailed in the "Build" document in the "Lugdunum" chapter.

:::success
We encourage you to build them and read their code as you read this document; this helps to understand the code snippets below _in-context_, and it allows you to change the code to see the implications in real-time.
:::

We wish you a pleasant reading!

\pagebreak

# Building a project

To build the project with Lugdunum, you need a `CMakeLists.txt` similar to the following code listing. It may seem a little long, but each part is commented in an effort to simplify it as much as possible.

```cmake
cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# use macros
include(${CMAKE_SOURCE_DIR}/cmake/Macros.cmake)

# determine the build type
lug_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")


if(ANDROID)
    populate_android_infos()
endif()

# project name
project(my_project)

# define the executable
set(EXECUTABLE_NAME "my_project")

# set the path of thirdparty
lug_set_option(LUG_THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty" STRING "Choose the path for the thirdparty directory")

# use config
include(${PROJECT_SOURCE_DIR}/cmake/Config.cmake)

# find Vulkan
find_package(Vulkan)
if (NOT VULKAN_INCLUDE_DIR)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/vulkan")
        message(FATAL_ERROR "Can't find vulkan in the thirdparty directory")
    endif()

    set(VULKAN_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/vulkan/include)
    message(STATUS "Found Vulkan: ${VULKAN_INCLUDE_DIR}")
endif()

include_directories(${VULKAN_INCLUDE_DIR})

# find fmt
find_package(Fmt)

if (NOT FMT_INCLUDE_DIR)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/fmt")
        message(FATAL_ERROR "Can't find fmt in the thirdparty directory")
    endif()

    set(FMT_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/fmt/include)
    message(STATUS "Found Fmt: ${FMT_INCLUDE_DIR}")
endif()

include_directories(${FMT_INCLUDE_DIR})

# find Lugdunum

find_package(LUG QUIET COMPONENTS core graphics system window math)
if (NOT LUG_FOUND)
    if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/lugdunum")
        message(FATAL_ERROR "Can't find lugdunum in the thirdparty directory")
    endif()

    set(LUG_ROOT ${LUG_THIRDPARTY_DIR}/lugdunum)
    find_package(LUG REQUIRED COMPONENTS core graphics system window math)
endif()

include_directories(${LUG_INCLUDE_DIR})


# source files of your application
set(SRC
    src/Application.cpp
    src/main.cpp
)

# include files of your application
set(INC
    include/Application.hpp
)

# List of shaders in the directory ${LUG_RESOURCES_DIR}/shader
set(SHADERS
    gui.vert
    gui.frag
    skybox.frag
    skybox.vert
    filtercube.vert
    prefiltered.frag
    irradiance.frag
    genbrdflut.vert
    genbrdflut.frag
)

# List of resources in the directory ${LUG_RESOURCES_DIR}
set(LUG_RESOURCES
    shaders/forward/shader.vert
    shaders/forward/shader.frag
)

# List of resources in the directory ${CMAKE_SOURCE_DIR}/resources
set(OTHER_RESOURCES
    img/my_img.png
    models/my_model.gltf
)

# create target
if(LUG_OS_ANDROID)
    add_library(${EXECUTABLE_NAME} SHARED ${SRC} ${INC})
    set(EXT_LIBRARIES android)

    set(ANDROID_PROJECT_PATH ${CMAKE_SOURCE_DIR}/android/${EXECUTABLE_NAME})
    set(ANDROID_PROJECT_ASSETS ${ANDROID_PROJECT_PATH}/src/main/assets)
    set(ANDROID_PROJECT_SHADERS ${ANDROID_PROJECT_PATH}/src/main/assets/shaders)
else()
    add_executable(${EXECUTABLE_NAME} ${SRC} ${INC})
endif()

include_directories(include)
lug_add_compile_options(${EXECUTABLE_NAME})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${EXECUTABLE_NAME} ${LUG_LIBRARIES} Threads::Threads ${EXT_LIBRARIES})

# copy / build shaders
add_shaders(${EXECUTABLE_NAME} ${SHADERS})

# copy lugdunum resources
add_resources(${EXECUTABLE_NAME} lug_resources ${LUG_RESOURCES_DIR} ${LUG_RESOURCES})

# copy resources
add_resources(${EXECUTABLE_NAME} my_resources "${CMAKE_SOURCE_DIR}/resources" ${OTHER_RESOURCES})
```

:::info
You'll probably need the `cmake/` folder in your project, that you can directly copy from [our repository](https://github.com/Lugdunum3D/Lugdunum/tree/dev/cmake). You'll also need to use the `cmake/Macros.cmake` from the [LugBench directory](https://github.com/Lugdunum3D/LugBench/blob/dev/cmake/Macros.cmake) rather than the one provided by the Lugdunum repository.
:::

Then, you will need to use the [Third-party builder](https://github.com/Lugdunum3D/ThirdParty-Builder) to build the third-parties.
The [Third-party builder](https://github.com/Lugdunum3D/ThirdParty-Builder) needs an `.yml` describing all dependencies to build. All possible dependencies are listed in the [`thirdparty.yml`](https://github.com/Lugdunum3D/Lugdunum/blob/dev/thirdparty.yml) at the root of our repository.
Here is the thirdpary.yml you will need for a basic project:
```yaml=
lugdunum:                                                # Builder name
    repository:
        tag: 2d91bc649e701b87a187b305fe641e3eecde004a    # Commit hash
fmt:
    repository:
        tag: 07ed4215212324145bee94b94e34656923a4e9b4

imgui:
    repository:
        tag: 479e532f184061fee6af9a295a84bc6f5f263852

vulkan:
    repository:
        tag: 685295031d092db5417a5254e4f8b3e8024214cf
```

:::info
The CMakeLists.txt will search the third-parties in a directory named `thirdparty`
:::

# Creating a window

Let's see how simple it is to create a window with the Lugdunum Engine.

```cpp
int main() {
    lug::Window::Window::InitInfo windowInitInfo{                           
        800,                        // width
        600,                        // height
        "My window",                // title
        lug::Window::Style::Default // style
    };
    auto window = lug::Window::Window::create(initInfo);

    // ...

    return 0;
}
```

The first and second members of the structure are the size of the window's width and height.
The third member is the name you wish to give to your application.
The fourth member is the style of the window. It allows you to choose which decorations and features you wish to enable. You can use any combination of the following styles:

Style Flag                        | Description
--------------------------------- | ---------------------------------------------
`Window::Style::None`        | No decoration at all
`Window::Style::Titlebar`    | The window has a titlebar
`Window::Style::Resize`      | The window can be resized and has a maximize button
`Window::Style::Close`       | The window has a close button
`Window::Style::Fullscreen`  | The window is shown in fullscreen mode
`Window::Style::Default`     | It is a shortcut for `Titlebar | Resize | Close`

# Handling events

Now that the window is created you may want to handle events.
To do this, it is as simple as looping while the window is open and retrieving events that have been received.

```cpp
int main() {
    lug::Window::Window::InitInfo windowInitInfo{                           
        800,                        // width
        600,                        // height
        "My window",                // title
        lug::Window::Style::Default // style
    };
    auto window = lug::Window::Window::create(initInfo);

    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {

            // ...        
        }
        // ...
    }

    return 0;
}
```

```cpp
while (window->isOpen()) {

}
```

This line ensures that our application keeps running while our window is open. If the window's state ever changes then our application ends the loop and ends.

```cpp
lug::Window::Event event;
while (window->pollEvent(event)) {

}
```

To retrieve events we need an `Event struct` that we pass to `window->pollEvent(...)`.
Each time `pollEvent(...)` is called, the window returns the next available `Event` and discards it from its queue.
If there are no more events left to be handled, then it returns `false`.

## Window events

Here is a simple example where the user detects a [`Window::Event::Type::Close`](#lug::Window::Event::Type) events and then call the window's `window->close()` function which ends our application by exiting the while loop.

:::warning
Even if you do not care about events, you still need an event loop to ensure that the window works as intended.
:::

```cpp
int main() {
    lug::Window::Window::InitInfo windowInitInfo{                           
        800,                        // width
        600,                        // height
        "My window",                // title
        lug::Window::Style::Default // style
    };
    auto window = lug::Window::Window::create(initInfo);

    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {

            if (event.type == lug::Window::Event::Type::Close) {
                window->close();
            }
            
        }
    }
    
    return 0;
}
```

## Keyboard events

This is pretty much the same example as earlier, but in addition to detecting window events, the user also detects a keyboard event which leads to the same result.

```cpp
int main() {
    lug::Window::Window::InitInfo windowInitInfo{                           
        800,                        // width
        600,                        // height
        "My window",                // title
        lug::Window::Style::Default // style
    };
    auto window = lug::Window::Window::create(initInfo);

    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {

            if (event.type == lug::Window::Event::Type::Close) {
                window->close();
            }
        
            if (event.type == lug::Window::Event::Type::KeyPressed && event.key.code == lug::Window::Keyboard::Key::Escape) {
                window->close();
            }
            
        }
    }
    
    return 0;
}
```

## Mouse events

Finally here is an example that includes handling mouse events.
In this example, clicking the left mouse button does not do anything, but we are sure you will be able to come up with some creative ways to use this!

```cpp
int main() {
    lug::Window::Window::InitInfo windowInitInfo{                           
        800,                        // width
        600,                        // height
        "My window",                // title
        lug::Window::Style::Default // style
    };
    auto window = lug::Window::Window::create(initInfo);

    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {

            if (event.type == lug::Window::Event::Type::Close) {
                window->close();
            }
        
            if (event.type == lug::Window::Event::Type::KeyPressed && event.key.code == lug::Window::Keyboard::Key::Escape) {
                window->close();
            }
            
            if (event.type == lug::Window::Event::Type::ButtonPressed && event.button.code == lug::Window::Mouse::Button::Left) {
            // ...
            }
        }
    }
    
    return 0;
}
```

# Logger

## Initialization
First, you need to initialize an instance of Logger, with the name of the instance (you can choose any string you want here: it allows you to know where the logs come from in the future).

```cpp
lug::System::Logger::makeLogger("myLogger");
```

When you have an instance of the logger, you have to attach a handler for each output to which you want to log. For example, if you need the standard output:

```cpp
LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::StdoutHandler>("stdout"));
```

If you need to log on Android device:

```cpp
LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::LogCatHandler>("logcat"));
```

Below is a table that describes what handlers are available to you and in which circumstances you may use them.

Class                                           | Description
----------------------------------------------- | -----------------------------------------
LogCatHandler                                   | Android
StdoutHandler                                   | Standard output
StderrHandler                                   | Error output
FileHandler                                     | File
 

## Display a message Log

To use the logger, you have to use one of this following methods:

 Display message type    | Example usage
-------------------------|-------------------------------------------------
 `debug`                 | `LUG_LOG.debug("Debug info: {}", debugValue);`
 `info`                  | `LUG_LOG.info("Starting the app");`
 `warn` (warning)        | `LUG_LOG.warn("Something wrong happened");`
 `error`                 | `LUG_LOG.error("An error occured");`
 `fatal` (fatal error)   | `LUG_LOG.fatal("Fatal error");`
 `assrt` (assert)        | `LUG_LOG.assrt("Assert level logging");`

## Modules
The Vulkan API defines many different features (more information [on the Khronos group website](https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#features-features)) that must be activated at the creation of the device. 

Lugdunum abstracts these features with the use of _modules_.

Modules are a bunch of pre-defined set of mandatory features required by the renderer to run specific tasks.

For example, if you want to use tessellation shaders, you have to specify the module `tessellation` in the structure [`InitInfo`](#lug::Graphics::Graphics::InitInfo) during the initialization phase.

You can specify two type of modules: `mandatoryModules` and `optionalModules`. The former ensures the initialization fails if any mandatory module is not supported by the device, whereas the latter treats them as optional, as the name implies.

:::info
You can query which optional modules are active with lug::Graphics::getLoadedOptionalModules()
:::

## Choosing a device

Usually, calling [`Application::init()`](#lug::Core::Application::init()), lets Lugdunum decide which device to use. The engine prioritizes a discrete device that supports all the mandatory modules. If the engine is unable to find such a device, the call fails.

Alternatively the user can decide which device he or she wishes to use, as long as the device meets the minimum requirements for all the mandatory modules. For that, you can override [`Application::init()`](#lug::Core::Application::init()) and use two others methods named [`Application::beginInit(int argc, char *argv[])`](#lug::Core::Application::beginInit()) and [`Application::finishInit()`](#lug::Core::Application::finishInit()).

Between the two you can set what we call *preferences*, and choose the device that you want that way.

```cpp
bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::beginInit(argc, argv)) {
        return false;
    }

    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    auto& chosenDevice : vkRender->getPhysicalDeviceInfos();

    for (auto& chosenDevice : vkRender->getPhysicalDeviceInfos()) {
        if (...)
        {
            vkRender->getPreferences().device = chosenDevice; 
            break;
        }
    }

    if (!lug::Core::Application::finishInit()) {
        return false;
    }
}
```

In the example shown above the user retrieves the list of all available device, decides which one they wish to use and then sets that one as the device to use by default, setting it with the line `vkRender->getPreferences().device = chosenDevice;`.
If the device does not meet the minimum requirements [`Application::finishInit()`](#lug::Core::Application::finishInit()) fails.

# Using the builders

For most of the lug::Graphics objects, Lugdunum is using the builder pattern within the namespace lug::Graphics::Builder.
For the sake of example, we will take the builder of [`Graphics::Render::Camera::Camera`](#lug::Graphics::Render::Camera::Camera).

Every builder's constructor takes a pointer to a [`Graphics::Renderer`](#lug::Graphics::Renderer):
```cpp
lug::Graphics::Renderer* renderer = _graphics.getRenderer();
lug::Graphics::Builder::Camera cameraBuilder(*renderer);
```

When you have created your builder, you have to set all the parameters you need. In the case of [`Builder::Camera`](#lug::Graphics::Builder::Camera), you could set:

```cpp
cameraBuilder.setFovY(45.0f);
cameraBuilder.setZNear(0.1f);
cameraBuilder.setZFar(100.0f);
```

Once you have set all the parameters, you can call the build method of your builder. It will return a [`Resource::SharedPtr`](#lug::Graphics::Resource::SharedPtr):

```cpp
lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> camera = cameraBuilder.build();
// lug::Graphics::Resource::SharedPtr are like std::shared_ptr
// so you can check for nullptr value
if (!camera) {
    // ...
}
```

# Using lug::Core::Application
To simplify the use of Lugdunum, the engine provides an abstract class named [`Application`](#lug::Core::Application) that helps you get started quicker.

:::info
The use of this class is not mandatory but strongly recommended
:::

The first thing to do is to create your class that inherits from it:
```cpp
class Application : public ::lug::Core::Application {
    // ...
}
```


As [`Application`](#lug::Core::Application) is an abstract class, you have to override two methods as well as the destructor:
```cpp
void onEvent(const lug::Window::Event& event) override final;
void onFrame(const lug::System::Time& elapsedTime) override final;
~Application() override final;
```

Each time an event is triggered by the system, the method `onEvent()` is called. In this *callback* you can check the `event.type` which is referenced in the enum [`Window::Event::Type`](#lug::Window::Event)
 
```cpp
void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        // ...
        close() // Calling lug::Core::Application::close
    }
}
```

The second method to override is [`Application::onFrame()`](#lug::Core::Application::onFrame()). This method is called each loop, and you can do whatever pleases you in it, e.g., your application's logic. The `elapsedTime` variable contains the elapsed time since the last call to `onFrame()`.

```cpp
void Application::onFrame(const lug::System::Time& elapsedTime) {
    _rotation += (0.05f * elapsedTime.getMilliseconds<float>());

    float x = 20.0f * cos(lug::Math::Geometry::radians(_rotation));
    float y = 20.0f * sin(lug::Math::Geometry::radians(_rotation));

    if (_rotation > 360.0f) {
        _rotation -= 360.0f;
    }

    auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

    for (int i = 0; i < 2; ++i) {
        renderViews[i]->getCamera()->setPosition({x, -10.0f, y}, lug::Graphics::Node::TransformSpace::World);
        renderViews[i]->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }
}
```

The constructor of [`Application`](#lug::Core::Application) takes a structure `Info` defined as follow:
```cpp
struct Info {
    const char* name;
    Version version;
};
```

You can use the initializer list to make it easier:
```cpp
lug::Core::Application::Application{{"triangle", {0, 1, 0}}} {
    // ...
}
```

After the constructor phase, you have to call [`Application::init(argc, argv)`](#lug::Core::Application::init()). This method processes two main initialization steps.

First, it will initialize `lug::Graphics::Graphics _graphics;` with these default values: 
```cpp
lug::Graphics::Graphics::InitInfo _graphicsInitInfo{
    lug::Graphics::Renderer::Type::Vulkan,                  // type
    {                                                       // rendererInitInfo
        "shaders/",                                         // shaders root
        lug::Graphics::Render::Technique::Type::Forward,    // renderTechnique
        lug::Graphics::Renderer::DisplayMode::Full          // displayMode
    },
    {                                                       // mandatoryModules
        lug::Graphics::Module::Type::Core
    },
    {},                                                     // optionalModules
};
```

* [`Renderer::Type`](#lug::Graphics::Renderer::Type) is set to `Vulkan` by default, as it the only supported renderer at this moment.
* [`Renderer::Technique::Type`](#lug::Graphics::Renderer::Technique::Type) is set to `Forward` by default, as it the only supported render technique at this moment.
* [`Module::Type::Core`](#lug::Graphics::Module::Type::Core) defines basic default requirements for Vulkan (see [Modules](#modules)).

Finally, it will initialize `lug::Graphics::Render::Window* _window{nullptr};` as [`Application`](#lug::Core::Application) manages itself all the window creation. It uses these default values:
```cpp
lug::Graphics::Render::Window::InitInfo _renderWindowInitInfo{
    {                               // windowInitInfo
        800,                        // width
        600,                        // height
        "Lugdunum - Default title", // title
        lug::Window::Style::Default // style
    },

    {}                              // renderViewsInitInfo
};
```

You can also manually change these values:
```cpp
getRenderWindowInfo().windowInitInfo.title = "Foo Bar";
```

:::warning
You have to change these value before calling [`Application::init(argc, argv)`](#lug::Core::Application::init())
:::

## Scene
### Create a scene
[`Graphics::ResouceManager`](#lug::Graphics::ResouceManager) allows you to load `.gltf` scenes as [`Graphics::Scene`](#lug::Graphics::Scene). Just few lines is needed:
```cpp
lug::Graphics::Renderer* renderer = _graphics.getRenderer();
lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("my_model.gltf");
if (!sceneResource) {
    // Handle error
}
```

Then, you need to cast the [`Graphics::Resource`](#lug::Graphics::Resource) pointer into a [`Graphics::Scene`](#lug::Graphics::Scene) pointer before using it:
```cpp
_scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);
```

### Using nodes

Once you have loaded your scene, you can add new [`Graphics::Scene::Node`](#lug::Graphics::Scene::Node)s to it:
```cpp
lug::Graphics::Scene::Node* node = _scene->createSceneNode("my_node");
```
The node name passed to [`createSceneNode()`](#lug::Graphics::Scene::createSceneNode()) is used to retrieve the node:
```cpp
lug::Graphics::Scene::Node* node = _scene->getSceneNode("my_node");
```

[`Graphics::Scene::Node`](#lug::Graphics::Scene::Node) class is derived from [`Graphics::Node`](#lug::Graphics::Node) class which allows multiple operations such as translate, scale and rotate.

## Camera

### Create a camera

You can use [`Graphics::Builder::Camera`](#lug::Graphics::Builder::Camera) to create a camera.
All the parameters of the camera are the followings:

Setter                                         | Description        | Default value
-----------------------------------------------|--------------------|---------------
setName(const std::string& name)               | Name of the camera |
setType([`Graphics::Builder::Camera::Type`](#lug::Graphics::Builder::Camera::Type) type) | Type of camera | Type::Perspective
setZNear(float znear)                          | Near plane         | 0.0f
setZFar(float zfar)                            | Far plane          | 0.0f

Depending on the type of camera, you can set the following parameters:

Perspective camera setter            | Description    | Default value
-------------------------------------|----------------|----------------
setFovY(float fovy)                  | Field of view  | 0.0f
setAspectRatio(float aspectRatio)    | Aspect ratio   | 0.0f

Orthographic camera setter       | Description        | Default value
---------------------------------|--------------------|------------------------------------
setXMag(float xmag)              | Projection width   | 0.0f
setYMag(float ymag)              | Projection height  | 0.0f

```cpp
// Create the camera builder
lug::Graphics::Renderer* renderer = _graphics.getRenderer();
lug::Graphics::Builder::Camera cameraBuilder(*renderer);

// Set all parameters
cameraBuilder.setFovY(45.0f);
cameraBuilder.setZNear(0.1f);
cameraBuilder.setZFar(100.0f);

// Build the camera
lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> camera = cameraBuilder.build();
if (!camera) {
    // Handle error
}

```

### Attaching a camera

Once the camera is created, you have to attach the camera to a node of your scene and a [`Graphics::Render::View`](#lug::Graphics::Render::View).

```cpp
// Create camera node and attach camera to it
lug::Graphics::Scene::Node* node = _scene->createSceneNode("cameraNode");
node->attachCamera(camera);

// Add camera node to scene root
_scene->getRoot().attachChild(*node);

// Attach camera to the first RenderView
auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();
renderViews[0]->attachCamera(camera);
```
 
### Using [`Core::FreeMovement`](#lug::Core::FreeMovement)

[`FreeMovement`](#lug::Core::FreeMovement) is a class to easily add a FPS camera to your application.

Here is a basic integration of [`FreeMovement`](#lug::Core::FreeMovement):
```cpp
// Application.hpp
class Application : public ::lug::Core::Application {
    // ...
    lug::Core::FreeMovement _fpsCamera;
};
```
```cpp
// Application.cpp
bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }
    
    lug::Graphics::Scene::Node* node = _scene->createSceneNode("camera");
    // ...
    
    // Bind fps camera to camera node
    _fpsCamera.setTargetNode(*node);
    // Give window to fps camera to receive key/mouse events
    _fpsCamera.setEventSource(*_graphics.getRenderer()->getWindow());
    return true;
}

void Application::onFrame(const lug::System::Time& elapsedTime) {
    // Update fps camera
    _fpsCamera.onFrame(elapsedTime);
}

``` 
## Lights

Our 3D engine has four different types of light:

 Types of light | Description
--------------- | ----------------------------------------------------------------
Ambient         | Light giving a uniform color to all objects of the scene, no matter where they are
 Directional    | Light that is being emitted from a source that is infinitely far away. All shadows cast by this light are parallel, an ideal choice for simulating sunlight. 
 Point          | Emits light from a single point, as a real-life bulb. It emits in all directions.
 Spotlight      | Emits light in a cone shape, like a flashlight, or a stage light.

As for the camera, you can create a light using a builder, [`Graphics::Builder::Light`](#lug::Graphics::Builder::Light). Let us assume that you want to create a Directional light, here is a sample:

```cpp
// Create the light builder
lug::Graphics::Renderer* renderer = _graphics.getRenderer();
lug::Graphics::Builder::Light lightBuilder(*renderer);

// Set all parameters
lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
lightBuilder.setColor({2.0f, 2.0f, 2.0f, 1.0f});
lightBuilder.setDirection({-5.0f, -5.0f, -5.0f});

// Build the light
lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
if (!light) {
    // Handle error
}
```

Then, you just have to add the light to a scene node:
```cpp
lug::Graphics::Scene::Node* node = _scene->createSceneNode("lightNode");
node->attachLight(light);

// Don't forget to add the light node to the scene root
_scene->getRoot().attachChild(*node);
```

## Skybox
### Basic skyBox
You can attach a [`Graphics::SkyBox`](#lug::Graphics::SkyBox) to your scene.
First of all, you need to create a skyBox using a [`Graphics::Builder::SkyBox`](#lug::Graphics::Builder::SkyBox):
```cpp
// Create the skyBox builder
lug::Graphics::lug::Graphics::Renderer* renderer = _graphics.getRenderer();
Builder::SkyBox skyBoxBuilder(*renderer);

// Set all parameters
skyBoxBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);
skyBoxBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
skyBoxBuilder.setBackgroundFilename("my_skybox.jpg");

lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> skyBox = skyBoxBuilder.build();
if (!skyBox) {
    // Handle error
}
```

Then, you just have to attach the skyBox to your scene:
```cpp
_scene->setSkyBox(skyBox);
```

### Using skyBox with IBL (Image based lighting)
When using a skyBox, you can also apply IBL to objects of your scene. IBL is a rendering technique where the skyBox is seen as a set of lights. It gives a more realistic effect on your scene by adding a better lighting and a reflection on your objects. 

The first step to apply your skyBox's IBL on your scene is to add an environment .hdr file in your builder:
```cpp
skyBoxBuilder.setEnvironnementFilename("my_environment.hdr");
```

Then, once your skyBox is built, you have to create an irradiance map and a prefiltered map from it:
```cpp
lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> irradianceMap = skyBox->createIrradianceMap(*renderer);

if (!irradianceMap) {
    // Handle error
}

lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> prefilteredMap = skyBox->createPrefilteredMap(*renderer);

if (!prefilteredMap) {
    // Handle error
}
```

Finally, you have to set these maps on the object's materials of your scene you want, using [`Material::setIrradianceMap()`](#lug::Graphics::Render::Material::setIrradianceMap()) and [`Material::setPrefilteredMap()`](#lug::Graphics::Render::Material::setPrefilteredMap()). Here is a simple function that add IBL on all the objects of the scene:
```cpp
void applyIBL(const lug::Graphics::Scene::Node* node, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> irradianceMap, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> prefilteredMap) {
    const lug::Graphics::Scene::Node::MeshInstance* meshInstance = node->getMeshInstance();
    if (meshInstance) {
        for (auto& material: meshInstance->materials) {
            material->setIrradianceMap(irradianceMap);
            material->setPrefilteredMap(prefilteredMap);
        }
    }

    for (const auto& child : node->getChildren()) {
        applyIBL(static_cast<const lug::Graphics::Scene::Node*>(child), irradianceMap, prefilteredMap);
    }
}

applyIBL(&_scene->getRoot(), irradianceMap, prefilteredMap);
```

## Handling Time

With Lugdunum, the time is in microseconds, and it is stored in an `int64_t`.
The [`Time`](#lug::System::Time) class represents a period, the time that elapses between two events.

A Time value can be constructed from a microseconds source.
```cpp
lug::System::Time time(10000);
```

You can get the time in different formats.
```cpp
int64_t  timeInMicroseconds = time.getMicroseconds();
float    timeInMilliseconds = time.getMilliseconds();
float    timeInSeconds = time.getSeconds();
```

## GUI - Dear ImGui

Firstly, to use _Dear ImGui_, you need to add the following code listing in your `CMakeLists.txt` in order to find the include directories.

```cmake
# find imgui
if (NOT EXISTS "${LUG_THIRDPARTY_DIR}/imgui")
    message(FATAL_ERROR "Can't find imgui in the thirdparty directory")
endif()

set(IMGUI_INCLUDE_DIR ${LUG_THIRDPARTY_DIR}/imgui/include)
message(STATUS "Found Imgui: ${IMGUI_INCLUDE_DIR}")

include_directories(${IMGUI_INCLUDE_DIR})
```

To be able to use Dear ImGui in the usual manner you need to include "lug/Gui.hpp". To learn more about Dear ImGui, please visit the projects Github page https://github.com/ocornut/imgui