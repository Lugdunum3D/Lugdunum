#pragma once

#include <string>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

/**
 * @brief      Class for loading a type of file
 */
class LUG_GRAPHICS_API Loader {
public:
    Loader() = default;

    Loader(const Loader&) = delete;
    Loader(Loader&&) = delete;

    Loader& operator=(const Loader&) = delete;
    Loader& operator=(Loader&&) = delete;

    virtual ~Loader() = default;

    /**
     * @brief      Loads a Resource from a file.
     * @param[in]  filename  The filename.
     * @return     The resource.
     */
    virtual Resource::SharedPtr<Resource> loadFile(const std::string& filename) = 0;
};

} // Graphics
} // lug
