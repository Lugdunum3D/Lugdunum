#pragma once

#include <memory>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Loader.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

/**
 * @brief      Class for resource manager.
 *             The ResourceManager allows the user to load resources and store them.
 *             There should be at most one resource manager at any time, it is usually created
 *             by the Graphics instance, and retrievable by #Graphics::getResourceManager()
 */
class LUG_GRAPHICS_API ResourceManager {
public:
    /**
     * @brief      Constructs a ResourceManager, from a Renderer instance.
     * @param      renderer  The renderer.
     */
    ResourceManager(Renderer* renderer);

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;

    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    ~ResourceManager() = default;

    /**
     * @brief      Retrieve a resource from the ResourceManager.
     * @param[in]  handle  The handle of the resource.
     * @tparam     T       The type of the resource.
     * @return     A resource, as a custom SharedPtr<T>, which will allow us to do reference
     *             counting later.
     */
    template <typename T>
    Resource::SharedPtr<T> get(Resource::Handle handle);

    /**
     * @brief      Loads a resource from a file.
     *
     * @param[in]  filename  The filename of the file to load the resource from. The used Loader
     *             is determined by the extension of the file, so it must be present!
     *
     * @return     A pointer to the resulting resource.
     */
    Resource::SharedPtr<Resource> loadFile(const std::string& filename);

private:
    Renderer* _renderer{nullptr};
    std::vector<std::unique_ptr<Resource>> _resources;

    /**
     * The list of the available loaders. The string is the extension of the file, and the
     * pointer is the corresponding loader. The implementation will determine which loader to
     * call in loadFile thanks to this map.
     */
    std::unordered_map<std::string, std::unique_ptr<Loader>> _loaders;
};

#include <lug/Graphics/ResourceManager.inl>

} // Graphics
} // lug
