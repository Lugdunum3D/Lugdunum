#pragma once

#include <string>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/ModelLoader.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API ObjModelLoader final : public lug::Graphics::ModelLoader {
public:
    ObjModelLoader() = default;

    ObjModelLoader(const ObjModelLoader&) = delete;
    ObjModelLoader(ObjModelLoader&& ObjModelLoader) = delete;

    ObjModelLoader& operator=(const ObjModelLoader&) = delete;
    ObjModelLoader& operator=(ObjModelLoader&& ObjModelLoader) = delete;

    ~ObjModelLoader() override final = default;

    static bool loadFromFile(Model* model, const std::string& file);
};

} // Graphics
} // lug
