//
//  HelloTriangleApplication.h
//  VulkanTutorial
//
//  Created by 张博 on 2023/5/25.
//

#ifndef HelloTriangleApplication_h
#define HelloTriangleApplication_h

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <vector>
#include <array>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Vulkan Tutorial
// Setup
//  1. Instance
//  2. Physical Device
//  3. Logical Device
// Presentation
//  1. Surface
//  2. Swap Chain
//  3. Image Views
// Pipeline
//  1. Graphics Pipeline
//  2. Render Pass
// Drawing
//  1. Framebuffers
//  2. Command pools
//  3. Synchronization
//      * Wait for the previous frame to finish
//      * Acquire an image from the swap chain
//      * Record a command buffer which draws the scene onto that image
//      * Submit the recorded command buffer
//      * Present the swap chain image

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	// Binding descriptions
	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	// Attribute descriptions
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		// pos
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		// float: VK_FORMAT_R32_SFLOAT
		// vec2: VK_FORMAT_R32G32_SFLOAT
		// vec3: VK_FORMAT_R32G32B32_SFLOAT
		// vec4: VK_FORMAT_R32G32B32A32_SFLOAT
		// ivec2: VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed integers
		// uvec4: VK_FORMAT_R32G32B32A32_UINT, a 4-component vector of 32-bit unsigned integers
		// double: VK_FORMAT_R64_SFLOAT, a double-precision (64-bit) float
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		// color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class HelloTriangleApplication
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VkInstance instance;

	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;

	// inflight frames
	const int MAX_FRAMES_IN_FLIGHT = 2;

	// validation layers
	const std::vector<const char*> validationLayers{
		"VK_LAYER_KHRONOS_validation"
	};
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
	// debug msg
	VkDebugUtilsMessengerEXT debugMessenger;

	// physical device
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	// logical device
	VkDevice device;
	VkQueue graphicsQueue;

	// window surface
	VkSurfaceKHR surface;
	VkQueue presentQueue;

	// swap chain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// ImageViews
	std::vector<VkImageView> swapChainImageViews;

	// Graphics pipeline
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	// Render pass
	VkRenderPass renderPass;

	// Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;

	// Command pools
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	// Semaphores and Fences
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	// Vertex buffer
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	// Index buffer
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	// Descriptor pool
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	// Member variables
	bool framebufferResized = false;
	uint32_t currentFrame = 0;

	// Vertex
	const std::vector<Vertex> vertices = 
	{
	    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	// index
	const std::vector<uint16_t> indices = 
	{
		0, 1, 2, 2, 3, 0
	};

	// Uniform buffer
	struct UniformBufferObject
	{
	    glm::mat4 model;
	    glm::mat4 view;
	    glm::mat4 proj;
	};

private:
	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();

	// vk instance
	void createInstance();
	void destroyInstance();

	// debug msg
	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	                                      const VkAllocationCallbacks* pAllocator,
	                                      VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
	                                   const VkAllocationCallbacks* pAllocator);
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	// physical device
	void pickPhysicalDevice();

	// logical device
	void createLogicalDevice();
	void destroyDevice();

	// surface
	void createSurface();
	void destroySurface();

	// swapchain
	void createSwapChain();
	void destroySwapChain();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// ImageView
	void createImageViews();
	void destroyImageViews();

	// Graphics Pipeline
	void createGraphicsPipeline();
	void destroyGraphicsPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);

	// Render Pass
	void createRenderPass();
	void destroyRenderPass();

	// Framebuffers
	void createFramebuffers();
	void destroyFramebuffers();

	// Command pools
	void createCommandPool();
	void destroyCommandPool();
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	// Draw
	void drawFrame();
	void createSyncObjects();
	void destroySyncObjects();

	// Recreate swap chain
	void recreateSwapChain();
	void cleanupSwapChain();

	// Create vertex buffer
	void createVertexBuffer();
	void destroyVertexBuffer();

	// Create index buffer
	void createIndexBuffer();
	void destroyIndexBuffer();

	// Uniform buffer
	void createDescriptorSetLayout();
	void destroyDescriptorSetLayout();
	void createUniformBuffers();
	void destroyUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);

	// Descriptor pool
	void createDescriptorPool();
	void createDescriptorSets();
	void destroyDescriptorPool();

	// utils
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
	                  VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
};


#endif /* HelloTriangleApplication_h */
