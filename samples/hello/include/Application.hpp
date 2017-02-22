#pragma once

#include <lug/Core/Application.hpp>
#include <lug/Graphics/Scene.hpp>
#include <lug/Graphics/Camera.hpp>

class Application : public lug::Core::Application {
public:
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application() override final;

    bool init(int argc, char* argv[]);

    void onEvent(const lug::Window::Event& event) override final;
    void onFrame(const lug::System::Time& elapsedTime) override final;

private:
    std::unique_ptr<lug::Graphics::Scene> _scene;

    // Temporary store mesh because we don't have resource manager yet
    std::unique_ptr<lug::Graphics::Mesh> _cube;
    std::unique_ptr<lug::Graphics::Mesh> _plane;

    std::unique_ptr<lug::Graphics::Model> _model;

    float _rotation{0.0f};
};
