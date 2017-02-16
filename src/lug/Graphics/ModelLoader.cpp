#include <lug/Graphics/ModelLoader.hpp>
#include <lug/Graphics/ObjModelLoader.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {


// TODO: Move in helper class
std::string  getFileExtension(const std::string& fileName)
{
    std::size_t size = fileName.rfind('.', fileName.length());
    if (size != std::string::npos)
        return (fileName.substr(size + 1, fileName.length() - size));

    return ("");
}

bool ModelLoader::loadFromFile(Model* model, const std::string& file) {
    std::string fileExtension = getFileExtension(file);

    if (fileExtension == "obj") {
        return ObjModelLoader::loadFromFile(model, file);
    }

    LUG_LOG.error("ModelLoader::loadFromFile error loading lodel \"{}\": unknow file format", file);
    return false;
}

} // Graphics
} // lug
