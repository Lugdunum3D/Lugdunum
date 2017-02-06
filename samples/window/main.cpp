#include <unordered_map>
#include <lug/Window/Window.hpp>
#include <lug/Window/Keyboard.hpp>
#include <lug/System/Logger.hpp>

#if defined(LUG_SYSTEM_ANDROID)
    #include <lug/System/Logger/LogCatHandler.hpp>
#else
    #include <lug/System/Logger/OstreamHandler.hpp>
#endif

#include <unordered_map>

auto createKeyEnumMap() {
    std::unordered_map<lug::Window::Keyboard::Key, std::string> returnValue;

    returnValue[lug::Window::Keyboard::Key::Unknown] = "Unknown";

    // Basic keys
    returnValue[lug::Window::Keyboard::Key::A] = "A";
    returnValue[lug::Window::Keyboard::Key::B] = "B";
    returnValue[lug::Window::Keyboard::Key::C] = "C";
    returnValue[lug::Window::Keyboard::Key::D] = "D";
    returnValue[lug::Window::Keyboard::Key::E] = "E";
    returnValue[lug::Window::Keyboard::Key::F] = "F";
    returnValue[lug::Window::Keyboard::Key::G] = "G";
    returnValue[lug::Window::Keyboard::Key::H] = "H";
    returnValue[lug::Window::Keyboard::Key::I] = "I";
    returnValue[lug::Window::Keyboard::Key::J] = "J";
    returnValue[lug::Window::Keyboard::Key::K] = "K";
    returnValue[lug::Window::Keyboard::Key::L] = "L";
    returnValue[lug::Window::Keyboard::Key::M] = "M";
    returnValue[lug::Window::Keyboard::Key::N] = "N";
    returnValue[lug::Window::Keyboard::Key::O] = "O";
    returnValue[lug::Window::Keyboard::Key::P] = "P";
    returnValue[lug::Window::Keyboard::Key::Q] = "Q";
    returnValue[lug::Window::Keyboard::Key::R] = "R";
    returnValue[lug::Window::Keyboard::Key::S] = "S";
    returnValue[lug::Window::Keyboard::Key::T] = "T";
    returnValue[lug::Window::Keyboard::Key::U] = "U";
    returnValue[lug::Window::Keyboard::Key::V] = "V";
    returnValue[lug::Window::Keyboard::Key::W] = "W";
    returnValue[lug::Window::Keyboard::Key::X] = "X";
    returnValue[lug::Window::Keyboard::Key::Y] = "Y";
    returnValue[lug::Window::Keyboard::Key::Z] = "Z";

    returnValue[lug::Window::Keyboard::Key::Num0] = "Num0";
    returnValue[lug::Window::Keyboard::Key::Num1] = "Num1";
    returnValue[lug::Window::Keyboard::Key::Num2] = "Num2";
    returnValue[lug::Window::Keyboard::Key::Num3] = "Num3";
    returnValue[lug::Window::Keyboard::Key::Num4] = "Num4";
    returnValue[lug::Window::Keyboard::Key::Num5] = "Num5";
    returnValue[lug::Window::Keyboard::Key::Num6] = "Num6";
    returnValue[lug::Window::Keyboard::Key::Num7] = "Num7";
    returnValue[lug::Window::Keyboard::Key::Num8] = "Num8";
    returnValue[lug::Window::Keyboard::Key::Num9] = "Num9";

    // Modifiers
    returnValue[lug::Window::Keyboard::Key::LControl] = "LControl";
    returnValue[lug::Window::Keyboard::Key::LShift] = "LShift";
    returnValue[lug::Window::Keyboard::Key::LAlt] = "LAlt";
    returnValue[lug::Window::Keyboard::Key::LSystem] = "LSystem";
    returnValue[lug::Window::Keyboard::Key::RControl] = "RControl";
    returnValue[lug::Window::Keyboard::Key::RShift] = "RShift";
    returnValue[lug::Window::Keyboard::Key::RAlt] = "RAlt";
    returnValue[lug::Window::Keyboard::Key::RSystem] = "RSystem";

    // Advanced keys
    returnValue[lug::Window::Keyboard::Key::Menu] = "Menu";
    returnValue[lug::Window::Keyboard::Key::LBracket] = "LBracket";
    returnValue[lug::Window::Keyboard::Key::RBracket] = "RBracket";
    returnValue[lug::Window::Keyboard::Key::SemiColon] = "SemiColon";
    returnValue[lug::Window::Keyboard::Key::Comma] = "Comma";
    returnValue[lug::Window::Keyboard::Key::Period] = "Period";
    returnValue[lug::Window::Keyboard::Key::Quote] = "Quote";
    returnValue[lug::Window::Keyboard::Key::Slash] = "Slash";
    returnValue[lug::Window::Keyboard::Key::BackSlash] = "BackSlash";
    returnValue[lug::Window::Keyboard::Key::Tilde] = "Tilde";
    returnValue[lug::Window::Keyboard::Key::Equal] = "Equal";
    returnValue[lug::Window::Keyboard::Key::Dash] = "Dash";
    returnValue[lug::Window::Keyboard::Key::Space] = "Space";
    returnValue[lug::Window::Keyboard::Key::Return] = "Return";
    returnValue[lug::Window::Keyboard::Key::BackSpace] = "BackSpace";
    returnValue[lug::Window::Keyboard::Key::Tab] = "Tab";
    returnValue[lug::Window::Keyboard::Key::PageUp] = "PageUp";
    returnValue[lug::Window::Keyboard::Key::PageDown] = "PageDown";
    returnValue[lug::Window::Keyboard::Key::End] = "End";
    returnValue[lug::Window::Keyboard::Key::Home] = "Home";
    returnValue[lug::Window::Keyboard::Key::Insert] = "Insert";
    returnValue[lug::Window::Keyboard::Key::Delete] = "Delete";
    returnValue[lug::Window::Keyboard::Key::Add] = "Add";
    returnValue[lug::Window::Keyboard::Key::Subtract] = "Subtract";
    returnValue[lug::Window::Keyboard::Key::Multiply] = "Multiply";
    returnValue[lug::Window::Keyboard::Key::Divide] = "Divide";
    returnValue[lug::Window::Keyboard::Key::Left] = "Left";
    returnValue[lug::Window::Keyboard::Key::Right] = "Right";
    returnValue[lug::Window::Keyboard::Key::Up] = "Up";
    returnValue[lug::Window::Keyboard::Key::Down] = "Down";
    returnValue[lug::Window::Keyboard::Key::Pause] = "Pause";
    returnValue[lug::Window::Keyboard::Key::CapsLock] = "CapsLock";
    returnValue[lug::Window::Keyboard::Key::Escape] = "Escape";

    // AZERTY Specifics
    returnValue[lug::Window::Keyboard::Key::Twosuperior] = "Twosuperior";
    returnValue[lug::Window::Keyboard::Key::Ampersand] = "Ampersand";
    returnValue[lug::Window::Keyboard::Key::Eacute] = "Eacute";
    returnValue[lug::Window::Keyboard::Key::QuoteDouble] = "QuoteDouble";
    returnValue[lug::Window::Keyboard::Key::LParen] = "LParen";
    returnValue[lug::Window::Keyboard::Key::Egrave] = "Egrave";
    returnValue[lug::Window::Keyboard::Key::Underscore] = "Underscore";
    returnValue[lug::Window::Keyboard::Key::Ccedilla] = "Ccedilla";
    returnValue[lug::Window::Keyboard::Key::Agrave] = "Agrave";
    returnValue[lug::Window::Keyboard::Key::RParen] = "RParen";
    returnValue[lug::Window::Keyboard::Key::DeadCircumflex] = "DeadCircumflex";
    returnValue[lug::Window::Keyboard::Key::Ugrave] = "Ugrave";
    returnValue[lug::Window::Keyboard::Key::Asterisk] = "Asterisk";
    returnValue[lug::Window::Keyboard::Key::Dollar] = "Dollar";
    returnValue[lug::Window::Keyboard::Key::Colon] = "Colon";
    returnValue[lug::Window::Keyboard::Key::Exclam] = "Exclam";
    returnValue[lug::Window::Keyboard::Key::Less] = "Less";
    returnValue[lug::Window::Keyboard::Key::Greater] = "Greater";

    // Numpad
    returnValue[lug::Window::Keyboard::Key::Numpad0] = "Numpad0";
    returnValue[lug::Window::Keyboard::Key::Numpad1] = "Numpad1";
    returnValue[lug::Window::Keyboard::Key::Numpad2] = "Numpad2";
    returnValue[lug::Window::Keyboard::Key::Numpad3] = "Numpad3";
    returnValue[lug::Window::Keyboard::Key::Numpad4] = "Numpad4";
    returnValue[lug::Window::Keyboard::Key::Numpad5] = "Numpad5";
    returnValue[lug::Window::Keyboard::Key::Numpad6] = "Numpad6";
    returnValue[lug::Window::Keyboard::Key::Numpad7] = "Numpad7";
    returnValue[lug::Window::Keyboard::Key::Numpad8] = "Numpad8";
    returnValue[lug::Window::Keyboard::Key::Numpad9] = "Numpad9";

    // Function keys
    returnValue[lug::Window::Keyboard::Key::F1] = "F1";
    returnValue[lug::Window::Keyboard::Key::F2] = "F2";
    returnValue[lug::Window::Keyboard::Key::F3] = "F3";
    returnValue[lug::Window::Keyboard::Key::F4] = "F4";
    returnValue[lug::Window::Keyboard::Key::F5] = "F5";
    returnValue[lug::Window::Keyboard::Key::F6] = "F6";
    returnValue[lug::Window::Keyboard::Key::F7] = "F7";
    returnValue[lug::Window::Keyboard::Key::F8] = "F8";
    returnValue[lug::Window::Keyboard::Key::F9] = "F9";
    returnValue[lug::Window::Keyboard::Key::F10] = "F10";
    returnValue[lug::Window::Keyboard::Key::F11] = "F11";
    returnValue[lug::Window::Keyboard::Key::F12] = "F12";
    returnValue[lug::Window::Keyboard::Key::F13] = "F13";
    returnValue[lug::Window::Keyboard::Key::F14] = "F14";
    returnValue[lug::Window::Keyboard::Key::F15] = "F15";

    return returnValue;
}

