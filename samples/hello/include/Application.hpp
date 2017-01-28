#pragma once

#include <lug/Core/Application.hpp>
#include <lug/Graphics/Scene.hpp>

class Application : public lug::Core::Application {
public:
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application() = default;

    bool init(int argc, char* argv[]) override final;

    void onEvent(const lug::Window::Event& event) override final;
    void onFrame() override final;

private:
    std::unique_ptr<lug::Graphics::Scene> _scene;

    // Temporary store mesh because it can't be attached to the scene yet
    std::unique_ptr<lug::Graphics::Mesh> _mesh;
};
