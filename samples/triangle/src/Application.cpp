#include "Application.hpp"

#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Material.hpp>
#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Builder/Scene.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

Application::Application() : lug::Core::Application::Application{{"triangle", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello Triangle";
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    lug::Graphics::Renderer* renderer = _graphics.getRenderer();

    // Build the scene
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> scene;
    {
        lug::Graphics::Builder::Scene sceneBuilder(*renderer);
        sceneBuilder.setName("scene");

        scene = sceneBuilder.build();
        if (!scene) {
            LUG_LOG.error("Application: Can't create the scene");
            return false;
        }
    }

    // Build the triangle
    if (!initTriangleMesh()) {
        return false;
    }

    // Attach the triangle
    {
        lug::Graphics::Builder::Material materialBuilder(*renderer);

        lug::Graphics::Scene::Node* node = scene->createSceneNode("triangle");
        scene->getRoot().attachChild(*node);

        node->attachMeshInstance(_triangleMesh, materialBuilder.build());
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

        lug::Graphics::Scene::Node* node = scene->createSceneNode("camera");
        scene->getRoot().attachChild(*node);

        node->attachCamera(camera);

        node->setPosition({0.0f, 0.0f, 5.0f}, lug::Graphics::Node::TransformSpace::World);
        camera->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

        // Attach camera to RenderView
        {
            auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

            LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

            renderViews[0]->attachCamera(camera);
        }
    }

    // Attach an ambient light
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Ambient);
        lightBuilder.setColor({1.0f, 1.0f, 1.0f, 1.0f});

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application: Can't create the light");
            return false;
        }

        scene->getRoot().attachLight(light);
    }

    return true;
}

bool Application::initTriangleMesh() {
    const std::vector<lug::Math::Vec3f> positions = {
        {-1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f},
        {-0.0f, 1.0f, 1.0f}
    };

    const std::vector<lug::Math::Vec3f> normals = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    };

    const std::vector<lug::Math::Vec4f> colors = {
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f}
    };

    const std::vector<uint16_t> indices = {0, 1, 2};

    // Build the mesh
    {
        lug::Graphics::Builder::Mesh meshBuilder(*_graphics.getRenderer());
        meshBuilder.setName("triangle");

        lug::Graphics::Builder::Mesh::PrimitiveSet* primitiveSet = meshBuilder.addPrimitiveSet();

        primitiveSet->setMode(lug::Graphics::Render::Mesh::PrimitiveSet::Mode::Triangles);

        primitiveSet->addAttributeBuffer(
            indices.data(),
            sizeof(uint16_t),
            static_cast<uint32_t>(indices.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice
        );

        primitiveSet->addAttributeBuffer(
            positions.data(),
            sizeof(lug::Math::Vec3f),
            static_cast<uint32_t>(positions.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Position
        );

        primitiveSet->addAttributeBuffer(
            normals.data(),
            sizeof(lug::Math::Vec3f),
            static_cast<uint32_t>(normals.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Normal
        );

        primitiveSet->addAttributeBuffer(
            colors.data(),
            sizeof(lug::Math::Vec4f),
            static_cast<uint32_t>(colors.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Color
        );

        _triangleMesh = meshBuilder.build();

        if (!_triangleMesh) {
            LUG_LOG.error("Application: Can't create the triangle mesh");
            return false;
        }
    }

    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        close();
    }
}

void Application::onFrame(const lug::System::Time&) {}
