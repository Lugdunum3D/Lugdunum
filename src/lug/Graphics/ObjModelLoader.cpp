#define TINYOBJLOADER_IMPLEMENTATION
#include <lug/Ext/tinyobjloader/tiny_obj_loader.h>

#include <lug/Graphics/ObjModelLoader.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {

bool ObjModelLoader::loadFromFile(Model* model, const std::string& file) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str())) {
        LUG_LOG.error("ObjModelLoader::loadFromFile error loading lodel \"{}\": {}", file, err);
        return false;
    }

    uint32_t verticesOffset = 0;
    uint32_t indicesOffset = 0;

    for (const auto& shape : shapes) {
        std::unique_ptr<Model::Mesh> mesh = std::make_unique<Model::Mesh>("mesh", verticesOffset, indicesOffset);
        for (const auto& index : shape.mesh.indices) {
            // TODO: Remove duplicates
            Mesh::Vertex vertex;
            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };
            vertex.normal = {
                attrib.normals[3 * index.vertex_index + 0],
                attrib.normals[3 * index.vertex_index + 1],
                attrib.normals[3 * index.vertex_index + 2]
            };

            if (attrib.texcoords.size() > 0) {
                vertex.uv = {
                    attrib.texcoords[2 * index.vertex_index + 0],
                    1.0f - attrib.texcoords[2 * index.vertex_index + 1]
                };
            }
            vertex.color = {
                0.0f, 0.0f, 1.0f
            };

            uint32_t vertexIndice = 0;
            // Search duplicate vertex
            {
                bool duplicate = false;
                for (auto& meshVertex: mesh->vertices) {
                    if (meshVertex == vertex) {
                        duplicate = true;
                        break;
                    }
                    vertexIndice++;
                }

                // Add vertex
                if (!duplicate) {
                    mesh->vertices.push_back(vertex);
                    verticesOffset++;
                }
            }

            mesh->indices.push_back(vertexIndice);
            indicesOffset++;

        }
        model->addMesh(std::move(mesh));
    }

    return true;
}

} // Graphics
} // lug
