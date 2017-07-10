#pragma once

#include <cstdint>
#include <type_traits>
#include <string>

#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

class ResourceManager;

/**
 * @brief      Class for resource.
 */
class LUG_GRAPHICS_API Resource {
    friend class lug::Graphics::ResourceManager;

public:
    /**
     * @brief      Type of the resource.
     */
    enum class Type : uint8_t {
        Scene,      ///< A scene
        SceneNode,  ///< A scene node (a scene is composed of scene nodes, to make a tree)
        Material,   ///< A material
        Mesh,       ///< A mesh
        Light,      ///< A light
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

        bool operator==(const Handle& other) const {
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
        template <typename RhsT>
        friend class SharedPtr;

        static_assert(
            std::is_base_of<Resource, T>::value,
            "T must inherit from Resource"
        );

    public:
        constexpr SharedPtr(T* pointer = nullptr);

        SharedPtr(const SharedPtr<T>& rhs);
        SharedPtr(SharedPtr<T>&& rhs);

        SharedPtr<T>& operator=(const SharedPtr<T>& rhs);
        SharedPtr<T>& operator=(SharedPtr<T>&& rhs);

        ~SharedPtr();

        T& operator*() const;
        T* operator->() const;

        explicit operator bool() const {
            return _resource != nullptr;
        }

        T* get() const;

    public:
        /**
         * @brief      Dynamic casting of a SharedPtr to another one (RhsT to T)
         *
         * @tparam     RhsT     The type of the old SharedPtr
         */
        template <typename RhsT>
        static SharedPtr<T> cast(const SharedPtr<RhsT>& rhs);

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
        template <typename RhsT>
        friend class WeakPtr;

        static_assert(
            std::is_base_of<Resource, T>::value,
            "T must inherit from Resource"
        );

    public:
        constexpr WeakPtr(T* pointer = nullptr);
        constexpr WeakPtr(const SharedPtr<T>& rhs);

        WeakPtr(const WeakPtr<T>& rhs);
        WeakPtr(WeakPtr<T>&& rhs);

        WeakPtr<T>& operator=(const WeakPtr<T>& rhs);
        WeakPtr<T>& operator=(WeakPtr<T>&& rhs);

        ~WeakPtr();

        /**
         * @brief      Transforms a WeakPtr to a SharedPtr
         */
        SharedPtr<T> lock() const;

    public:
        /**
         * @brief      Dynamic casting of a WeakPtr to another one (RhsT to T)
         *
         * @tparam     RhsT     The type of the old WeakPtr
         */
        template <typename RhsT>
        static WeakPtr<T> cast(const WeakPtr<RhsT>& rhs);

    private:
        T* _resource{nullptr};
    };

public:
    Resource(Type type, const std::string& name);

    Resource(const Resource&) = delete;
    Resource(Resource&&) = delete;

    Resource& operator=(const Resource&) = delete;
    Resource& operator=(Resource&&) = delete;

    virtual ~Resource() = default;

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

    /**
     * @brief      Gets the name of the Resource.
     *
     * @return     The name.
     */
    const std::string& getName() const;

    /**
     * @brief      Sets the name of the Resource.
     *
     * @param[in]  name  The name
     */
    void setName(const std::string &name);

protected:
    std::string _name;

private:
    Handle _handle;
};

#include <lug/Graphics/Resource.inl>

} // Graphics
} // lug
