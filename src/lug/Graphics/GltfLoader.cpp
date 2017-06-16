#include <lug/Graphics/GltfLoader.hpp>

#include <gltf2/glTF2.hpp>
#include <gltf2/Exceptions.hpp>

#include <lug/System/Logger/Logger.hpp>
#include <lug/Graphics/Builder/Scene.hpp>
#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Scene/Scene.hpp>

namespace lug {
namespace Graphics {

GltfLoader::GltfLoader(Renderer& renderer): Loader(renderer) {}

static void* getBufferViewData(const gltf2::Asset& asset, const gltf2::Accessor& accessor) {
    const gltf2::BufferView& bufferView = asset.bufferViews[accessor.bufferView];
    const gltf2::Buffer& buffer = asset.buffers[bufferView.buffer];

    if (buffer.uri.size()) {
        LUG_LOG.error("GltfLoader::createMesh Buffer uri not supported yet {}", buffer.uri);
        return nullptr;
    }
    else if (!buffer.data) {
        LUG_LOG.error("GltfLoader::createMesh Buffer data can't be null");
        return nullptr;
    }

    // TODO(nokitoo): handle uri
    return buffer.data + bufferView.byteOffset;
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

static Resource::SharedPtr<Render::Mesh> createMesh(Renderer& renderer, const gltf2::Asset& asset, const gltf2::Mesh& gltfMesh) {
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

        // Material
        // TODO(nokitoo)

        // Indices
        if (gltfPrimitive.indices != -1) {
            const gltf2::Accessor& accessor = asset.accessors[gltfPrimitive.indices]; // Get the accessor from its index (directly from indices)

            uint32_t componentSize = getAttributeSize(accessor);
            void* data = getBufferViewData(asset, accessor);
            if (!data) {
                return nullptr;
            }
            primitiveSet->addAttributeBuffer(data, componentSize * accessor.count, Render::Mesh::PrimitiveSet::Attribute::Type::Indice);
        }

        // Attributes
        for (auto& attribute : gltfPrimitive.attributes) {
            Render::Mesh::PrimitiveSet::Attribute::Type type;
            if (attribute.first == "POSITION") {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::Position;
            }
            else if (attribute.first == "NORMAL") {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::Normal;
            }
            else if (attribute.first == "TANGENT") {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::Tangent;
            }
            else if (attribute.first.find("TEXCOORD_") != std::string::npos) {
                type = Render::Mesh::PrimitiveSet::Attribute::Type::TexCoord;
            }
            else {
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
            primitiveSet->addAttributeBuffer(data, componentSize * accessor.count, type);
        }

    }

    return meshBuilder.build();
}

static bool createNode(Renderer& renderer, const gltf2::Asset& asset, const gltf2::Node& gltfNode, Scene::Node& parent) {
    Scene::Node* node = parent.createSceneNode(gltfNode.name);

    if (gltfNode.mesh != -1) {
        Resource::SharedPtr<Render::Mesh> mesh = createMesh(renderer, asset, asset.meshes[gltfNode.mesh]);
        if (!mesh) {
            LUG_LOG.error("GltfLoader::createNode Can't create the mesh resource");
            return false;
        }
        node->attachMeshInstance(mesh);
    }

    // TODO(nokitoo): Create material

    for (uint32_t nodeIdx : gltfNode.children) {
        const gltf2::Node& childrenGltfNode = asset.nodes[nodeIdx];
        if (!createNode(renderer, asset, childrenGltfNode, *node)) {
            return false;
        }
        // TODO(nokitoo): set node transformations
    }

    return true;
}

Resource::SharedPtr<Resource> GltfLoader::loadFile(const std::string& filename) {
    gltf2::Asset asset;
    try {
        asset = gltf2::load(filename);
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
