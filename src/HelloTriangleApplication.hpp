#ifndef __HELLO_TRIANGLE_APPLICAITON
#define __HELLO_TRIANGLE_APPLICAITON

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class HelloTriangleApplication
{
public:
    const char* APP_NAME = "First Vulkan App";
    const char* ENGINE_NAME = "No engine";
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    void run();
private:
    GLFWwindow* window;
    VkInstance instance;

    void initVulkan();
    void createInstance();
    void initWindow();
    void mainLoop();
    void cleanup();
};

#endif
