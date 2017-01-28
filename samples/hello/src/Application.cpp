#include "Application.hpp"
#include <lug/Graphics/MeshInstance.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    getRenderWindowInfo().windowInitInfo.title = "Hello Cube";
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    _scene = _graphics.createScene();
    _cube = _graphics.createMesh("Cube");

    _cube->vertices = {
        {{-1.0f,-1.0f,-1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},

        // Back
        {{-1.0f,-1.0f,-1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f}},
        {{1.0f, 1.0f,-1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f}},
        {{1.0f,-1.0f,-1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f}},
        {{1.0f, 1.0f,-1.0f}, {1.0f, 0.0f, 1.0}, {0.0f, 0.0f}},

        // Left
        {{-1.0f,-1.0f,-1.0f}, {1.0f, 0.0f, 0.0}, {0.0f, 0.0f}},
        {{1.0f,-1.0f,-1.0f}, {1.0f, 0.0f, 0.0}, {0.0f, 0.0f}},
        {{1.0f,-1.0f, 1.0f}, {1.0f, 0.0f, 0.0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f,-1.0f}, {1.0f, 0.0f, 0.0}, {0.0f, 0.0f}},
        {{1.0f,-1.0f, 1.0f}, {1.0f, 0.0f, 0.0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f}, {1.0f, 0.0f, 0.0}, {0.0f, 0.0f}},

        // Right
        {{-1.0f, 1.0f,-1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{-1.0f, 1.0f,-1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f,-1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},

        // Top
        {{1.0f, 1.0f,-1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f,-1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f,-1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f,-1.0f,-1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f,-1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},

        // Bottom
        {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 0.0f}},
        {{-1.0f,-1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 0.0f}},
        {{1.0f,-1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0}, {0.0f, 0.0f}}
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

        cubeNode->attachMovableObject(std::move(cubeInstance));
        _scene->getRoot()->attachChild(std::move(cubeNode));
    }

    // Add camera to scene
    {
        _camera = _graphics.createCamera("camera");
        std::unique_ptr<lug::Graphics::MovableCamera> movableCamera = _scene->createMovableCamera("movable camera", _camera.get());
        std::unique_ptr<lug::Graphics::SceneNode> movableCameraNode = _scene->createSceneNode("movable camera node");

        movableCameraNode->attachMovableObject(std::move(movableCamera));
        _scene->getRoot()->attachChild(std::move(movableCameraNode));
        _camera->setScene(_scene.get());
    }

    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::EventType::CLOSE) {
        close();
    }
}

void Application::onFrame() {
    // Do something on each frame
}
