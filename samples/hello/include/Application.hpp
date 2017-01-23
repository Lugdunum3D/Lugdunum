#pragma once

#include <lug/Core/Application.hpp>

class Application : public lug::Core::Application {
public:
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application() = default;

    void onEvent(const lug::Window::Event& event) override final;
    void onFrame() override final;
};
