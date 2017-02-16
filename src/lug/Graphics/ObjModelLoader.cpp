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

    int result = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file.c_str());
    if (err.size() != 0) {
        LUG_LOG.error("ObjModelLoader::loadFromFile error loading lodel \"{}\": {}", file, err);
    }

    if (!result) {
        return false;
    }


    uint32_t verticesOffset = 0;
    uint32_t meshIndicesOffset = 0;

    LUG_LOG.info("Loading model \"{}\"", file);
    LUG_LOG.info("Nb Materials: {}", materials.size());
    for (const auto& shape : shapes) {
        uint32_t indicesOffset = 0;
        uint32_t faceNb = 0;
        std::unique_ptr<Model::Mesh> mesh = std::make_unique<Model::Mesh>("mesh", verticesOffset, meshIndicesOffset);

        // Loop over faces
        for (const auto& faceVertices: shape.mesh.num_face_vertices) {
            // Loop over face vertices
            for (uint32_t i = 0; i < faceVertices; ++i) {
                tinyobj::index_t index = shape.mesh.indices[indicesOffset + i];
                tinyobj::material_t* material = nullptr;

                if (shape.mesh.material_ids.size() > 0 && shape.mesh.material_ids[faceNb] != -1) {
                    material = &materials[shape.mesh.material_ids[faceNb]];
                }

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

                if (material) {
                    vertex.color = {
                        material->diffuse[0],
                        material->diffuse[1],
                        material->diffuse[2]
                    };
                }
                else {
                    vertex.color = {
                        1.0f, 1.0f, 1.0f
                    };
                }

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
            }
            indicesOffset += faceVertices;
            faceNb++;
        }
        model->addMesh(std::move(mesh));
        meshIndicesOffset += indicesOffset;
    }

    return true;
}

} // Graphics
} // lug
