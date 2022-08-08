#include "HelloTriangleApplication.hpp"


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
    window = glfwCreateWindow(HEIGHT, WIDTH, "Vulkan", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "GLFW: Couldn't create window!\n";
    }
}

void HelloTriangleApplication::initVulkan()
{

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
    glfwDestroyWindow(window);
    glfwTerminate();
}