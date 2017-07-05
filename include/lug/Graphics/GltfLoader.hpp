#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Loader.hpp>

namespace lug {
namespace Graphics {

class Renderer;

/**
 * @brief      Class for loading glTF files
 */
class LUG_GRAPHICS_API GltfLoader final : public Loader {
public:
    GltfLoader(Renderer& renderer);

    GltfLoader(const GltfLoader&) = delete;
    GltfLoader(GltfLoader&&) = delete;

    GltfLoader& operator=(const GltfLoader&) = delete;
    GltfLoader& operator=(GltfLoader&&) = delete;

    ~GltfLoader() = default;

    /**
     * @brief      Loads a glTF ressource from a file
     * @param[in]  filename  The filename
     * @return     SharedPtr to the resulting Resource
     */
    Resource::SharedPtr<Resource> loadFile(const std::string& filename) override final;
};

} // Graphics
} // lug
