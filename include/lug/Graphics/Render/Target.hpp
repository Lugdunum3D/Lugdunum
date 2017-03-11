#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/View.hpp>

namespace lug {
namespace Graphics {
namespace Render {

class LUG_GRAPHICS_API Target {
public:
    Target() = default;

    Target(const Target&) = delete;
    Target(Target&&) = delete;

    Target& operator=(const Target&) = delete;
    Target& operator=(Target&&) = delete;

    virtual ~Target() = default;

    virtual View* createView(View::InitInfo& initInfo) = 0;

    virtual bool render() = 0;

    virtual uint16_t getWidth() const = 0;
    virtual uint16_t getHeight() const = 0;

    const std::vector<std::unique_ptr<View>>& getRenderViews() const;
    std::vector<std::unique_ptr<View>>& getRenderViews();

protected:
    std::vector<std::unique_ptr<View>> _renderViews;
};

#include <lug/Graphics/Render/Target.inl>

} // Render
} // Graphics
} // lug
