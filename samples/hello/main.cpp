#include <iostream>
#include <lug/Graphic/Vulkan/Loader.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>

int main() {
    lug::Graphic::Vulkan::Loader::loadCore();

    std::cout << (void*)vkCreateInstance << std::endl;
    return 0;
}
