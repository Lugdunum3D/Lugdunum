#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderView.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API RenderTarget {
public:
    RenderTarget() = default;

    RenderTarget(const RenderTarget&) = delete;
    RenderTarget(RenderTarget&&) = delete;

    RenderTarget& operator=(const RenderTarget&) = delete;
    RenderTarget& operator=(RenderTarget&&) = delete;

    virtual ~RenderTarget() = default;

    virtual RenderView* createView(RenderView::InitInfo& initInfo) = 0;

    virtual void render() = 0;

    virtual uint16_t getWidth() const = 0;
    virtual uint16_t getHeight() const = 0;
};

} // Graphics
} // lug
