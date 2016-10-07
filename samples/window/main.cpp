#include <lug/Config.hpp>
#include <lug/Window/Window.hpp>

int main() {
    auto window = lug::Window::Window::create(800, 600, "Lug window", lug::Window::Style::Default);
    
    if (!window) {
        return 1;
    }
    
    while (window->isOpen()) { // While window is open execute the following
        lug::Window::Event event;
        while (window->pollEvent(event)) {
            // Handle events here
        }
    
        // ...
    }
    
    window->close();
    
    return 0;
}
