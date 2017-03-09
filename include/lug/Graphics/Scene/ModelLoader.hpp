#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Model.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

class LUG_GRAPHICS_API ModelLoader {
public:
    ModelLoader() = default;

    ModelLoader(const ModelLoader&) = delete;
    ModelLoader(ModelLoader&& ModelLoader) = delete;

    ModelLoader& operator=(const ModelLoader&) = delete;
    ModelLoader& operator=(ModelLoader&& ModelLoader) = delete;

    ~ModelLoader() = default;

    static bool loadFromFile(Render::Model* model, const std::string& file);
};

} // Scene
} // Graphics
} // lug
