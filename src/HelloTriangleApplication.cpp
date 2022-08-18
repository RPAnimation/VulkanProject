#include "HelloTriangleApplication.hpp"
#include "VulkanUtils.hpp"

void HelloTriangleApplication::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void HelloTriangleApplication::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, APP_NAME, nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "GLFW: Couldn't create window!\n";
    }
}

void HelloTriangleApplication::initVulkan()
{
    createInstance();
}

void HelloTriangleApplication::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = APP_NAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName        = ENGINE_NAME;
	appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion         = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = 0;

	auto glfwExtensions                = getRequiredExtensions();
	createInfo.enabledExtensionCount   = glfwExtensions.size();
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();

	uint32_t vkExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(vkExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, extensions.data());

	std::cout << vkExtensionCount << " Available extensions:\n";
	for (const auto &extension : extensions)
	{
		std::cout << "\t" << extension.extensionName << "\n";
	}

	if (enableValidationLayers && checkValidationLayerSupport())
	{
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (enableValidationLayers)
		{
			populateDebugMessengerCreateInfo(debugCreateInfo, debugCallback);
			createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			createInfo.pNext               = &debugCreateInfo;

			std::cout << "Validation layers enabled.\n ";
		}
		else
		{
			createInfo.enabledLayerCount   = 0;
			createInfo.ppEnabledLayerNames = nullptr;
			createInfo.pNext               = nullptr;

			std::cout << "Validation layers disabled.\n ";
		}
	}
	else
	{
		throw std::runtime_error("Validation layers not available!");
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}
}

void HelloTriangleApplication::setupDebugMessanger()
{
	if (!enableValidationLayers)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo, debugCallback);

	VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessanger);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(err2msg(result));
	}
}
void HelloTriangleApplication::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}
void HelloTriangleApplication::cleanup()
{
	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(instance, debugMessanger, nullptr);
	}
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);

	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (char const *layerName : validationLayers)
	{
		bool isFound = false;

		for (const VkLayerProperties &layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)
		{
			return false;
		}
	}

	return true;
}

VkBool32 HelloTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << "\n";
	return VK_FALSE;
}
