#pragma once

#include <lug/Core/Application.hpp>
#include <lug/Graphics/Scene/Scene.hpp>

class Application : public ::lug::Core::Application {
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
    std::unique_ptr<lug::Graphics::Scene::Scene> _scene;

    // Temporary store mesh because we don't have resource manager yet
    std::unique_ptr<lug::Graphics::Render::Model> _model;

    float _rotation{0.0f};
};