int main() {
    auto window = lug::Window::Window::create(800, 600, "Default Window", lug::Window::Style::Default);

    auto logger = lug::System::makeLogger("LugdunumSamples");

    auto keyEnumMap = createKeyEnumMap();

#if defined(LUG_SYSTEM_ANDROID)
    auto handler = lug::System::makeHandler<lug::System::LogCatHandler>("LogCat");
#else
    auto handler = lug::System::makeHandler<lug::System::StdoutHandler>("Stdout");
#endif

    logger->addHandler(handler);
    lug::System::Logger::logger.addHandler(handler);

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

            if (event.type == lug::Window::EventType::Close) {
                logger->info("Closing the app");
                window->close();
            }

            if (event.type == lug::Window::EventType::KeyPressed && event.key.code == lug::Window::Keyboard::Key::Escape) {
                logger->info("Closing the app (escape)");
                window->close();
            }

            if (event.type == lug::Window::EventType::KeyPressed
                && event.key.code == lug::Window::Keyboard::Key::Q && event.key.ctrl) {
                logger->info("Closing the app (ctrl+Q)");
                window->close();
            }

            // Checking KeyPressed events
            if (event.type == lug::Window::EventType::KeyPressed) {
                logger->info(keyEnumMap[event.key.code] + " is pressed");
            }

            // Checking KeyReleased events
            if (event.type == lug::Window::EventType::KeyReleased) {
                logger->info(keyEnumMap[event.key.code] + " is released");
            }

            if (event.type == lug::Window::EventType::CharEntered) {
                logger->info("Char event received: {}", static_cast<char>(event.character.val));
            }

            // Checking to see if any special keys are pressed
            if (event.type == lug::Window::EventType::KeyPressed ||
                event.type == lug::Window::EventType::KeyReleased) {
                if (event.key.ctrl) {
                    logger->info("MODIFIER CTRL");
                }
                if (event.key.alt) {
                    logger->info("MODIFIER ALT");
                }
                if (event.key.shift) {
                    logger->info("MODIFIER SHIFT");
                }
                if (event.key.system) {
                    logger->info("MODIFIER SYSTEM");
                }
            }

        }

//        // Checking that keys states are correctly set
//        for (auto it = keyEnumMap.begin(); it != keyEnumMap.end(); ++it) {
//            if (window->isKeyPressed(it->first)) {
//                logger->info(it->second + " is set to pressed");
//            }
//        }
    }

    return 0;
}
