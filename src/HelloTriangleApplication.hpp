#ifndef __HELLO_TRIANGLE_APPLICAITON
#define __HELLO_TRIANGLE_APPLICAITON

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

class HelloTriangleApplication
{
public:
    const char* APP_NAME = "First Vulkan App";
    const char* ENGINE_NAME = "No engine";
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    void run();
private:
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessanger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice = VK_NULL_HANDLE;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;

    // Main phase
    void initWindow();
    void mainLoop();
    void cleanup();

    // Vulkan phase
    void initVulkan();
    void createInstance();
    void setupDebugMessanger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();

    // Support functions
    bool checkValidationLayerSupport();
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                  void* pUserData);
};

#endif
