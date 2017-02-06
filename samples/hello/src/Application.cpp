#include "Application.hpp"
#include <lug/Graphics/DirectionalLight.hpp>
#include <lug/Graphics/Spotlight.hpp>
#include <lug/Graphics/PointLight.hpp>
#include <lug/Graphics/MeshInstance.hpp>
#include <lug/Graphics/Light.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello Cube";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::RenderTechnique::Type::Forward,  // renderTechniqueType
        {                                               // viewport
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },

            {                                           // extent
                0.5f,                                   // width
                1.0f                                    // height
            },

            0.0f,                                       // minDepth
            1.0f                                        // maxDepth
        },
        {                                               // scissor
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },
            {                                           // extent
                1.0f,                                   // width
                1.0f                                    // height
            }
        },
        nullptr                                         // camera
    });

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::RenderTechnique::Type::Forward,  // renderTechniqueType
        {                                               // viewport
            {                                           // offset
                0.5f,                                   // x
                0.0f                                    // y
            },

            {                                           // extent
                0.5f,                                   // width
                1.0f                                    // height
            },

            0.0f,                                       // minDepth
            1.0f                                        // maxDepth
        },
        {                                               // scissor
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },
            {                                           // extent
                1.0f,                                   // width
                1.0f                                    // height
            }
        },
        nullptr                                         // camera
    });
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    _scene = _graphics.createScene();
    _cube = _graphics.createMesh("Cube");
    _plane = _graphics.createMesh("Plane");



    //1. pos
    //2. color
    //3. normal
    //4. texture uv
    _cube->vertices = {
        // Front
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},

        // Back
        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

        // Left
        {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // Right
        {{1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

        // Top
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},

        // Bottom
        {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}
    };

    _cube->indices = {
        // Front
        2, 1, 0,
        2, 3, 1,

        // Back
        4, 5, 6,
        6, 5, 7,

        // Left
        8, 9, 10,
        10, 9, 11,

        // Right
        13, 12, 14,
        13, 14, 15,

        // Top
        16, 19, 17,
        16, 18, 19,

        // Bottom
        20, 21, 23,
        20, 23, 22
    };

    if (!_cube->load()) {
        return false;
    }

    //1. pos
    //2. color
    //3. normal
    //4. texture uv
    _plane->vertices = {
        {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}
    };

    _plane->indices = {
        3, 1, 0,
        2, 3, 0
    };

    if (!_plane->load()) {
        return false;
    }

    // Add cube2 to scene
    {
        std::unique_ptr<lug::Graphics::MeshInstance> cubeInstance2 = _scene->createMeshInstance("cube instance 2", _cube.get());
        std::unique_ptr<lug::Graphics::SceneNode> cubeNode2 = _scene->createSceneNode("cube instance node 2");
        _cubeNode2 = cubeNode2.get();

        cubeNode2->attachMovableObject(std::move(cubeInstance2));
        _scene->getRoot()->attachChild(std::move(cubeNode2));
    }

    // Add cube to scene
    {
        std::unique_ptr<lug::Graphics::MeshInstance> cubeInstance = _scene->createMeshInstance("cube instance", _cube.get());
        std::unique_ptr<lug::Graphics::SceneNode> cubeNode = _scene->createSceneNode("cube instance node");
        _cubeNode = cubeNode.get();

        cubeNode->attachMovableObject(std::move(cubeInstance));
        _scene->getRoot()->attachChild(std::move(cubeNode));
    }


    // Add plane to scene
    {
        std::unique_ptr<lug::Graphics::MeshInstance> planeInstance = _scene->createMeshInstance("plane instance", _plane.get());
        std::unique_ptr<lug::Graphics::SceneNode> planeNode = _scene->createSceneNode("plane instance node");

        planeNode->scale({10.0f, 1.0f, 10.0f});

        planeNode->attachMovableObject(std::move(planeInstance));
        _scene->getRoot()->attachChild(std::move(planeNode));
    }

    // Add directional light to scene
/*   {
        std::unique_ptr<lug::Graphics::Light> light = _scene->createLight("light", lug::Graphics::Light::Type::DIRECTIONAL_LIGHT);
        std::unique_ptr<lug::Graphics::SceneNode> lightNode = _scene->createSceneNode("light node");

        light->setDiffuse({0.2f, 0.2f, 0.2f});
        static_cast<lug::Graphics::DirectionalLight*>(light.get())->setDirection({0.0f, 4.0f, 5.0f});

        lightNode->attachMovableObject(std::move(light));
        _scene->getRoot()->attachChild(std::move(lightNode));
    }*/

    // Add point light to scene
    {
        std::unique_ptr<lug::Graphics::Light> light = _scene->createLight("light", lug::Graphics::Light::Type::POINT_LIGHT);
        std::unique_ptr<lug::Graphics::SceneNode> lightNode = _scene->createSceneNode("light node");

        light->setDiffuse({0.0f, 0.2f, 1.0f});
        static_cast<lug::Graphics::PointLight*>(light.get())->setPosition({1.5f, -2.0f, 1.5f});
        static_cast<lug::Graphics::PointLight*>(light.get())->setLinear(0.14f);
        static_cast<lug::Graphics::PointLight*>(light.get())->setQuadric(0.07f);

        lightNode->attachMovableObject(std::move(light));
        _scene->getRoot()->attachChild(std::move(lightNode));
    }

    // Add spotlight to scene
    {
        std::unique_ptr<lug::Graphics::Light> light = _scene->createLight("light", lug::Graphics::Light::Type::SPOTLIGHT);
        std::unique_ptr<lug::Graphics::SceneNode> lightNode = _scene->createSceneNode("light node");

        light->setDiffuse({0.9f, 0.9f, 0.9f});
        static_cast<lug::Graphics::Spotlight*>(light.get())->setDirection({0.0f, 4.0f, 5.0f});
        static_cast<lug::Graphics::Spotlight*>(light.get())->setPosition({0.0f, -4.0f, -5.0f});
        static_cast<lug::Graphics::Spotlight*>(light.get())->setAngle(9.0f);
        static_cast<lug::Graphics::Spotlight*>(light.get())->setOuterAngle(11.0f);

        lightNode->attachMovableObject(std::move(light));
        _scene->getRoot()->attachChild(std::move(lightNode));
    }

    std::unique_ptr<lug::Graphics::Camera> camera = _graphics.createCamera("camera");
    std::unique_ptr<lug::Graphics::Camera> camera2 = _graphics.createCamera("camera2");
    // Add camera to scene
    {
        std::unique_ptr<lug::Graphics::MovableCamera> movableCamera = _scene->createMovableCamera("movable camera", camera.get());
        std::unique_ptr<lug::Graphics::MovableCamera> movableCamera2 = _scene->createMovableCamera("movable camera2", camera2.get());
        std::unique_ptr<lug::Graphics::SceneNode> movableCameraNode = _scene->createSceneNode("movable camera node");

        movableCameraNode->attachMovableObject(std::move(movableCamera));
        movableCameraNode->attachMovableObject(std::move(movableCamera2));
        _scene->getRoot()->attachChild(std::move(movableCameraNode));
        camera->setScene(_scene.get());
        camera2->setScene(_scene.get());
    }

    // Attach cameras to RenderView
    {
        auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

        LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

        renderViews[0]->attachCamera(std::move(camera));
        renderViews[1]->attachCamera(std::move(camera2));
    }

    _cubeNode2->translate({0.0f, 0.0f, 2.0f});
    _cubeNode->scale({0.5f, 0.5f, 0.5f});

    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::EventType::CLOSE) {
        close();
    }
}

void Application::onFrame(float elapsedTime) {
    _cubeNode->rotate(0.005f * elapsedTime, {0.0f, 1.0f, 0.0f});
}
