#include <iostream>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

int main() {
    lug::Graphics::Graphics graphics{{"hello", {0, 1, 0}}};

    // graphics.setRendererType(Renderer::Type::Vulkan);
    // graphics.addModule(...);

    // Add or remove modules
    // Set the renderer type

    graphics.init();

    return 0;
}
