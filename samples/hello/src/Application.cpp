#include "Application.hpp"

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello Model";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        {                                                   // viewport
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },

            {                                               // extent
                1.0f,                                       // width
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

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    // Load scene
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("models/Box.gltf");
    if (!sceneResource) {
        return false;
    }

    _scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);

    // Attach camera
    {
        lug::Graphics::Builder::Camera cameraBuilder(*renderer);

        cameraBuilder.setFovY(45.0f);
        cameraBuilder.setZNear(0.1f);
        cameraBuilder.setZFar(100.0f);

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> camera = cameraBuilder.build();
        if (!camera) {
            LUG_LOG.error("Application::init Can't create camera");
            return false;
        }

        lug::Graphics::Scene::Node* node = _scene->createSceneNode("camera");
        _scene->getRoot().attachChild(*node);

        node->attachCamera(camera);

        // Attach camera to RenderView
        {
            auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

            LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

            renderViews[0]->attachCamera(camera);
        }
    }

    // Attach ambient light to the root node
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Ambient);
        lightBuilder.setColor({0.01f, 0.01f, 0.01f, 1.0f});

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application::init Can't create ambient light");
            return false;
        }

        _scene->getRoot().attachLight(light);
    }

    // Attach spot light to the camera
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Spot);
        lightBuilder.setColor({10.0f, 10.0f, 10.0f, 1.0f});
        lightBuilder.setDirection({0.0f, 0.0f, -1.0f});
        lightBuilder.setFalloffAngle(lug::Math::Geometry::radians(12.5f));
        lightBuilder.setFalloffExponent(80.0);

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application::init Can't create light");
            return false;
        }

        _scene->getSceneNode("camera")->attachLight(light);
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

    for (int i = 0; i < 1; ++i) {
        float x = 2.0f * cos(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));
        float y = 2.0f * sin(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));

        //_scene->getSceneNode("camera")->setPosition({x, 10.0f, y}, lug::Graphics::Node::TransformSpace::World);
        _scene->getSceneNode("camera")->setPosition({x, 1.0f, y}, lug::Graphics::Node::TransformSpace::World);
        renderViews[i]->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }
}
