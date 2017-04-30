#include <lug/Graphics/GltfLoader.hpp>
#include <lug/Graphics/ResourceManager.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {

ResourceManager::ResourceManager(Renderer& renderer) : _renderer(renderer) {
    if (_renderer.getType() == Renderer::Type::Vulkan) {
        _loaders["gltf"] = std::make_unique<GltfLoader>();
        _loaders["glb"] = std::make_unique<GltfLoader>();
    }
}

Resource::SharedPtr<Resource> ResourceManager::loadFile(const std::string& filename) {
    std::string::size_type extensionPos = filename.find_last_of(".");
    if (extensionPos == std::string::npos) {
        LUG_LOG.error("ResourceManager: Can't find extension of the filename {}", filename);
        return nullptr;
    }

    std::string extension = filename.substr(extensionPos + 1);

    auto loader = _loaders.find(extension);
    if (loader == _loaders.end()) {
        LUG_LOG.error("ResourceManager: Can't find loader for extension {}", extension);
        return nullptr;
    }

    return loader->second->loadFile(filename);
}

} // Graphics
} // lug
