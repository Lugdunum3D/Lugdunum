#include "Application.hpp"
#include <lug/Graphics/MeshInstance.hpp>

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

    _cube->vertices = {
        // Front
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},

        // Back
        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 1.0}},
        {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 1.0}},
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0}},

        // Left
        {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0}},
        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0}},
        {{-1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0}},
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0}},

        // Right
        {{1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},

        // Top
        {{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

        // Bottom
        {{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 1.0}},
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 1.0}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}}
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

    // Add mesh to scene
    {
        std::unique_ptr<lug::Graphics::MeshInstance> cubeInstance = _scene->createMeshInstance("cube instance", _cube.get());
        std::unique_ptr<lug::Graphics::SceneNode> cubeNode = _scene->createSceneNode("cube instance node");
        _cubeNode = cubeNode.get();

        cubeNode->attachMovableObject(std::move(cubeInstance));
        _scene->getRoot()->attachChild(std::move(cubeNode));
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

    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::EventType::CLOSE) {
        close();
    }
}

void Application::onFrame() {
    _cubeNode->rotate(0.0005f, {0.0f, 1.0f, 0.0f});
}
