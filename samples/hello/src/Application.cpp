#include "Application.hpp"

#include <lug/Graphics/Light/Directional.hpp>
#include <lug/Graphics/Scene/ModelInstance.hpp>

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello Model";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::Render::Technique::Type::Forward,    // renderTechniqueType
        {                                                   // viewport
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },

            {                                               // extent
                0.5f,                                       // width
                1.0f                                        // height
            },

            0.0f,                                           // minDepth
            1.0f                                            // maxDepth
        },
        {                                                   // scissor
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },
            {                                               // extent
                1.0f,                                       // width
                1.0f                                        // height
            }
        },
        nullptr                                             // camera
    });

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::Render::Technique::Type::Forward,    // renderTechniqueType
        {                                                   // viewport
            {                                               // offset
                0.5f,                                       // x
                0.0f                                        // y
            },

            {                                               // extent
                0.5f,                                       // width
                1.0f                                        // height
            },

            0.0f,                                           // minDepth
            1.0f                                            // maxDepth
        },
        {                                                   // scissor
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },
            {                                               // extent
                1.0f,                                       // width
                1.0f                                        // height
            }
        },
        nullptr                                             // camera
    });
}

Application::~Application() {
    // TODO: Remove this when the ResourceManager is done
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    vkRender->getDevice().waitIdle();
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    // Load the model
    {
        // Low Poly by Olexandr Zymohliad
        // is licensed under CC Attribution
        // https://skfb.ly/IXwu
        _model = _graphics.createModel("model", "models/LowPoly/Low poly.obj");

        if (!_model) {
            return false;
        }
    }

    // Create the scene
    _scene = _graphics.createScene();

    // Add model to scene
    {
        std::unique_ptr<lug::Graphics::Scene::ModelInstance> modelInstance = _scene->createModelInstance("model instance", _model.get());
        _scene->getRoot()->createSceneNode("model instance node", std::move(modelInstance));
    }

    // Add directional light to scene
    {
        std::unique_ptr<lug::Graphics::Light::Light> light = _scene->createLight("light", lug::Graphics::Light::Light::Type::Directional);

        // Set the diffuse color to white and the position
        light->setDiffuse({1.0f, 1.0f, 1.0f});
        static_cast<lug::Graphics::Light::Directional*>(light.get())->setDirection({0.0f, -4.0f, 5.0f});

        _scene->getRoot()->createSceneNode("light node", std::move(light));
    }

    // Create two cameras
    std::unique_ptr<lug::Graphics::Render::Camera> camera = _graphics.createCamera("camera");
    camera->setScene(_scene.get());

    std::unique_ptr<lug::Graphics::Render::Camera> camera2 = _graphics.createCamera("camera2");
    camera2->setScene(_scene.get());

    // Add camera to scene
    {
        std::unique_ptr<lug::Graphics::Scene::MovableCamera> movableCamera = _scene->createMovableCamera("movable camera", camera.get());
        std::unique_ptr<lug::Graphics::Scene::MovableCamera> movableCamera2 = _scene->createMovableCamera("movable camera2", camera2.get());

        std::unique_ptr<lug::Graphics::Scene::Node> movableCameraNode = _scene->createSceneNode("movable camera node");

        movableCameraNode->attachMovableObject(std::move(movableCamera));
        movableCameraNode->attachMovableObject(std::move(movableCamera2));

        _scene->getRoot()->attachChild(std::move(movableCameraNode));
    }

    // Attach cameras to RenderView
    {
        auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

        LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

        renderViews[0]->attachCamera(std::move(camera));
        renderViews[1]->attachCamera(std::move(camera2));
    }

    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        close();
    }
}

void Application::onFrame(const lug::System::Time& elapsedTime) {
    _rotation += (0.05f * elapsedTime.getMilliseconds<float>());

    if (_rotation > 360.0f) {
        _rotation -= 360.0f;
    }

    auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

    for (int i = 0; i < 2; ++i) {
        float x = 30.0f * cos(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));
        float y = 30.0f * sin(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));

        renderViews[i]->getCamera()->setPosition({x, 10.0f, y}, lug::Graphics::Node::TransformSpace::World);
        renderViews[i]->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }
}
