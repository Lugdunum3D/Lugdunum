#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <lug/Graphics/ModelLoader.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/System/Logger/Logger.hpp>
#if defined(LUG_SYSTEM_ANDROID)
    #include <assimp/port/AndroidJNI/AndroidJNIIOSystem.h>
    #include <lug/Window/Android/WindowImplAndroid.hpp>
#endif
namespace lug {
namespace Graphics {

// TODO: Move in helper class
std::string  getFileExtension(const std::string& fileName) {
    std::size_t size = fileName.rfind('.', fileName.length());
    if (size != std::string::npos)
        return (fileName.substr(size + 1, fileName.length() - size));

    return ("");
}

bool ModelLoader::loadFromFile(Model* model, const std::string& file) {
    Assimp::Importer importer;

#if defined(LUG_SYSTEM_ANDROID)
    Assimp::AndroidJNIIOSystem* ioSystem = new Assimp::AndroidJNIIOSystem(::lug::Window::priv::WindowImpl::activity);
    importer.SetIOHandler(ioSystem);
#endif

    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

    LUG_LOG.info("Loading model {}", file);

    const aiScene* scene = importer.ReadFile(file,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_FixInfacingNormals |
        aiProcess_GenUVCoords |
        aiProcess_ValidateDataStructure |
        aiProcess_ImproveCacheLocality |
        aiProcess_RemoveRedundantMaterials |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData |
        aiProcess_OptimizeMeshes |
        aiProcess_GenSmoothNormals);

    if (!scene) {
        LUG_LOG.error("ModelLoader::loadFromFile: Failed to load model {}: {}", file, importer.GetErrorString());
        return false;
    }

    uint32_t verticesOffset = 0;
    uint32_t indicesOffset = 0;

    for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
        std::unique_ptr<Model::Mesh> mesh = std::make_unique<Model::Mesh>("mesh", verticesOffset, indicesOffset);

        auto& assimpMesh = scene->mMeshes[i];
        aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
        aiColor3D color;

        for (uint32_t j = 0; j < assimpMesh->mNumVertices; j++) {
            Mesh::Vertex vertex;

            vertex.pos = {
                assimpMesh->mVertices[j].x,
                assimpMesh->mVertices[j].y,
                assimpMesh->mVertices[j].z
            };

            // Color
            if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
                vertex.color = {color.r, color.g, color.b};
            }
            else if (assimpMesh->HasVertexColors(j)) {
                vertex.color = {
                    assimpMesh->mColors[j]->r,
                    assimpMesh->mColors[j]->g,
                    assimpMesh->mColors[j]->b
                };
            }
            else {
                vertex.color = {1.0f, 1.0f, 1.0f};
            }

            // UV
            if (assimpMesh->HasTextureCoords(0)) {
                vertex.uv = {
                    assimpMesh->mTextureCoords[0][j].x,
                    -assimpMesh->mTextureCoords[0][j].y
                };

            }
            else {
                vertex.uv = {0.0f, 0.0f};
            }

            // Normals
            if (assimpMesh->HasNormals()) {
                vertex.normal = {
                    assimpMesh->mNormals[j].x,
                    assimpMesh->mNormals[j].y,
                    assimpMesh->mNormals[j].z
                };
            }
            else {
                vertex.normal = {1.0f, 1.0f, 1.0f};
            }

            mesh->vertices.push_back(vertex);
            verticesOffset++;
        }

        for (uint32_t j = 0; j < assimpMesh->mNumFaces; j++) {
            auto &&face = assimpMesh->mFaces[j];
            for (int k = 0; k < 3; k++) {
                mesh->indices.push_back(face.mIndices[k]);
                indicesOffset++;
            }
        }

        model->addMesh(std::move(mesh));
    }

    return true;
}

} // Graphics
} // lug
