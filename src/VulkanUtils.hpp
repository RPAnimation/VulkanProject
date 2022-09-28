#ifndef VULKANUTILS_H
#define VULKANUTILS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <array>
#include <glm/glm.hpp>
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

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription description{};
        description.binding = 0;
        description.stride = sizeof(Vertex);
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return description;
    }
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescription()
    {
        std::array<VkVertexInputAttributeDescription, 2> description{};
        description[0].binding = 0;
        description[0].location = 0;
        description[0].format = VK_FORMAT_R32G32_SFLOAT;
        description[0].offset = offsetof(Vertex, pos);

        description[1].binding = 0;
        description[1].location = 1;
        description[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        description[1].offset = offsetof(Vertex, color);
        return description;
    }
};

// SWAP CHAIN SUPPORT CHECK

const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

const char* err2msg(VkResult code);
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

SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device,
                                              const VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities, GLFWwindow* window);

std::vector<char> readFile(const std::string &filename);

VkShaderModule createShaderModule(const VkDevice& device, const std::vector<char>& code);

uint32_t findMemoryType(const VkPhysicalDevice physicalDevice,
                        uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);

void createMemoryBuffer(const VkDevice& device,
                        const VkPhysicalDevice& physicalDevice,
                        VkDeviceSize deviceSize,
                        VkBufferUsageFlags usageFlags,
                        VkMemoryPropertyFlags properties,
                        VkBuffer& buffer,
                        VkDeviceMemory& deviceMemory);

void copyBuffer(VkBuffer srcBuffer,
                VkBuffer dstBuffer,
                VkDeviceSize size,
                const VkCommandPool& commandPool,
                const VkDevice& device,
                const VkQueue& graphicsQueue);

#endif
