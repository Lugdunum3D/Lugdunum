#pragma once

#include <lug/Core/Application.hpp>
#include <lug/Core/FreeMovement.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Scene/Scene.hpp>

class Application : public ::lug::Core::Application {
public:
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application() override final = default;

    bool init(int argc, char* argv[]);
    bool initSphereMesh();

    void onEvent(const lug::Window::Event& event) override final;
    void onFrame(const lug::System::Time& elapsedTime) override final;

private:
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh> _sphereMesh;
    lug::Core::FreeMovement _mover;
};
