#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Model.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API ModelLoader {
public:
    ModelLoader() = default;

    ModelLoader(const ModelLoader&) = delete;
    ModelLoader(ModelLoader&& ModelLoader) = delete;

    ModelLoader& operator=(const ModelLoader&) = delete;
    ModelLoader& operator=(ModelLoader&& ModelLoader) = delete;

    virtual ~ModelLoader() = default;

    static bool loadFromFile(Model* model, const std::string& file);
};

} // Graphics
} // lug
