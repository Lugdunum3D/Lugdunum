#include "Application.hpp"

#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Material.hpp>
#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Builder/Scene.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

Application::Application() : lug::Core::Application::Application{{"spheres_pbr", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Spheres PBR";
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    lug::Graphics::Renderer* renderer = _graphics.getRenderer();

    // Build the scene
    {
        lug::Graphics::Builder::Scene sceneBuilder(*renderer);
        sceneBuilder.setName("scene");

        _scene = sceneBuilder.build();
        if (!_scene) {
            LUG_LOG.error("Application: Can't create the scene");
            return false;
        }
    }

    // Build the sphere
    if (!initSphereMesh()) {
        return false;
    }

    // Attach the spheres
    {
        const int nbRows = 7;
        const int nbColumns = 7;
        const float spacing = 2.5;

        lug::Graphics::Builder::Material materialBuilder(*renderer);
        materialBuilder.setBaseColorFactor({1.0f, 0.0f, 0.0f, 1.0f});

        // Attach the spheres
        for (int row = 0; row < nbRows; ++row) {
            materialBuilder.setMetallicFactor((float)row / (float)nbRows);

            for (int col = 0; col < nbColumns; ++col) {
                lug::Graphics::Scene::Node* node = _scene->createSceneNode("sphere" + std::to_string(row * nbColumns + col));
                _scene->getRoot().attachChild(*node);

                if (col == 0) {
                    materialBuilder.setRoughnessFactor(0.05f);
                } else {
                    materialBuilder.setRoughnessFactor((float)col / (float)nbColumns);
                }

                node->attachMeshInstance(_sphereMesh, materialBuilder.build());

                node->setPosition({
                    (float)(col - (nbColumns / 2)) * spacing,
                    (float)(row - (nbRows / 2)) * spacing,
                    0.0f
                }, lug::Graphics::Node::TransformSpace::World);
            }
        }
    }

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

        _mover.setTargetNode(*node);
        _mover.setEventSource(*_graphics.getRenderer()->getWindow());

        node->attachCamera(camera);

        node->setPosition({0.0f, 0.0f, 25.0f}, lug::Graphics::Node::TransformSpace::World);
        camera->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

        // Attach camera to RenderView
        {
            auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

            LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

            renderViews[0]->attachCamera(camera);
        }
    }

    const lug::Math::Vec3f lightPositions[] = {
        lug::Math::Vec3f{-10.0f,  10.0f, 10.0f},
        lug::Math::Vec3f{ 10.0f,  10.0f, 10.0f},
        lug::Math::Vec3f{-10.0f, -10.0f, 10.0f},
        lug::Math::Vec3f{ 10.0f, -10.0f, 10.0f},
    };

    for (uint32_t i = 0; i < sizeof(lightPositions) / sizeof(lug::Math::Vec3f); ++i) {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Point);
        lightBuilder.setColor({300.0f, 300.0f, 300.0f, 1.0f});
        lightBuilder.setLinearAttenuation(0.0f);

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application::init Can't create light");
            return false;
        }

        lug::Graphics::Scene::Node* node = _scene->createSceneNode("light" + std::to_string(i));
        _scene->getRoot().attachChild(*node);

        node->setPosition(lightPositions[i]);
        node->attachLight(light);
    }

    return true;
}

bool Application::initSphereMesh() {
    std::vector<lug::Math::Vec3f> positions;
    std::vector<lug::Math::Vec3f> normals;
    std::vector<uint16_t> indices;

    // Generate positions / normals / indices
    {
        const int X_SEGMENTS = 64;
        const int Y_SEGMENTS = 64;
        for (int y = 0; y <= Y_SEGMENTS; ++y) {
            for (int x = 0; x <= X_SEGMENTS; ++x) {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * lug::Math::pi<float>()) * std::sin(ySegment * lug::Math::pi<float>());
                float yPos = std::cos(ySegment * lug::Math::pi<float>());
                float zPos = std::sin(xSegment * 2.0f * lug::Math::pi<float>()) * std::sin(ySegment * lug::Math::pi<float>());

                positions.push_back({xPos, yPos, zPos});
                normals.push_back({xPos, yPos, zPos});
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y) {
            if (!oddRow) { // even rows: y == 0, y == 2; and so on
                for (int x = 0; x <= X_SEGMENTS; ++x) {
                    indices.push_back(static_cast<uint16_t>((y + 1) * (X_SEGMENTS + 1) + x));
                    indices.push_back(static_cast<uint16_t>(y       * (X_SEGMENTS + 1) + x));
                }
            } else {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back(static_cast<uint16_t>(y       * (X_SEGMENTS + 1) + x));
                    indices.push_back(static_cast<uint16_t>((y + 1) * (X_SEGMENTS + 1) + x));
                }
            }

            oddRow = !oddRow;
        }
    }

    // Build the mesh
    {
        lug::Graphics::Builder::Mesh meshBuilder(*_graphics.getRenderer());
        meshBuilder.setName("sphere");

        lug::Graphics::Builder::Mesh::PrimitiveSet* primitiveSet = meshBuilder.addPrimitiveSet();

        primitiveSet->setMode(lug::Graphics::Render::Mesh::PrimitiveSet::Mode::TriangleStrip);

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

        _sphereMesh = meshBuilder.build();

        if (!_sphereMesh) {
            LUG_LOG.error("Application: Can't create the sphere mesh");
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

void Application::onFrame(const lug::System::Time& elapsedTime) {
    _mover.onFrame(elapsedTime);
}
