#include "VulkanUtils.hpp"
#include <fstream>
#include <limits>
#include <set>

const struct error_codes global_error_codes[] = {
    {VK_SUCCESS, "VK_SUCCESS"},
    {VK_NOT_READY, "VK_NOT_READY"},
    {VK_TIMEOUT, "VK_TIMEOUT"},
    {VK_EVENT_SET, "VK_EVENT_SET"},
    {VK_EVENT_RESET, "VK_EVENT_RESET"},
    {VK_INCOMPLETE, "VK_INCOMPLETE"},
    {VK_ERROR_OUT_OF_HOST_MEMORY, "VK_ERROR_OUT_OF_HOST_MEMORY"},
    {VK_ERROR_OUT_OF_DEVICE_MEMORY, "VK_ERROR_OUT_OF_DEVICE_MEMORY"},
    {VK_ERROR_INITIALIZATION_FAILED, "VK_ERROR_INITIALIZATION_FAILED"},
    {VK_ERROR_DEVICE_LOST, "VK_ERROR_DEVICE_LOST"},
    {VK_ERROR_MEMORY_MAP_FAILED, "VK_ERROR_MEMORY_MAP_FAILED"},
    {VK_ERROR_LAYER_NOT_PRESENT, "VK_ERROR_LAYER_NOT_PRESENT"},
    {VK_ERROR_EXTENSION_NOT_PRESENT, "VK_ERROR_EXTENSION_NOT_PRESENT"},
    {VK_ERROR_FEATURE_NOT_PRESENT, "VK_ERROR_FEATURE_NOT_PRESENT"},
    {VK_ERROR_INCOMPATIBLE_DRIVER, "VK_ERROR_INCOMPATIBLE_DRIVER"},
    {VK_ERROR_TOO_MANY_OBJECTS, "VK_ERROR_TOO_MANY_OBJECTS"},
    {VK_ERROR_FORMAT_NOT_SUPPORTED, "VK_ERROR_FORMAT_NOT_SUPPORTED"},
    {VK_ERROR_FRAGMENTED_POOL, "VK_ERROR_FRAGMENTED_POOL"},
    {VK_ERROR_UNKNOWN, "VK_ERROR_UNKNOWN"}};

const char *err2msg(VkResult code)
{
	for (int i = 0; global_error_codes[i].name; ++i)
	{
		if (global_error_codes[i].value == code)
		{
			return global_error_codes[i].name;
		}
	}
	return "Unknown error";
}

std::vector<const char *> getRequiredExtensions()
{
	uint32_t     glfwExtensionCount = 0;
	const char **glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance                                instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                      const VkAllocationCallbacks              *pAllocator,
                                      VkDebugUtilsMessengerEXT                 *pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
	    glfwGetInstanceProcAddress(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     debugMessenger,
                                   const VkAllocationCallbacks *pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
	    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT  &createInfo,
                                      PFN_vkDebugUtilsMessengerCallbackEXT debugCallback)
{
	createInfo                 = {};
	createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	                             VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	                             VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	                         VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	                         VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData       = nullptr;
}

bool isDeviceSuitable(const VkPhysicalDevice device, const VkSurfaceKHR surface)
{
	QueueFamiliyIndices        indices = findQueueFamilies(device, surface);
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures   deviceFeatures;

	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	bool isSwapChainSupported = checkDeviceExtensionSupport(device);
	bool isSwapChainAdequate  = false;

	if (isSwapChainSupported)
	{
		SwapChainSupportDetails swapChainDetails = querySwapChainSupport(device, surface);
		isSwapChainAdequate                      = !swapChainDetails.formats.empty() && !swapChainDetails.presentModes.empty();
	}

	return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
	        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
	       deviceFeatures.geometryShader && indices.isComplete() && isSwapChainSupported && isSwapChainAdequate && deviceFeatures.samplerAnisotropy;
}

QueueFamiliyIndices findQueueFamilies(const VkPhysicalDevice &device, const VkSurfaceKHR &surface)
{
	QueueFamiliyIndices indices;
	uint32_t            queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto &family : queueFamilies)
	{
		if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
		{
			indices.presentFamily = i;
		}
		if (indices.isComplete())
		{
			break;
		}
		i++;
	}
	return indices;
}

bool checkDeviceExtensionSupport(const VkPhysicalDevice device)
{
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto &extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}
	return requiredExtensions.empty();
}

SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &device, const VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
	uint32_t surfaceFormatsCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surfaceFormatsCount, nullptr);

	if (surfaceFormatsCount != 0)
	{
		details.formats.resize(surfaceFormatsCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surfaceFormatsCount, details.formats.data());
	}

	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
	for (const auto &format : availableFormats)
	{
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
	for (const auto &presentMode : availablePresentModes)
	{
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfaceCapabilities, GLFWwindow *window)
{
	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return surfaceCapabilities.currentExtent;
	}
	int width, height = {0};
	glfwGetFramebufferSize(window, &width, &height);

	VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

	actualExtent.width  = std::clamp(actualExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
	actualExtent.height = std::clamp(actualExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);

	return actualExtent;
}

std::vector<char> readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open a file \"" + filename + "\"");
    }
    size_t            fileSize = file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

VkShaderModule createShaderModule(const VkDevice &device, const std::vector<char> &code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.pCode    = reinterpret_cast<const uint32_t *>(code.data());
	createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	VkShaderModule shaderModule;
	VkResult       result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}
	return shaderModule;
}

uint32_t findMemoryType(const VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
	for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) && (deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}
	throw std::runtime_error("Failed to find suitable memory type!");
}

void createMemoryBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice, VkDeviceSize deviceSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &deviceMemory)
{
	VkBufferCreateInfo bufferCreateInfo{};
	bufferCreateInfo.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size        = deviceSize;
	bufferCreateInfo.usage       = usageFlags;
	bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	bufferCreateInfo.flags       = 0;

	VkResult result = vkCreateBuffer(device, &bufferCreateInfo, nullptr, &buffer);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}

	VkMemoryRequirements memRequirements{};
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo memoryAllocInfo{};
	memoryAllocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocInfo.allocationSize  = memRequirements.size;
	memoryAllocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	result = vkAllocateMemory(device, &memoryAllocInfo, nullptr, &deviceMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}

	vkBindBufferMemory(device, buffer, deviceMemory, 0);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkCommandPool &commandPool, const VkDevice &device, const VkQueue &graphicsQueue)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, device);

	VkBufferCopy copyRegion{};
	copyRegion.dstOffset = 0;
	copyRegion.srcOffset = 0;
	copyRegion.size      = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	endSingleTimeCommands(device, commandPool, commandBuffer, graphicsQueue);
}

void createImage(int32_t textureWidth, int32_t textureHeight, int32_t mipLevels, const VkPhysicalDevice &physicalDevice,
                 const VkDevice &logicalDevice, VkImage &textureImage, VkDeviceMemory &textureImageMemory,
                 VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkImageCreateInfo imageCreateInfo{};
	imageCreateInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType     = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.width  = textureWidth;
	imageCreateInfo.extent.height = textureHeight;
	imageCreateInfo.extent.depth  = 1;
	imageCreateInfo.mipLevels     = mipLevels;
	imageCreateInfo.arrayLayers   = 1;
	imageCreateInfo.format        = format;
	imageCreateInfo.tiling        = tiling;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage         = usage;
	imageCreateInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
	imageCreateInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.flags         = 0;

	VkResult result = vkCreateImage(logicalDevice, &imageCreateInfo, nullptr, &textureImage);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(logicalDevice, textureImage, &memoryRequirements);

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.memoryTypeIndex = findMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, properties);
	allocateInfo.allocationSize  = memoryRequirements.size;

	result = vkAllocateMemory(logicalDevice, &allocateInfo, nullptr, &textureImageMemory);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}

	vkBindImageMemory(logicalDevice, textureImage, textureImageMemory, 0);
}

VkCommandBuffer beginSingleTimeCommands(const VkCommandPool &commandPool, const VkDevice &device)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool        = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}

void endSingleTimeCommands(const VkDevice &device, const VkCommandPool &commandPool, const VkCommandBuffer &commandBuffer, const VkQueue &graphicsQueue)
{
	vkEndCommandBuffer(commandBuffer);
	VkSubmitInfo submitInfo{};
	submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers    = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void transitionImageLayout(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue &graphicsQueue, const VkImageLayout &oldLayout, const VkImageLayout &newLayout, VkImage &image, VkFormat format, uint32_t mipLevels)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, device);

	VkImageMemoryBarrier barrier{};
	barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout                       = oldLayout;
	barrier.newLayout                       = newLayout;
	barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.image                           = image;
	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}
	barrier.subresourceRange.baseMipLevel   = 0;
	barrier.subresourceRange.levelCount     = mipLevels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount     = 1;
	barrier.srcAccessMask                   = 0;
	barrier.dstAccessMask                   = 0;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else
	{
		throw std::runtime_error("Unsupported layout transition!");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	endSingleTimeCommands(device, commandPool, commandBuffer, graphicsQueue);
}

void copyBufferToImage(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue &graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, device);

	VkBufferImageCopy region{};
	region.bufferOffset      = 0;
	region.bufferRowLength   = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel       = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount     = 1;

	region.imageOffset = {0, 0, 0};
	region.imageExtent = {width, height, 1};

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	endSingleTimeCommands(device, commandPool, commandBuffer, graphicsQueue);
}

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags flags, uint32_t mipLevels)
{
	VkImageViewCreateInfo createInfo{};
	createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image                           = image;
	createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format                          = format;
	createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.subresourceRange.aspectMask     = flags;
	createInfo.subresourceRange.baseMipLevel   = 0;
	createInfo.subresourceRange.levelCount     = mipLevels;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount     = 1;
	VkImageView imageView;
	VkResult    result = vkCreateImageView(device, &createInfo, nullptr, &imageView);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}
	return imageView;
}

VkFormat findSuitableFormat(const VkPhysicalDevice &physicalDevice, const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}
	throw std::runtime_error("Failed to find supported format!");
}

void generateMipmaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue, VkPhysicalDevice physicalDevice)
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
	{
		throw std::runtime_error("texture format doesn't support linear blitting!");
	}

	VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, device);

	VkImageMemoryBarrier barrier{};
	barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image                           = image;
	barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount     = 1;
	barrier.subresourceRange.levelCount     = 1;

	int32_t mipWidth  = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; ++i)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkImageBlit imageBlit{};
		imageBlit.srcOffsets[0]                 = {0, 0, 0};
		imageBlit.srcOffsets[1]                 = {mipWidth, mipHeight, 1};
		imageBlit.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		imageBlit.srcSubresource.mipLevel       = i - 1;
		imageBlit.srcSubresource.baseArrayLayer = 0;
		imageBlit.srcSubresource.layerCount     = 1;

		imageBlit.dstOffsets[0]                 = {0, 0, 0};
		imageBlit.dstOffsets[1]                 = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1};
		imageBlit.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		imageBlit.dstSubresource.mipLevel       = i;
		imageBlit.dstSubresource.baseArrayLayer = 0;
		imageBlit.dstSubresource.layerCount     = 1;

		vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		if (mipWidth > 1)
		{
			mipWidth /= 2;
		}

		if (mipHeight > 1)
		{
			mipHeight /= 2;
		}
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	endSingleTimeCommands(device, commandPool, commandBuffer, graphicsQueue);
}
