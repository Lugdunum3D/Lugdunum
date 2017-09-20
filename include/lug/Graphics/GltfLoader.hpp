#pragma once

#include <vector>

#include <gltf2/glTF2.hpp>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Loader.hpp>
#include <lug/Graphics/Render/Material.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Render/Texture.hpp>
#include <lug/Graphics/Scene/Node.hpp>

namespace lug {
namespace Graphics {

class Renderer;

/**
 * @brief      Class for loading glTF files
 */
class LUG_GRAPHICS_API GltfLoader final : public Loader {
private:
    struct LoadedAssets {
        std::vector<Resource::SharedPtr<Render::Texture>> textures;
        Resource::SharedPtr<Render::Material> defaultMaterial;
        std::vector<Resource::SharedPtr<Render::Material>> materials;
        std::vector<Resource::SharedPtr<Render::Mesh>> meshes;
    };

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

private:
    Resource::SharedPtr<Render::Texture> createTexture(Renderer& renderer, const gltf2::Asset& asset, GltfLoader::LoadedAssets& loadedAssets, int32_t index);
    Resource::SharedPtr<Render::Material> createMaterial(Renderer& renderer, const gltf2::Asset& asset, GltfLoader::LoadedAssets& loadedAssets, int32_t index);
    Resource::SharedPtr<Render::Material> createDefaultMaterial(Renderer& renderer, GltfLoader::LoadedAssets& loadedAssets);
    Resource::SharedPtr<Render::Mesh> createMesh(Renderer& renderer, const gltf2::Asset& asset, GltfLoader::LoadedAssets& loadedAssets, int32_t index);
    bool createNode(Renderer& renderer, const gltf2::Asset& asset, GltfLoader::LoadedAssets& loadedAssets, int32_t index, Scene::Node& parent);
};

} // Graphics
} // lug
