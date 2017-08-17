#pragma once

#include <cstdint>

#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {
namespace Render {

class LUG_GRAPHICS_API DirtyObject {
public:
    DirtyObject() = default;

    DirtyObject(const DirtyObject&) = delete;
    DirtyObject(DirtyObject&&) = delete;

    DirtyObject& operator=(const DirtyObject&) = delete;
    DirtyObject& operator=(DirtyObject&&) = delete;

    virtual ~DirtyObject() = default;

    void setDirty();
    void setDirty(uint32_t currentFrame);

    void clearDirty();
    void clearDirty(uint32_t currentFrame);

    bool isDirty(uint32_t currentFrame) const;
    bool isDirty() const;

private:
    uint32_t _dirtyFlag{0xFFFFFFFF};
};

#include <lug/Graphics/Render/DirtyObject.inl>

} // Render
} // Graphics
} // lug
