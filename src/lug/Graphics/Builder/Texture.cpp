#include <lug/Graphics/Builder/Texture.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(push)
    #pragma warning(disable : 4244)
    #pragma warning(disable : 4456)

#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(pop)
#endif

#if defined(LUG_SYSTEM_ANDROID)
    #include <android/asset_manager.h>

    #include <lug/Window/Android/WindowImplAndroid.hpp>
    #include <lug/Window/Window.hpp>
#endif

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Texture::Texture(Renderer& renderer) : _renderer(renderer) {}

Texture::~Texture() {
    for (auto& layer : _layers) {
        delete[] layer.data;
        layer.data = nullptr;
    }
}

Resource::SharedPtr<Render::Texture> Texture::build() {
    Resource::SharedPtr<Render::Texture> texture{nullptr};

    switch (_renderer.getType()) {
        case Renderer::Type::Vulkan:
            texture = lug::Graphics::Vulkan::Builder::Texture::build(*this);
    }

    if (texture) {
        texture->_width = _width;
        texture->_height = _height;

        texture->_magFilter = _magFilter;
        texture->_minFilter = _minFilter;
        texture->_mipMapFilter = _mipMapFilter;
        texture->_wrapS = _wrapS;
        texture->_wrapT = _wrapT;
    }

    return texture;
}

bool Texture::addLayer(uint32_t width, uint32_t height, const unsigned char* data) {
    if (!_width && !_height) {
        _width = width;
        _height = height;
    } else if (width != _width || height != _height) {
        return false;
    }

    if (!data) {
        _layers.push_back({data});
        return true;
    }

    unsigned char* newData = new unsigned char[width * height * 4];

    if (!newData) {
        return false;

    }
    std::memcpy(newData, data, width * height * 4 * sizeof(unsigned char));

    _layers.push_back({newData});

    return true;
}


bool Texture::addLayer(const std::string& filename) {
    int texWidth{0};
    int texHeight{0};
    int texChannels{0};

#if defined(LUG_SYSTEM_ANDROID)
    // Load shader from compressed asset
    AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, filename.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        LUG_LOG.error("Vulkan::Texture::build: Can't open Android asset \"{}\"", filename);
        freePixels(layersPixels);
        return nullptr;
    }

    uint32_t size = AAsset_getLength(asset);

    if (size <= 0) {
        LUG_LOG.error("Vulkan::Texture::build: Android asset \"{}\" is empty", filename);
        freePixels(layersPixels);
        return nullptr;
    }

    unsigned char* data = new unsigned char[size];

    AAsset_read(asset, reinterpret_cast<char*>(data), size);
    AAsset_close(asset);

    stbi_uc* pixels = stbi_load_from_memory(data, size, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    delete[] data;
#else
    stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
#endif

    if (!pixels) {
        return false;
    }

    if (!addLayer(texWidth, texHeight, pixels)) {
        stbi_image_free(pixels);
        return false;
    }

    stbi_image_free(pixels);
    return true;
}

} // Builder
} // Graphics
} // lug
