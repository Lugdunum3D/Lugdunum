#include <lug/Window/Window.hpp>
#include <lug/Window/Keyboard.hpp>
#include <lug/System/Logger.hpp>

#if defined(LUG_SYSTEM_ANDROID)
#include <lug/System/Logger/LogCatHandler.hpp>
#else
#include <lug/System/Logger/OstreamHandler.hpp>
#endif

int main() {
    auto window = lug::Window::Window::create(800, 600, "Default Window", lug::Window::Style::Default);

    auto logger = lug::System::makeLogger("LugdunumSamples");

#if defined(LUG_SYSTEM_ANDROID)
    auto handler = lug::System::makeHandler<lug::System::LogCatHandler>("LogCat");
#else
    auto handler = lug::System::makeHandler<lug::System::StdoutHandler>("Stdout");
#endif

    logger->addHandler(handler);

    if (!window) {
        logger->fatal("Window was not created");
        return 1;
    }

    logger->info("Window was created successfully");

    // While window is open execute the following
    while (window->isOpen()) {
        lug::Window::Event event;
        while (window->pollEvent(event)) {
            logger->info("Event received: {}", static_cast<uint32_t>(event.type));

            if (event.type == lug::Window::EventType::CLOSE) {
                logger->info("Closing the app");
                window->close();
            }

            if (event.type == lug::Window::EventType::KEY_DOWN)
            {
                if (event.key.code == lug::Window::Keyboard::Key::A) {
                    logger->info("A is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::B) {
                    logger->info("B is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::C) {
                    logger->info("C is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::D) {
                    logger->info("D is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::E) {
                    logger->info("E is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F) {
                    logger->info("F is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::G) {
                    logger->info("G is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::H) {
                    logger->info("H is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::I) {
                    logger->info("I is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::J) {
                    logger->info("J is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::K) {
                    logger->info("K is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::L) {
                    logger->info("L is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::M) {
                    logger->info("M is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::N) {
                    logger->info("N is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::O) {
                    logger->info("O is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::P) {
                    logger->info("P is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Q) {
                    logger->info("Q is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::R) {
                    logger->info("R is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::S) {
                    logger->info("S is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::T) {
                    logger->info("T is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::U) {
                    logger->info("U is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::V) {
                    logger->info("V is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::W) {
                    logger->info("W is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::X) {
                    logger->info("X is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Y) {
                    logger->info("Y is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Z) {
                    logger->info("Z is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num0) {
                    logger->info("Num0 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num1) {
                    logger->info("Num1 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num2) {
                    logger->info("Num2 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num3) {
                    logger->info("Num3 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num4) {
                    logger->info("Num4 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num5) {
                    logger->info("Num5 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num6) {
                    logger->info("Num6 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num7) {
                    logger->info("Num7 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num8) {
                    logger->info("Num8 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num9) {
                    logger->info("Num9 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Escape) {
                    logger->info("Escape is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LControl) {
                    logger->info("LControl is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LShift) {
                    logger->info("LShift is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LAlt) {
                    logger->info("LAlt is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LSystem) {
                    logger->info("LSystem is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RControl) {
                    logger->info("RControl is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RShift) {
                    logger->info("RShift is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RAlt) {
                    logger->info("RAlt is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RSystem) {
                    logger->info("RSystem is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Menu) {
                    logger->info("Menu is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LBracket) {
                    logger->info("LBracket is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RBracket) {
                    logger->info("RBracket is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::SemiColon) {
                    logger->info("SemiColon is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Comma) {
                    logger->info("Comma is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Period) {
                    logger->info("Period is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Quote) {
                    logger->info("Quote is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Slash) {
                    logger->info("Slash is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::BackSlash) {
                    logger->info("BackSlash is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Tilde) {
                    logger->info("Tilde is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Equal) {
                    logger->info("Equal is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Dash) {
                    logger->info("Dash is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Space) {
                    logger->info("Space is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Return) {
                    logger->info("Return is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::BackSpace) {
                    logger->info("BackSpace is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Tab) {
                    logger->info("Tab is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::PageUp) {
                    logger->info("PageUp is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::PageDown) {
                    logger->info("PageDown is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::End) {
                    logger->info("End is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Home) {
                    logger->info("Home is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Insert) {
                    logger->info("Insert is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Delete) {
                    logger->info("Delete is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Add) {
                    logger->info("Add is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Subtract) {
                    logger->info("Subtract is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Multiply) {
                    logger->info("Multiply is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Divide) {
                    logger->info("Divide is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Left) {
                    logger->info("Left is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Right) {
                    logger->info("Right is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Up) {
                    logger->info("Up is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Down) {
                    logger->info("Down is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad0) {
                    logger->info("Numpad0 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad1) {
                    logger->info("Numpad1 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad2) {
                    logger->info("Numpad2 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad3) {
                    logger->info("Numpad3 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad4) {
                    logger->info("Numpad4 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad5) {
                    logger->info("Numpad5 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad6) {
                    logger->info("Numpad6 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad7) {
                    logger->info("Numpad7 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad8) {
                    logger->info("Numpad8 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad9) {
                    logger->info("Numpad9 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F1) {
                    logger->info("F1 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F2) {
                    logger->info("F2 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F3) {
                    logger->info("F3 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F4) {
                    logger->info("F4 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F5) {
                    logger->info("F5 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F6) {
                    logger->info("F6 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F7) {
                    logger->info("F7 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F8) {
                    logger->info("F8 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F9) {
                    logger->info("F9 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F10) {
                    logger->info("F10 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F11) {
                    logger->info("F11 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F12) {
                    logger->info("F12 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F13) {
                    logger->info("F13 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F14) {
                    logger->info("F14 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F15) {
                    logger->info("F15 is pressed");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Pause) {
                    logger->info("Pause is pressed");
                }
            }

            if (event.type == lug::Window::EventType::KEY_UP)
            {
                if (event.key.code == lug::Window::Keyboard::Key::A) {
                    logger->info("A is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::B) {
                    logger->info("B is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::C) {
                    logger->info("C is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::D) {
                    logger->info("D is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::E) {
                    logger->info("E is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F) {
                    logger->info("F is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::G) {
                    logger->info("G is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::H) {
                    logger->info("H is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::I) {
                    logger->info("I is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::J) {
                    logger->info("J is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::K) {
                    logger->info("K is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::L) {
                    logger->info("L is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::M) {
                    logger->info("M is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::N) {
                    logger->info("N is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::O) {
                    logger->info("O is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::P) {
                    logger->info("P is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Q) {
                    logger->info("Q is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::R) {
                    logger->info("R is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::S) {
                    logger->info("S is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::T) {
                    logger->info("T is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::U) {
                    logger->info("U is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::V) {
                    logger->info("V is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::W) {
                    logger->info("W is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::X) {
                    logger->info("X is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Y) {
                    logger->info("Y is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Z) {
                    logger->info("Z is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num0) {
                    logger->info("Num0 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num1) {
                    logger->info("Num1 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num2) {
                    logger->info("Num2 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num3) {
                    logger->info("Num3 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num4) {
                    logger->info("Num4 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num5) {
                    logger->info("Num5 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num6) {
                    logger->info("Num6 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num7) {
                    logger->info("Num7 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num8) {
                    logger->info("Num8 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Num9) {
                    logger->info("Num9 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Escape) {
                    logger->info("Escape is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LControl) {
                    logger->info("LControl is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LShift) {
                    logger->info("LShift is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LAlt) {
                    logger->info("LAlt is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LSystem) {
                    logger->info("LSystem is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RControl) {
                    logger->info("RControl is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RShift) {
                    logger->info("RShift is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RAlt) {
                    logger->info("RAlt is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RSystem) {
                    logger->info("RSystem is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Menu) {
                    logger->info("Menu is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::LBracket) {
                    logger->info("LBracket is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::RBracket) {
                    logger->info("RBracket is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::SemiColon) {
                    logger->info("SemiColon is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Comma) {
                    logger->info("Comma is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Period) {
                    logger->info("Period is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Quote) {
                    logger->info("Quote is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Slash) {
                    logger->info("Slash is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::BackSlash) {
                    logger->info("BackSlash is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Tilde) {
                    logger->info("Tilde is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Equal) {
                    logger->info("Equal is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Dash) {
                    logger->info("Dash is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Space) {
                    logger->info("Space is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Return) {
                    logger->info("Return is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::BackSpace) {
                    logger->info("BackSpace is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Tab) {
                    logger->info("Tab is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::PageUp) {
                    logger->info("PageUp is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::PageDown) {
                    logger->info("PageDown is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::End) {
                    logger->info("End is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Home) {
                    logger->info("Home is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Insert) {
                    logger->info("Insert is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Delete) {
                    logger->info("Delete is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Add) {
                    logger->info("Add is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Subtract) {
                    logger->info("Subtract is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Multiply) {
                    logger->info("Multiply is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Divide) {
                    logger->info("Divide is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Left) {
                    logger->info("Left is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Right) {
                    logger->info("Right is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Up) {
                    logger->info("Up is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Down) {
                    logger->info("Down is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad0) {
                    logger->info("Numpad0 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad1) {
                    logger->info("Numpad1 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad2) {
                    logger->info("Numpad2 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad3) {
                    logger->info("Numpad3 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad4) {
                    logger->info("Numpad4 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad5) {
                    logger->info("Numpad5 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad6) {
                    logger->info("Numpad6 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad7) {
                    logger->info("Numpad7 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad8) {
                    logger->info("Numpad8 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Numpad9) {
                    logger->info("Numpad9 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F1) {
                    logger->info("F1 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F2) {
                    logger->info("F2 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F3) {
                    logger->info("F3 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F4) {
                    logger->info("F4 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F5) {
                    logger->info("F5 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F6) {
                    logger->info("F6 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F7) {
                    logger->info("F7 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F8) {
                    logger->info("F8 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F9) {
                    logger->info("F9 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F10) {
                    logger->info("F10 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F11) {
                    logger->info("F11 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F12) {
                    logger->info("F12 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F13) {
                    logger->info("F13 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F14) {
                    logger->info("F14 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::F15) {
                    logger->info("F15 is released");
                }
                else if (event.key.code == lug::Window::Keyboard::Key::Pause) {
                    logger->info("Pause is released");
                }
            }

        }

    }

    return 0;
}
