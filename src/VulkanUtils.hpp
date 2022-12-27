#ifndef VULKANUTILS_H
#define VULKANUTILS_H

#include <array>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

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

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription description{};
		description.binding   = 0;
		description.stride    = sizeof(Vertex);
		description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return description;
	}
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> description{};
		description[0].binding  = 0;
		description[0].location = 0;
		description[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
		description[0].offset   = offsetof(Vertex, pos);

		description[1].binding = 0;
		description[1].location = 1;
		description[1].format   = VK_FORMAT_R32G32B32_SFLOAT;
		description[1].offset   = offsetof(Vertex, color);

		description[2].binding  = 0;
		description[2].location = 2;
		description[2].format   = VK_FORMAT_R32G32_SFLOAT;
		description[2].offset   = offsetof(Vertex, texCoord);
		return description;
	}
	bool operator==(const Vertex &other) const
	{
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};

namespace std
{
template <>
struct hash<Vertex>
{
	size_t operator()(Vertex const &vertex) const
	{
		return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1));
	}
};
}        // namespace std

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
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
void                     populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT  &createInfo,
                                                          PFN_vkDebugUtilsMessengerCallbackEXT debugCallback);

bool isDeviceSuitable(const VkPhysicalDevice device, const VkSurfaceKHR surface);

QueueFamiliyIndices findQueueFamilies(const VkPhysicalDevice &device, const VkSurfaceKHR &surface);

bool checkDeviceExtensionSupport(const VkPhysicalDevice device);

SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &device,
                                              const VkSurfaceKHR      surface);
VkSurfaceFormatKHR      chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities, GLFWwindow *window);

std::vector<char> readFile(const std::string &filename);

VkShaderModule createShaderModule(const VkDevice &device, const std::vector<char> &code);

uint32_t findMemoryType(const VkPhysicalDevice physicalDevice,
                        uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);

void createMemoryBuffer(const VkDevice         &device,
                        const VkPhysicalDevice &physicalDevice,
                        VkDeviceSize            deviceSize,
                        VkBufferUsageFlags      usageFlags,
                        VkMemoryPropertyFlags   properties,
                        VkBuffer               &buffer,
                        VkDeviceMemory         &deviceMemory);

void copyBuffer(VkBuffer             srcBuffer,
                VkBuffer             dstBuffer,
                VkDeviceSize         size,
                const VkCommandPool &commandPool,
                const VkDevice      &device,
                const VkQueue       &graphicsQueue);

void createImage(int32_t textureWidth, int32_t textureHeight, int32_t mipLevels, const VkPhysicalDevice &physicalDevice,
                 const VkDevice &logicalDevice, VkImage &textureImage, VkDeviceMemory &textureImageMemory,
                 VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);

VkCommandBuffer beginSingleTimeCommands(const VkCommandPool &commandPool, const VkDevice &device);
void            endSingleTimeCommands(const VkDevice &device, const VkCommandPool &commandPool, const VkCommandBuffer &commandBuffer, const VkQueue &graphicsQueue);

void transitionImageLayout(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue &graphicsQueue, const VkImageLayout &oldLayout, const VkImageLayout &newLayout, VkImage &image, VkFormat format, uint32_t mipLevels);

void copyBufferToImage(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue &graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags flags, uint32_t mipLevels);

VkFormat findSuitableFormat(const VkPhysicalDevice &physicalDevice, const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

void generateMipmaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice);

#endif
