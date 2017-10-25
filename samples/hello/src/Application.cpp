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
#include <lug/Math/Geometry/Trigonometry.hpp>

#include <imgui.h>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello";

    // We can set the display mode, by default to full
    getGraphicsInfo().rendererInitInfo.displayMode = ::lug::Graphics::Renderer::DisplayMode::Full;
}

void applyIrradianceMap(const lug::Graphics::Scene::Node* node, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> irradianceMap) {
    const lug::Graphics::Scene::Node::MeshInstance* meshInstance = node->getMeshInstance();
    if (meshInstance) {
        for (auto& material: meshInstance->materials) {
            material->setIrradianceMap(irradianceMap);
        }
    }

    for (const auto& child : node->getChildren()) {
        applyIrradianceMap(static_cast<const lug::Graphics::Scene::Node*>(child), irradianceMap);
    }
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    // Load scene
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("models/DamagedHelmet/DamagedHelmet.gltf");
    if (!sceneResource) {
        return false;
    }

    _scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);

    // Adjust the model
    {
        lug::Graphics::Scene::Node* node = _scene->getSceneNode("UnityGlTF_correctionMatrix");
        node->rotate(lug::Math::Geometry::radians(270.0f), {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }

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

        lug::Graphics::Scene::Node* node = _scene->createSceneNode("camera");
        _scene->getRoot().attachChild(*node);

        node->attachCamera(camera);

        // Set initial position of the camera
        node->setPosition({5.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
        // Look at once
        node->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

        // Attach the camera node to the mover
        _cameraMover.setTargetNode(*node);
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
        skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::PositiveX, "textures/skybox2/posx.bmp");
        skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::NegativeX, "textures/skybox2/negx.bmp");
        skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::PositiveY, "textures/skybox2/posy.bmp");
        skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::NegativeY, "textures/skybox2/negy.bmp");
        skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::PositiveZ, "textures/skybox2/posz.bmp");
        skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::NegativeZ, "textures/skybox2/negz.bmp");

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> skyBox = skyBoxBuilder.build();
        if (!skyBox) {
            LUG_LOG.error("Application::init Can't create skyBox");
            return false;
        }

        _scene->setSkyBox(skyBox);

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> irradianceMap = skyBox->createIrradianceMap(*renderer);

        if (!irradianceMap) {
            LUG_LOG.error("Application::init Can't create irradiance map");
            return false;
        }

        applyIrradianceMap(&_scene->getRoot(), irradianceMap);
    }

    // Build imgui texture
    {
        lug::Graphics::Builder::Texture textureBuilder(*renderer);

        if (!textureBuilder.addLayer("textures/skybox/right.jpg")) {
            LUG_LOG.error("Application: Can't load texture");
            return false;
        }

        textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);
        textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
        textureBuilder.setMipMapFilter(lug::Graphics::Render::Texture::Filter::Linear);

        _texture = textureBuilder.build();
        if (!_texture) {
            LUG_LOG.error("Application::init Can't create texture");
            return false;
        }
    }

    return static_cast<lug::Graphics::Vulkan::Render::Window*>(_graphics.getRenderer()->getWindow())->initGui();
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
    auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(_texture);

    ImGui::Begin("Imgui texture", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Image(vkTexture.get(), ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
    ImGui::End();

    _cameraMover.onFrame(elapsedTime);
}
