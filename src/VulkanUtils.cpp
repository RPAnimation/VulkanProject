#include "VulkanUtils.hpp"
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

const char *err2msg(int code)
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

	return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
	        deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
	       deviceFeatures.geometryShader && indices.isComplete() && isSwapChainSupported;
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
