#pragma once

namespace lug {
namespace Window {
namespace priv {

#include <cstdint>

constexpr uint8_t MWMHintsFunctions    = 1 << 0;
constexpr uint8_t MWMHintsDecorations  = 1 << 1;

constexpr uint8_t MWMDecorBorder       = 1 << 1;
constexpr uint8_t MWMDecorResizeh      = 1 << 2;
constexpr uint8_t MWMDecorTitle        = 1 << 3;
constexpr uint8_t MWMDecorMenu         = 1 << 4;
constexpr uint8_t MWMDecorMinimize     = 1 << 5;
constexpr uint8_t MWMDecorMaximize     = 1 << 6;

constexpr uint8_t MWMFuncResize        = 1 << 1;
constexpr uint8_t MWMFuncMove          = 1 << 2;
constexpr uint8_t MWMFuncMinimize      = 1 << 3;
constexpr uint8_t MWMFuncMaximize      = 1 << 4;

constexpr uint8_t MWMFuncClose         = 1 << 5;

struct WMHints {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long          inputMode;
    unsigned long state;
};

} // namespace priv
} // namespace Window
} // namespace lug
