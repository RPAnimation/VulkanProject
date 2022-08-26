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

struct error_codes
{
    int value;
    const char* name;
};

struct QueueFamiliyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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

bool isDeviceSuitable(const VkPhysicalDevice device, const VkSurfaceKHR surface);

QueueFamiliyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

bool checkDeviceExtensionSupport(const VkPhysicalDevice device);

#endif
