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

    /**
     * @brief      Render the camera's scene of all render views
     *
     * @return     Has render succeed
     */
    virtual bool render() = 0;

    /**
     * @brief      Gets the width.
     *
     * @return     The width.
     */
    virtual uint16_t getWidth() const = 0;
    /**
     * @brief      Gets the height.
     *
     * @return     The height.
     */
    virtual uint16_t getHeight() const = 0;

    /**
     * @brief      Gets the render views.
     *
     * @return     The render views.
     */
    const std::vector<std::unique_ptr<View>>& getRenderViews() const;
    /**
     * @brief      Get the render views.
     *
     * @return     The render views.
     */
    std::vector<std::unique_ptr<View>>& getRenderViews();

protected:
    std::vector<std::unique_ptr<View>> _renderViews;
};

#include <lug/Graphics/Render/Target.inl>

} // Render
} // Graphics
} // lug
