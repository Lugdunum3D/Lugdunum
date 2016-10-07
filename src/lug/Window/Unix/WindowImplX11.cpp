#include <lug/Window/Unix/WindowImplX11.hpp>

bool lug::Window::priv::WindowImpl::create(uint16_t width, uint16_t height, const std::string& title, Style style) {
    return false;
}

void lug::Window::priv::WindowImpl::close() {
}

void lug::Window::priv::WindowImpl::processEvents(std::queue<lug::Window::Event>& events) {
}
