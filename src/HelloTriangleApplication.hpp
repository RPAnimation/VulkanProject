#ifndef __HELLO_TRIANGLE_APPLICAITON
#define __HELLO_TRIANGLE_APPLICAITON

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "VulkanUtils.hpp"

class HelloTriangleApplication
{
  public:
	const char	                 *APP_NAME             = "First Vulkan App";
	const char	                 *ENGINE_NAME          = "No engine";
	const uint32_t                  WIDTH                = 800;
	const uint32_t                  HEIGHT               = 600;
	const std::string               MODEL_OBJ_FILEPATH   = "./models/nefertiti.obj";
	const std::string               MODEL_TEX_FILEPATH   = "./textures/nefertiti.png";
	const std::vector<const char *> validationLayers     = {"VK_LAYER_KHRONOS_validation"};
	const int                       MAX_FRAMES_IN_FLIGHT = 2;
	void                            run();
	bool                            framebufferResized = false;

  private:
	// Device setup
	GLFWwindow              *window;
	VkInstance               instance;
	VkDebugUtilsMessengerEXT debugMessanger;
	VkPhysicalDevice         physicalDevice = VK_NULL_HANDLE;
	VkDevice                 logicalDevice  = VK_NULL_HANDLE;
	VkQueue                  graphicsQueue;
	VkSurfaceKHR             surface;
	VkQueue                  presentQueue;

	// Swap chain
	VkSwapchainKHR             swapChain;
	std::vector<VkImage>       swapChainImages;
	VkFormat                   swapChainImageFormat;
	VkExtent2D                 swapChainExtent;
	std::vector<VkImageView>   swapChainImageViews;
	VkRenderPass               renderPass;
	VkDescriptorSetLayout      descriptorSetLayout;
	VkPipelineLayout           pipelineLayout;
	VkPipeline                 graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool              commandPool;

	// Vertex/Index buffers -> UBO
	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;

	VkBuffer                     vertexBuffer;
	VkDeviceMemory               vertexBufferMemory;
	VkBuffer                     indexBuffer;
	VkDeviceMemory               indexBufferMemory;
	std::vector<VkBuffer>        uniformBuffers;
	std::vector<VkDeviceMemory>  uniformBuffersMemory;
	VkDescriptorPool             descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkCommandBuffer> commandBuffers;

	// Sync objects
	std::vector<VkFence>     inFlightFences;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;

	// Textures
	uint32_t       mipLevels;
	VkImage        textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView    textureImageView;
	VkSampler      textureSampler;

	// Depth
	VkImage        depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView    depthImageView;

	// Main phase
	void initWindow();
	void initVulkan();
	void mainLoop();
	void drawFrame();
	void cleanup();

	// Vulkan phase
	void createInstance();
	void setupDebugMessanger();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void recreateSwapChain();
	void cleanupSwapChain();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createDepthResources();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void loadModel();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();
	void createCommandBuffers();
	void createSyncObjects();

	// Helpful variables
	uint32_t currentFrame = 0;

	// Support functions
	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL
	     debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
	                   VkDebugUtilsMessageTypeFlagsEXT             messageType,
	                   const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
	                   void	                                   *pUserData);
	void recordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);
	void updateUniformBuffer(uint32_t currentImage);
};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

#endif
