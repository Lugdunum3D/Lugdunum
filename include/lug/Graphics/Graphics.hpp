#pragma once

#include <algorithm>
#include <memory>
#include <set>
#include <lug/Graphics/Camera.hpp>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Mesh.hpp>
#include <lug/Graphics/Model.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Scene.hpp>

namespace lug {
namespace Graphics {

/**
 * @brief      Class for graphics.
 *
 *             This class is the root class of the 3D engine. It handles modules, will create
 *             the Graphics::Renderer based on the @p rendererType given in the Graphics::InitInfo
 *             structure. @n
 *             It also handles Scene, Camera and Mesh creation.
 */
class LUG_GRAPHICS_API Graphics {
public:
    /**
     * @brief      Utility structure used to initialize the Graphics component.
     */
    struct InitInfo {
        Renderer::Type rendererType;            /** The renderer to use. */
        Renderer::InitInfo rendererInitInfo;    /** Initialization info for the renderer. */
    };

public:
    /**
     * @brief      Constructs a Graphics instance.
     *
     * @param[in]  appName     The application name.
     * @param[in]  appVersion  The application version.
     */
    Graphics(const char* appName, uint32_t appVersion);

    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;

    Graphics& operator=(const Graphics&) = delete;
    Graphics& operator=(Graphics&&) = delete;

    ~Graphics() = default;

    /**
     * @brief      Initializes the application with the informations filled in @p initInfo structure.
     *
     * @param[in]  initInfo  The initialize information.
     *
     * @return     @p true if the initialization was successful.
     */
    bool init(const InitInfo& initInfo);

    /**
     * @brief      Check if a module is loaded
     *
     * @param[in]  type  The module type to check.
     *
     * @return     @p true if the module is loaded.
     */
    bool isModuleLoaded(Module::Type type) const;
    const std::set<Module::Type>& getLoadedModules() const;

    /**
     * @brief      Gets the renderer.
     *
     * @return     A raw pointer to the renderer instance.
     */
    Renderer* getRenderer() const;

    /**
     * @brief      Creates a scene.
     *
     * @return     A pointer to the scene instance. The ownership is given to the caller.
     */
    std::unique_ptr<Scene> createScene();

    /**
     * @brief      Creates a mesh.
     *
     * @param[in]  name  The name of the mesh.
     *
     * @return     A pointer to the scene instance. The ownership is given to the caller. @n
     *             Returns @p nullptr if the mesh creation fails.
     */
    std::unique_ptr<Mesh> createMesh(const std::string& name);

    /**
     * @brief      Creates a model.
     *
     * @param[in]  name  The name of the model.
     *
     * @return     A pointer to the scene instance. The ownership is given to the caller. @n
     *             Returns @p nullptr if the model creation fails.
     */
    std::unique_ptr<Model> createModel(const std::string& name, const std::string& fileName = "");

    /**
     * @brief      Creates a camera.
     *
     * @param[in]  name  The name of the camera.
     *
     * @return     A pointer to the camera instance. The ownership is given to the caller. @n
     *             Returns @p nullptr if the camera creation fails.
     */
    std::unique_ptr<Camera> createCamera(const std::string& name);

private:
    const char* _appName;
    uint32_t _appVersion;

    std::set<Module::Type> _loadedModules{};
    std::unique_ptr<Renderer> _renderer{nullptr};
    Renderer::Type _rendererType;
};

#include <lug/Graphics/Graphics.inl>

} // Graphics
} // lug
