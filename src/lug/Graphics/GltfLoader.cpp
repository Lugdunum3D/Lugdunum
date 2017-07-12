#include <lug/Graphics/GltfLoader.hpp>

#include <gltf2/glTF2.hpp>
#include <gltf2/Exceptions.hpp>

#include <lug/System/Logger/Logger.hpp>
#include <lug/Graphics/Builder/Scene.hpp>
#include <lug/Graphics/Builder/Material.hpp>
#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/Graphics/Scene/Scene.hpp>

namespace lug {
namespace Graphics {

GltfLoader::GltfLoader(Renderer& renderer): Loader(renderer) {}

static void* getBufferViewData(const gltf2::Asset& asset, const gltf2::Accessor& accessor) {
    const gltf2::BufferView& bufferView = asset.bufferViews[accessor.bufferView];
    const gltf2::Buffer& buffer = asset.buffers[bufferView.buffer];

    if (!buffer.data) {
        LUG_LOG.error("GltfLoader::createMesh Buffer data can't be null");
        return nullptr;
    }

    // TODO(nokitoo): handle uri
    return buffer.data + bufferView.byteOffset + accessor.byteOffset;
}

static uint32_t getAttributeSize(const gltf2::Accessor& accessor) {
    uint32_t componentSize = 0;
    switch (accessor.componentType) {
        case gltf2::Accessor::ComponentType::Byte:
            componentSize = sizeof(char);
            break;
        case gltf2::Accessor::ComponentType::UnsignedByte:
            componentSize = sizeof(unsigned char);
            break;
        case gltf2::Accessor::ComponentType::Short:
            componentSize = sizeof(short);
            break;
        case gltf2::Accessor::ComponentType::UnsignedShort:
            componentSize = sizeof(unsigned short);
            break;
        case gltf2::Accessor::ComponentType::UnsignedInt:
            componentSize = sizeof(unsigned int);
            break;
        case gltf2::Accessor::ComponentType::Float:
            componentSize = sizeof(float);
            break;
    }

    // Mutliply the componentSize according to the type
    switch (accessor.type) {
        case gltf2::Accessor::Type::Scalar:
            return componentSize;
        case gltf2::Accessor::Type::Vec2:
            return componentSize * 2;
        case gltf2::Accessor::Type::Vec3:
            return componentSize * 3;
        case gltf2::Accessor::Type::Vec4:
            return componentSize * 4;
        case gltf2::Accessor::Type::Mat2:
            return componentSize * 4;
        case gltf2::Accessor::Type::Mat3:
            return componentSize * 9;
        case gltf2::Accessor::Type::Mat4:
            return componentSize * 16;
    }

    return componentSize;
}

static Resource::SharedPtr<Render::Texture> createTexture(Renderer& renderer, const gltf2::Asset& asset, const gltf2::Texture& gltfTexture) {
    // TODO: Check if the texture is already created
    Builder::Texture textureBuilder(renderer);

    if (gltfTexture.source != -1) {
        // TODO: Handle correctly the load with bufferView / uri data
        textureBuilder.addLayer(asset.images[gltfTexture.source].uri);
    }

    if (gltfTexture.sampler != -1) {
        const gltf2::Sampler& sampler = asset.samplers[gltfTexture.sampler];

        switch(sampler.magFilter) {
            case gltf2::Sampler::MagFilter::None:
                break;
            case gltf2::Sampler::MagFilter::Nearest:
                textureBuilder.setMagFilter(Render::Texture::Filter::Nearest);
                break;
            case gltf2::Sampler::MagFilter::Linear:
                textureBuilder.setMagFilter(Render::Texture::Filter::Linear);
                break;
        }

        switch(sampler.minFilter) {
            case gltf2::Sampler::MinFilter::None:
                break;
            case gltf2::Sampler::MinFilter::Nearest:
                textureBuilder.setMinFilter(Render::Texture::Filter::Nearest);
                break;
            case gltf2::Sampler::MinFilter::Linear:
                textureBuilder.setMinFilter(Render::Texture::Filter::Linear);
                break;
            case gltf2::Sampler::MinFilter::NearestMipMapNearest:
                textureBuilder.setMinFilter(Render::Texture::Filter::Nearest);
                textureBuilder.setMipMapFilter(Render::Texture::Filter::Nearest);
                break;
            case gltf2::Sampler::MinFilter::LinearMipMapNearest:
                textureBuilder.setMinFilter(Render::Texture::Filter::Linear);
                textureBuilder.setMipMapFilter(Render::Texture::Filter::Nearest);
                break;
            case gltf2::Sampler::MinFilter::NearestMipMapLinear:
                textureBuilder.setMinFilter(Render::Texture::Filter::Nearest);
                textureBuilder.setMipMapFilter(Render::Texture::Filter::Linear);
                break;
            case gltf2::Sampler::MinFilter::LinearMipMapLinear:
                textureBuilder.setMinFilter(Render::Texture::Filter::Linear);
                textureBuilder.setMipMapFilter(Render::Texture::Filter::Linear);
                break;
        }

        switch(sampler.wrapS) {
            case gltf2::Sampler::WrappingMode::ClampToEdge:
                textureBuilder.setWrapS(Render::Texture::WrappingMode::ClampToEdge);
                break;
            case gltf2::Sampler::WrappingMode::MirroredRepeat:
                textureBuilder.setWrapS(Render::Texture::WrappingMode::MirroredRepeat);
                break;
            case gltf2::Sampler::WrappingMode::Repeat:
                textureBuilder.setWrapS(Render::Texture::WrappingMode::Repeat);
                break;
        }

        switch(sampler.wrapT) {
            case gltf2::Sampler::WrappingMode::ClampToEdge:
                textureBuilder.setWrapT(Render::Texture::WrappingMode::ClampToEdge);
                break;
            case gltf2::Sampler::WrappingMode::MirroredRepeat:
                textureBuilder.setWrapT(Render::Texture::WrappingMode::MirroredRepeat);
                break;
            case gltf2::Sampler::WrappingMode::Repeat:
                textureBuilder.setWrapT(Render::Texture::WrappingMode::Repeat);
                break;
        }
    }

    return textureBuilder.build();
}

static Resource::SharedPtr<Render::Material> createMaterial(Renderer& renderer, const gltf2::Asset& asset, const gltf2::Material& gltfMaterial) {
    // TODO: Check if the material is already created
    Builder::Material materialBuilder(renderer);

    materialBuilder.setName(gltfMaterial.name);

    materialBuilder.setBaseColorFactor({
        gltfMaterial.pbr.baseColorFactor[0],
        gltfMaterial.pbr.baseColorFactor[1],
        gltfMaterial.pbr.baseColorFactor[2],
        gltfMaterial.pbr.baseColorFactor[3]
    });

    if (gltfMaterial.pbr.baseColorTexture.index != -1) {
        Resource::SharedPtr<Render::Texture> texture = createTexture(renderer, asset, asset.textures[gltfMaterial.pbr.baseColorTexture.index]);
        if (!texture) {
            LUG_LOG.error("GltfLoader::createMaterial Can't create the texture resource");
            return nullptr;
        }

        materialBuilder.setBaseColorTexture(texture, gltfMaterial.pbr.baseColorTexture.texCoord);
    }

    materialBuilder.setMetallicFactor(gltfMaterial.pbr.metallicFactor);
    materialBuilder.setRoughnessFactor(gltfMaterial.pbr.roughnessFactor);

    if (gltfMaterial.pbr.metallicRoughnessTexture.index != -1) {
        Resource::SharedPtr<Render::Texture> texture = createTexture(renderer, asset, asset.textures[gltfMaterial.pbr.metallicRoughnessTexture.index]);
        if (!texture) {
            LUG_LOG.error("GltfLoader::createMaterial Can't create the texture resource");
            return nullptr;
        }

        materialBuilder.setMetallicRoughnessTexture(texture, gltfMaterial.pbr.metallicRoughnessTexture.texCoord);
    }

    if (gltfMaterial.normalTexture.index != -1) {
        Resource::SharedPtr<Render::Texture> texture = createTexture(renderer, asset, asset.textures[gltfMaterial.normalTexture.index]);
        if (!texture) {
            LUG_LOG.error("GltfLoader::createMaterial Can't create the texture resource");
            return nullptr;
        }

        materialBuilder.setNormalTexture(texture, gltfMaterial.normalTexture.texCoord);
    }

    if (gltfMaterial.occlusionTexture.index != -1) {
        Resource::SharedPtr<Render::Texture> texture = createTexture(renderer, asset, asset.textures[gltfMaterial.occlusionTexture.index]);
        if (!texture) {
            LUG_LOG.error("GltfLoader::createMaterial Can't create the texture resource");
            return nullptr;
        }

        materialBuilder.setOcclusionTexture(texture, gltfMaterial.occlusionTexture.texCoord);
    }

    if (gltfMaterial.emissiveTexture.index != -1) {
        Resource::SharedPtr<Render::Texture> texture = createTexture(renderer, asset, asset.textures[gltfMaterial.emissiveTexture.index]);
        if (!texture) {
            LUG_LOG.error("GltfLoader::createMaterial Can't create the texture resource");
            return nullptr;
        }

        materialBuilder.setEmissiveTexture(texture, gltfMaterial.emissiveTexture.texCoord);
    }

    materialBuilder.setEmissiveFactor({
        gltfMaterial.emissiveFactor[0],
        gltfMaterial.emissiveFactor[1],
        gltfMaterial.emissiveFactor[2]
    });

    return materialBuilder.build();
}

static Resource::SharedPtr<Render::Material> createDefaultMaterial(Renderer& renderer) {
    // TODO: Check if the material is already created
    Builder::Material materialBuilder(renderer);
    return materialBuilder.build();
}

static void* generateNormals(float* positions, uint32_t accessorCount) {
    Math::Vec3f* data = new Math::Vec3f[accessorCount];

    uint32_t trianglesCount = accessorCount / 3;
    uint32_t positionsIdx = 0;
    for (uint32_t i = 0; i < trianglesCount; ++i) {
        Math::Vec3f a{positions[positionsIdx], positions[positionsIdx + 1], positions[positionsIdx + 2]};
        Math::Vec3f b{positions[positionsIdx + 3], positions[positionsIdx + 4], positions[positionsIdx + 5]};
        Math::Vec3f c{positions[positionsIdx + 6], positions[positionsIdx + 7], positions[positionsIdx + 8]};
        Math::Vec3f edge1 = b - a;
        Math::Vec3f edge2 = c - a;

        data[i++] = cross(edge1, edge2);
    }

    return data;
}

static Resource::SharedPtr<Render::Mesh> createMesh(Renderer& renderer, const gltf2::Asset& asset, const gltf2::Mesh& gltfMesh) {
    // TODO: Check if the mesh is already created
    Builder::Mesh meshBuilder(renderer);
    meshBuilder.setName(gltfMesh.name);

    for (const gltf2::Primitive& gltfPrimitive : gltfMesh.primitives) {
        Builder::Mesh::PrimitiveSet* primitiveSet = meshBuilder.addPrimitiveSet();

        // Mode
        switch (gltfPrimitive.mode) {
            case gltf2::Primitive::Mode::Points:
                primitiveSet->setMode(Render::Mesh::PrimitiveSet::Mode::Points);
                break;
            case gltf2::Primitive::Mode::Lines:
                primitiveSet->setMode(Render::Mesh::PrimitiveSet::Mode::Lines);
                break;
            case gltf2::Primitive::Mode::LineLoop:
                LUG_LOG.error("GltfLoader::createMesh Unsupported mode LineLoop");
                return nullptr;
            case gltf2::Primitive::Mode::LineStrip:
                primitiveSet->setMode(Render::Mesh::PrimitiveSet::Mode::LineStrip);
                break;
            case gltf2::Primitive::Mode::Triangles:
                primitiveSet->setMode(Render::Mesh::PrimitiveSet::Mode::Triangles);
                break;
            case gltf2::Primitive::Mode::TriangleStrip:
                primitiveSet->setMode(Render::Mesh::PrimitiveSet::Mode::TriangleStrip);
                break;
            case gltf2::Primitive::Mode::TriangleFan:
                primitiveSet->setMode(Render::Mesh::PrimitiveSet::Mode::TriangleFan);
                break;
        }

        // Indices
        if (gltfPrimitive.indices != -1) {
            const gltf2::Accessor& accessor = asset.accessors[gltfPrimitive.indices]; // Get the accessor from its index (directly from indices)

            uint32_t componentSize = getAttributeSize(accessor);
            void* data = getBufferViewData(asset, accessor);
            if (!data) {
                return nullptr;
            }
            primitiveSet->addAttributeBuffer(
                data,
                componentSize,
                accessor.count,
                Render::Mesh::PrimitiveSet::Attribute::Type::Indice
            );
        }

        // Attributes
        struct {
            void* data{nullptr};
            uint32_t accessorCount{0};
        } positions; // Store positions for normals generation
        bool hasNormals = false;

        for (auto& attribute : gltfPrimitive.attributes) {
            Render::Mesh::PrimitiveSet::Attribute::Type type;
            if (attribute.first == "POSITION") {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::Position;
            } else if (attribute.first == "NORMAL") {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::Normal;
                hasNormals = true;
            } else if (attribute.first == "TANGENT") {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::Tangent;
            } else if (attribute.first.find("TEXCOORD_") != std::string::npos) {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::TexCoord;
            } else if (attribute.first.find("COLOR_") != std::string::npos) {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::Color;
            } else {
                LUG_LOG.warn("GltfLoader::createMesh Unsupported attribute {}", attribute.first);
                continue;
            }

            // TODO(nokitoo): See if we can use COLOR_0 or if we just discard it
            const gltf2::Accessor& accessor = asset.accessors[attribute.second]; // Get the accessor from its index (second in the pair)

            uint32_t componentSize = getAttributeSize(accessor);
            void* data = getBufferViewData(asset, accessor);
            if (!data) {
                return nullptr;
            }
            if (type == Render::Mesh::PrimitiveSet::Attribute::Type::Position) { // Store positions in case we need to generate the normals later
                positions.data = data;
                positions.accessorCount = accessor.count;
            }
            primitiveSet->addAttributeBuffer(data, componentSize, accessor.count, type);
        }

        // Generate flat normals if there is not any
        if (!hasNormals) {
            void* data = generateNormals((float*)positions.data, positions.accessorCount);
            if (!data) {
                return nullptr;
            }
            primitiveSet->addAttributeBuffer(
                data, sizeof(Math::Vec3f), positions.accessorCount,
                Render::Mesh::PrimitiveSet::Attribute::Type::Normal
            );
        }

        // Material
        Resource::SharedPtr<Render::Material> material = gltfPrimitive.material != -1 ? createMaterial(renderer, asset, asset.materials[gltfPrimitive.material]) : createDefaultMaterial(renderer);
        if (!material) {
            LUG_LOG.error("GltfLoader::createMesh Can't create the material resource");
            return nullptr;
        }

        primitiveSet->setMaterial(material);

        // TODO(nokitoo): set node transformations
    }

    return meshBuilder.build();
}

static bool createNode(Renderer& renderer, const gltf2::Asset& asset, const gltf2::Node& gltfNode, Scene::Node& parent) {
    Scene::Node* node = parent.createSceneNode(gltfNode.name);
    parent.attachChild(*node);

    if (gltfNode.mesh != -1) {
        Resource::SharedPtr<Render::Mesh> mesh = createMesh(renderer, asset, asset.meshes[gltfNode.mesh]);
        if (!mesh) {
            LUG_LOG.error("GltfLoader::createNode Can't create the mesh resource");
            return false;
        }
        node->attachMeshInstance(mesh);
    }

    for (uint32_t nodeIdx : gltfNode.children) {
        const gltf2::Node& childrenGltfNode = asset.nodes[nodeIdx];
        if (!createNode(renderer, asset, childrenGltfNode, *node)) {
            return false;
        }
    }

    return true;
}

Resource::SharedPtr<Resource> GltfLoader::loadFile(const std::string& filename) {
    gltf2::Asset asset;
    try {
        asset = gltf2::load(filename);
        // TODO(nokitoo): Format the asset if not already done
        // Should we store the version of format in asset.extensions or asset.copyright/asset.version ?
    } catch (gltf2::MisformattedException& e) {
        LUG_LOG.error("GltfLoader::loadFile Can't load the file \"{}\": {}", filename, e.what());
        return nullptr;
    }

    if (asset.scene == -1) { // No scene to load
        return nullptr;
    }
    const gltf2::Scene& gltfScene = asset.scenes[asset.scene];

    Builder::Scene sceneBuilder(_renderer);
    sceneBuilder.setName(gltfScene.name);

    Resource::SharedPtr<lug::Graphics::Scene::Scene> scene = sceneBuilder.build();
    if (!scene) {
        LUG_LOG.error("GltfLoader::loadFile Can't create the scene resource");
        return nullptr;
    }

    for (uint32_t nodeIdx : gltfScene.nodes) {
        const gltf2::Node& gltfNode = asset.nodes[nodeIdx];
        if (!createNode(_renderer, asset, gltfNode, scene->getRoot())) {
            return nullptr;
        }
    }

    return Resource::SharedPtr<Resource>::cast(scene);
}

} // Graphics
} // lug
