#ifndef __HELLO_TRIANGLE_APPLICAITON
#define __HELLO_TRIANGLE_APPLICAITON

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class HelloTriangleApplication
{
const uint32_t WIDTH = 1600;
const uint32_t HEIGHT = 1200;

public:
    void run();
private:
    GLFWwindow* window;

    void initVulkan();
    void initWindow();
    void mainLoop();
    void cleanup();
};


#endif