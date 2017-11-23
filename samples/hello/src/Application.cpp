#include "Application.hpp"

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/SkyBox.hpp>
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/Render/SkyBox.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include <imgui.h>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello";
    getRenderWindowInfo().windowInitInfo.width = 1900;
    getRenderWindowInfo().windowInitInfo.height = 1000;

    // We can set the display mode, by default to full
    getGraphicsInfo().rendererInitInfo.displayMode = ::lug::Graphics::Renderer::DisplayMode::Full;

    // Set the antialiasing to MSAA 2x
    getGraphicsInfo().rendererInitInfo.antialiasing = ::lug::Graphics::Renderer::Antialiasing::MSAA8X;
}

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

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    // Load scene
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("models/centurion/centurion.gltf");
    if (!sceneResource) {
        return false;
    }

    _scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);

    // Adjust the model
  /*  {
        lug::Graphics::Scene::Node* node = _scene->getSceneNode("UnityGlTF_correctionMatrix");
        node->rotate(lug::Math::Geometry::radians(270.0f), {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }
*/
    // Attach camera
    {
        lug::Graphics::Builder::Camera cameraBuilder(*renderer);

        cameraBuilder.setFovY(45.0f);
        cameraBuilder.setZNear(0.1f);
        cameraBuilder.setZFar(100.0f);

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> camera = cameraBuilder.build();
        if (!camera) {
            LUG_LOG.error("Application: Can't create the camera");
            return false;
        }

        _cameraNode = _scene->createSceneNode("camera");
        _scene->getRoot().attachChild(*_cameraNode);

        _cameraNode->attachCamera(camera);

        // Set initial position of the camera
        _cameraNode->setPosition({5.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
        // Look at once
        _cameraNode->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

        // Attach the camera node to the mover
        _cameraMover.setTargetNode(*_cameraNode);
        _cameraMover.setEventSource(*_graphics.getRenderer()->getWindow());

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
            LUG_LOG.error("Application: Can't create the ambient light");
            return false;
        }

        _scene->getRoot().attachLight(light);
    }

    // Attach directional light
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
        lightBuilder.setColor({2.0f, 2.0f, 2.0f, 1.0f});
        lightBuilder.setDirection({-5.0f, -5.0f, -5.0f});

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application: Can't create the directional light");
            return false;
        }

        lug::Graphics::Scene::Node* node = _scene->createSceneNode("light");
        _scene->getRoot().attachChild(*node);

        node->attachLight(light);
    }

    // Attach skyBox
    {
        lug::Graphics::Builder::SkyBox skyBoxBuilder(*renderer);

        skyBoxBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);
        skyBoxBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);

        skyBoxBuilder.setBackgroundFilename("textures/Road_to_MonumentValley/Background.jpg");
        skyBoxBuilder.setEnvironnementFilename("textures/Road_to_MonumentValley/Environnement.hdr");

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> skyBox = skyBoxBuilder.build();
        if (!skyBox) {
            LUG_LOG.error("Application::init Can't create skyBox");
            return false;
        }

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> irradianceMap = skyBox->createIrradianceMap(*renderer);

        if (!irradianceMap) {
            LUG_LOG.error("Application::init Can't create irradiance map");
            return false;
        }

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> prefilteredMap = skyBox->createPrefilteredMap(*renderer);

        if (!prefilteredMap) {
            LUG_LOG.error("Application::init Can't create prefiltered map");
            return false;
        }

        _scene->setSkyBox(skyBox);
        applyIBL(&_scene->getRoot(), irradianceMap, prefilteredMap);
    }

    return true;
    //return static_cast<lug::Graphics::Vulkan::Render::Window*>(_graphics.getRenderer()->getWindow())->initGui();
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        close();
    }

    if (event.type == lug::Window::Event::Type::KeyPressed) {
        const uint32_t currentDisplayMode = static_cast<uint32_t>(_graphics.getRenderer()->getDisplayMode());

        if (event.key.code == lug::Window::Keyboard::Key::F1) {
            _graphics.getRenderer()->setDisplayMode(static_cast<::lug::Graphics::Renderer::DisplayMode>(currentDisplayMode == 0 ? 7 : currentDisplayMode - 1));
        } else if (event.key.code == lug::Window::Keyboard::Key::F2) {
            _graphics.getRenderer()->setDisplayMode(static_cast<::lug::Graphics::Renderer::DisplayMode>((currentDisplayMode + 1) % 8));
        }
    }
}

void Application::onFrame(const lug::System::Time& elapsedTime) {
    static float rotation = 0.0f;

    rotation += elapsedTime.getSeconds<float>() * 20.0f;
    if (rotation >= 360.0f) {
        rotation = 0.0f;
    }

    // Rotate camera
    {
        float angle = lug::Math::Geometry::radians(rotation);

        float x = 3.0f * cos(angle);
        float y = 3.0f * sin(angle);

        _cameraNode->setPosition({x, 0, y}, lug::Graphics::Node::TransformSpace::World);
        _cameraNode->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }
/*    auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(lug::Graphics::Vulkan::Render::SkyBox::getBrdfLut());

    ImGui::Begin("BRDF Lut", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Image(vkTexture.get(), ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
    ImGui::End();

    _cameraMover.onFrame(elapsedTime);*/
}
