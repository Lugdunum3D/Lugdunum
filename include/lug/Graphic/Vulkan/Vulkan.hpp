#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <lug/Graphic/Export.hpp>

#define LUG_CORE_VULKAN_FUNCTIONS(macro)            \
    macro(vkEnumerateInstanceExtensionProperties)   \
    macro(vkEnumerateInstanceLayerProperties)       \
    macro(vkCreateInstance)                         \
    macro(vkGetInstanceProcAddr)                    \
    macro(vkGetDeviceProcAddr)

#define LUG_INSTANCE_VULKAN_FUNCTIONS(macro) // TODO
#define LUG_DEVICE_VULKAN_FUNCTIONS(macro) // TODO

#define LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS(name) extern PFN_##name LUG_GRAPHIC_API name;
LUG_CORE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
LUG_DEVICE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
#undef LUG_DEFINE_DECLARATION__VULKAN_FUNCTIONS

