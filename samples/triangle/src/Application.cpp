#include "Application.hpp"

#include <lug/Graphics/Light/Directional.hpp>
#include <lug/Graphics/Scene/MeshInstance.hpp>

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

Application::Application() : lug::Core::Application::Application{{"triangle", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello Triangle";
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

    _scene = _graphics.createScene();
    _triangle = _graphics.createMesh("Triangle");

    // 1. pos
    // 2. color
    // 3. normal
    // 4. texture uv
    _triangle->vertices = {
        {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    };

    _triangle->indices = {
        2, 1, 0,
    };

    if (!_triangle->load()) {
        return false;
    }

    // Create the scene
    _scene = _graphics.createScene();

    // Add triangle to scene
    {
        std::unique_ptr<lug::Graphics::Scene::MeshInstance> triangleInstance = _scene->createMeshInstance("triangle instance", _triangle.get());
        _scene->getRoot()->createSceneNode("triangle instance node", std::move(triangleInstance));
    }

    // Add directional light to scene
    {
        std::unique_ptr<lug::Graphics::Light::Light> light = _scene->createLight("light", lug::Graphics::Light::Light::Type::Directional);

        // Set the diffuse to white color and the direction to the viewing direction of the camera
        light->setDiffuse({1.0f, 1.0f, 1.0f});
        static_cast<lug::Graphics::Light::Directional*>(light.get())->setDirection({0.0f, 0.0f, -1.0f});

        _scene->getRoot()->createSceneNode("light node", std::move(light));
    }

    // Create a camera
    std::unique_ptr<lug::Graphics::Render::Camera> camera = _graphics.createCamera("camera");
    camera->setScene(_scene.get());

    // Add camera to scene
    {
        std::unique_ptr<lug::Graphics::Scene::MovableCamera> movableCamera = _scene->createMovableCamera("movable camera", camera.get());
        _scene->getRoot()->createSceneNode("movable camera node", std::move(movableCamera));
    }

    // Attach camera to RenderView
    {
        auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();
        renderViews[0]->attachCamera(std::move(camera));
    }

    // Set the position of the camera
    {
        auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();
        renderViews[0]->getCamera()->setPosition({0.0f, 0.0f, 5.0f}, lug::Graphics::Node::TransformSpace::World);
    }

    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        close();
    }
}

void Application::onFrame(const lug::System::Time&) {}
