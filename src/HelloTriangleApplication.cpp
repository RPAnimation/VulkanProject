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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(HEIGHT, WIDTH, "Vulkan", nullptr, nullptr);
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