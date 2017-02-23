#include <ctime>
#include <sstream>
#include "Application.hpp"
#include <lug/Graphics/DirectionalLight.hpp>
#include <lug/Graphics/Spotlight.hpp>
#include <lug/Graphics/PointLight.hpp>
#include <lug/Graphics/MeshInstance.hpp>
#include <lug/Graphics/ModelInstance.hpp>
#include <lug/Graphics/Light.hpp>

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include <string>
#include <lug/System/Logger.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    std::srand((uint32_t)std::time(0));

    lug::System::Logger::logger.info("In constructor of Application::Application()");

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

    getGraphicsInfo().rendererInitInfo.useDiscreteGPU = false;
}

Application::~Application() {
    // TODO: Remove this when the ResourceManager is done
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    for (auto& queue: vkRender->getQueues()) {
        queue.waitIdle();
    }
}

float randomFloat(float from, float to) {
    return from + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(to-from)));
}

lug::Math::Vec3f randColor() {
    float r = randomFloat(0.0f, 1.0f);
    float g = randomFloat(0.0f, 1.0f);
    float b = randomFloat(0.0f, 1.0f);
    return lug::Math::Vec3f({r, g, b});
}

lug::Math::Vec3f randPos() {
    float x = randomFloat(-10.0f, 10.0f);
    float y = randomFloat(-1.0f, -10.0f);
    float z = randomFloat(-10.0f, 10.0f);
    return lug::Math::Vec3f({x, y, z});
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    _scene = _graphics.createScene();
    _cube = _graphics.createMesh("Cube");
    _plane = _graphics.createMesh("Plane");

    // Low Poly by Olexandr Zymohliad
    // is licensed under CC Attribution
    // https://skfb.ly/IXwu
    _model = _graphics.createModel("model", "models/LowPoly/Low poly.obj");
    if (!_model) {
        return false;
    }



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

/*    float cubeNbX = 5;
    float cubeNbY = 5;
    float centerX = cubeNbX * 3.0f / 2.0f;
    float centerY = cubeNbY * 3.0f / 2.0f;
    for (uint32_t x = 0; x < cubeNbX; ++x) {
        for (uint32_t y = 0; y < cubeNbY; ++y) {
            // Add cube2 to scene
            {
                std::unique_ptr<lug::Graphics::MeshInstance> cubeInstance2 = _scene->createMeshInstance("cube instance 2", _cube.get());
                std::unique_ptr<lug::Graphics::SceneNode> cubeNode2 = _scene->createSceneNode("cube instance node 2");
                cubeNode2->translate({x * 3.0f - centerX, 0.0f, y * 3.0f - centerY});

                cubeNode2->attachMovableObject(std::move(cubeInstance2));
                _scene->getRoot()->attachChild(std::move(cubeNode2));
            }
        }
    }

    // Add cube to scene
    {
        std::unique_ptr<lug::Graphics::MeshInstance> cubeInstance = _scene->createMeshInstance("cube instance", _cube.get());
        std::unique_ptr<lug::Graphics::SceneNode> cubeNode = _scene->createSceneNode("cube instance node");
        _cubeNode = cubeNode.get();

        cubeNode->attachMovableObject(std::move(cubeInstance));
        _scene->getRoot()->attachChild(std::move(cubeNode));
    }*/


    // Add plane to scene
/*    {
        std::unique_ptr<lug::Graphics::MeshInstance> planeInstance = _scene->createMeshInstance("plane instance", _plane.get());
        std::unique_ptr<lug::Graphics::SceneNode> planeNode = _scene->createSceneNode("plane instance node");

        planeNode->scale({10.0f, 1.0f, 10.0f});

        planeNode->attachMovableObject(std::move(planeInstance));
        _scene->getRoot()->attachChild(std::move(planeNode));
    }
*/
    // Add model to scene
    {
        std::unique_ptr<lug::Graphics::ModelInstance> modelInstance = _scene->createModelInstance("model instance", _model.get());
        std::unique_ptr<lug::Graphics::SceneNode> modelNode = _scene->createSceneNode("model instance node");

        modelNode->rotate(lug::Math::Geometry::radians(180.0f), {1.0f, 0.0f, 0.0f});

        modelNode->attachMovableObject(std::move(modelInstance));
        _scene->getRoot()->attachChild(std::move(modelNode));

    }

    // Add directional light to scene
   {
        std::unique_ptr<lug::Graphics::Light> light = _scene->createLight("light", lug::Graphics::Light::Type::DirectionalLight);
        std::unique_ptr<lug::Graphics::SceneNode> lightNode = _scene->createSceneNode("light node");

        light->setDiffuse({1.0f, 1.0f, 1.0f});
        static_cast<lug::Graphics::DirectionalLight*>(light.get())->setDirection({0.0f, 4.0f, 5.0f});

        lightNode->attachMovableObject(std::move(light));
        _scene->getRoot()->attachChild(std::move(lightNode));
    }
/*
    for (uint32_t i = 0; i < 30; ++i) {
        // Add point light to scene
        {
            std::stringstream ss;
            ss << "light" << i;
            std::unique_ptr<lug::Graphics::Light> light = _scene->createLight(ss.str(), lug::Graphics::Light::Type::PointLight);
            std::unique_ptr<lug::Graphics::SceneNode> lightNode = _scene->createSceneNode("light node");

            light->setDiffuse(randColor());
            light->setAmbient({0.0f, 0.0f, 0.0f});
            static_cast<lug::Graphics::PointLight*>(light.get())->setPosition(randPos());
            static_cast<lug::Graphics::PointLight*>(light.get())->setLinear(0.22f);
            static_cast<lug::Graphics::PointLight*>(light.get())->setQuadric(0.20f);

            lightNode->attachMovableObject(std::move(light));
            _scene->getRoot()->attachChild(std::move(lightNode));
        }
    }*/

    // Add spotlight to scene
/*    {
        std::unique_ptr<lug::Graphics::Light> light = _scene->createLight("light", lug::Graphics::Light::Type::Spotlight);
        std::unique_ptr<lug::Graphics::SceneNode> lightNode = _scene->createSceneNode("light node");

        light->setDiffuse({0.9f, 0.9f, 0.9f});
        static_cast<lug::Graphics::Spotlight*>(light.get())->setDirection({0.0f, 4.0f, 5.0f});
        static_cast<lug::Graphics::Spotlight*>(light.get())->setPosition({0.0f, -4.0f, -5.0f});
        static_cast<lug::Graphics::Spotlight*>(light.get())->setAngle(9.0f);
        static_cast<lug::Graphics::Spotlight*>(light.get())->setOuterAngle(11.0f);

        lightNode->attachMovableObject(std::move(light));
        _scene->getRoot()->attachChild(std::move(lightNode));
    }*/

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
//        renderViews[1]->attachCamera(std::move(camera2));
    }

/*    _cubeNode->scale({0.5f, 0.5f, 0.5f});
    _cubeNode->translate({0.0f, -2.0f, -centerY});*/

    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::EventType::CLOSE) {
        close();
    }
}

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
