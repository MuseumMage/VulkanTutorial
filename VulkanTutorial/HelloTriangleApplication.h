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
#include <iostream>
#include <optional>
#include <vector>

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

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class HelloTriangleApplication {
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

    bool framebufferResized = false;

    uint32_t currentFrame = 0;
    
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
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
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
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
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

    // utils
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}
};


#endif /* HelloTriangleApplication_h */
