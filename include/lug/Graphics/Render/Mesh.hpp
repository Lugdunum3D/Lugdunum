#pragma once

#include <cstdint>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Render/Material.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

// For friend
namespace Builder {
class Mesh;
} // Builder

namespace Render {

/**
 * @brief      A Mesh, which itsef is a Resource.
 *             A mesh is composed of a vector of PrimitiveSet.
 * @see        Mesh::PrimitiveSet
 */
class LUG_GRAPHICS_API Mesh : public Resource {
    friend class ::lug::Graphics::Builder::Mesh;

public:
    /**
     * @brief      Describes part of a Mesh.
     */
    struct PrimitiveSet {
        struct Attribute {
            enum class Type : uint8_t {
                Indice,     ///< Indices (UNSIGNED_SHORT)
                Position,   ///< Position (VEC3<FLOAT>)
                Normal,     ///< Normal (VEC3<FLOAT>)
                TexCoord,   ///< UV (VEC2<FLOAT>)
                Tangent     ///< Tangent (VEC4<FLOAT> w component is a sign value (-1 or +1) indicating handedness of the tangent basis)
            } type;

            /**
             * @brief      Access to the data of the attribute.
             */
            struct Buffer {
                char* data{nullptr};    ///< The data of the buffer.
                uint32_t size{0};       ///< The size of the above data buffer, in bytes.
            } buffer;
        };

        /**
         * @brief      Type of the primitive set, defaults to Triangles.
         */
        enum class Mode : uint8_t {
            Points = 0,         ///< Each vertex defines a separate point
            Lines = 1,          ///< The first two vertices define the first segment, with subsequent pairs of vertices each defining one more segment
            LineStrip = 3,      ///< The first vertex specifies the first segment’s start point while the second vertex specifies the first segment’s endpoint and the second segment’s start point
            Triangles = 4,      ///<
            TriangleStrip = 5,  ///<
            TriangleFan = 6     ///<
        } mode{Mode::Triangles};

        std::vector<Attribute> attributes;

        Attribute* indices{nullptr};
        Attribute* position{nullptr};
        Attribute* normal{nullptr};
        std::vector<Attribute*> texCoords{};
        Attribute* tangent{nullptr};

        Resource::SharedPtr<Material> material{nullptr};

        void* _data{nullptr}; // Specific to each Renderer
    };

public:
    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    virtual ~Mesh();

    const std::string& getName() const;
    void setName(const std::string &name);

protected:
    explicit Mesh(const std::string& name);

protected:
    std::string _name;

    std::vector<PrimitiveSet> _primitiveSets;
};

#include <lug/Graphics/Render/Mesh.inl>

} // Render
} // Graphics
} // lug
