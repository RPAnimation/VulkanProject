#ifndef VULKANUTILS_H
#define VULKANUTILS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <vulkan/vulkan.hpp>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct error_codes {
    int value;
    const char* name;
};

struct QueueFamiliyIndices
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() { return graphicsFamily.has_value(); }
};

const char* err2msg(int code);
std::vector<const char*> getRequiredExtensions();
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger);
void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo,
                                      PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);

bool isSuitablePhysicalDevice(const VkPhysicalDevice& device);

QueueFamiliyIndices findQueueFamilies(VkPhysicalDevice device);

#endif
