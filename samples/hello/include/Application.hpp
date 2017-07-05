#pragma once

#include <lug/Core/Application.hpp>

class Application : public ::lug::Core::Application {
public:
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application() override final = default;

    bool init(int argc, char* argv[]);

    void onEvent(const lug::Window::Event& event) override final;
    void onFrame(const lug::System::Time& elapsedTime) override final;

private:
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene;
    float _rotation{0.0f};
};
