#pragma once

#include <cstdint>
#include <type_traits>

#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

/**
 * @brief      Class for resource.
 */
class LUG_GRAPHICS_API Resource {
public:
    /**
     * @brief      Type of the resource.
     */
    enum class Type : uint8_t {
        Scene,      ///< A scene
        SceneNode,  ///< A scene node (a scene is composed of scene nodes, to make a tree)
        Material,   ///< A material
        Mesh,       ///< A mesh
        Texture,    ///< A texture
        Pipeline,   ///< A graphical pipeline
        Camera      ///< A camera
    };

    /**
     * @brief      Handle of the resource.
     *             It contains informations such as the type and the index in the ResourceManager's
     *             internal vector, i.e. the index of the Resource in this vector.
     */
    struct Handle {
        union {
            struct {
                uint32_t type : 8;      ///< #Type of the ressource.
                uint32_t index : 24;    ///< Index of the Resource in the ResourceManager's internal storage.
            };

            uint32_t value;             ///< Access of the raw value of the above bytefield.
        };

        explicit operator uint32_t() {
            return value;
        }

        bool operator==(const Handle& other) {
            return value == other.value;
        }
    };

    /**
     * @brief      Dummy class for a shared pointer.
     *
     * @tparam     T     The type of the pointer.
     */
    template <typename T>
    class SharedPtr {
        static_assert(
            std::is_base_of<Resource, T>::value,
            "T must inherit from Resource"
        );

    public:
        constexpr SharedPtr(T* pointer);

        SharedPtr(const SharedPtr<T>& rhs);
        SharedPtr(SharedPtr<T>&& rhs);

        SharedPtr<T>& operator=(const SharedPtr<T>& rhs);
        SharedPtr<T>& operator=(SharedPtr<T>&& rhs);

        ~SharedPtr();

        T& operator*() const;
        T* operator->() const;

    private:
        T* _resource{nullptr};
    };

    /**
     * @brief      Dummy class for a weak ptr
     *
     * @tparam     T     The type of the pointer
     */
    template <typename T>
    class WeakPtr {
        static_assert(
            std::is_base_of<Resource, T>::value,
            "T must inherit from Resource"
        );

    public:
        constexpr WeakPtr(T* pointer);
        constexpr WeakPtr(const SharedPtr<T>& rhs);

        WeakPtr(const WeakPtr<T>& rhs);
        WeakPtr(WeakPtr<T>&& rhs);

        WeakPtr<T>& operator=(const WeakPtr<T>& rhs);
        WeakPtr<T>& operator=(WeakPtr<T>&& rhs);

        ~WeakPtr();

        /**
         * @brief      Transforms a WeakPtr to a SharedPtr
         */
        SharedPtr<T> lock();

    private:
        T* _resource{nullptr};
    };

public:
    Resource() = default;
    ~Resource() = default;

    Resource(const Resource&) = delete;
    Resource(Resource&&) = delete;

    Resource& operator=(const Resource&) = delete;
    Resource& operator=(Resource&&) = delete;

    /**
     * @brief      Returns the type of the Resource.
     *
     * @return     The type.
     */
    Type getType() const;

    /**
     * @brief      Returns the handle of the Resource.
     *
     * @return     The handle.
     */
    Handle getHandle() const;

private:
    Handle _handle;
};

#include <lug/Graphics/Resource.inl>

} // Graphics
} // lug
