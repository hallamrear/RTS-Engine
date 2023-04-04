#pragma once
#include <optional>
#include <vulkan/vulkan.h>

namespace Bennett
{
	class Renderer
	{
	private:
		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};

		const std::vector<const char*> m_DeviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		struct QueueFamilyIndices
		{
			//std::optional is a wrapper that contains no value until you assign it with something
			//at any point, you can query if it contains an value by calling has_value()
			std::optional<uint32_t> GraphicsFamily;

			QueueFamilyIndices()
			{
				GraphicsFamily.reset();
			}
			
			inline bool IsComplete()
			{
				return GraphicsFamily.has_value();
			}
		};

		/// <summary>
		/// This function is an extension function and so I need to look up the function address to
		/// create a proxy function to handle this.
		/// </summary>
		inline VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance, 
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator, 
			VkDebugUtilsMessengerEXT* pDebugMessenger)
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) 
			{
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else 
			{
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}
		/// <summary>
		/// This function is an extension function and so I need to look up the function address to
		/// create a proxy function to handle this.
		/// </summary>
		inline void DestroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger, const
			VkAllocationCallbacks* pAllocator)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) 
			{
				func(instance, debugMessenger, pAllocator);
			}
		}

		//This is a vulkan designated callback function.
		static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback
		(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
			void* pUserData
		);

		//This variable is used to determine which validation layers we want to enable.
		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

		//This variable is used to determine whether we want to enable validation layers.
		#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
		const bool m_BuildShadersAtRuntime = false;
		#else
		const bool m_EnableValidationLayers = true;
		const bool m_BuildShadersAtRuntime = true;
		#endif

		bool m_IsInitialised;

		bool InitialiseCoreVulkanSystem(GLFWwindow& window);

		VkViewport m_Viewport;
		VkRect2D m_ScissorRect;

		//Instance
		VkDevice m_Device;
		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		bool CreateVulkanInstance();
		void DestroyVulkanInstance();

		//Validation Layers
		bool CheckValidationLayerSupport();

		//Extensions
		std::vector<const char*> GetRequiredExtensions();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		//Debug Messenger (Optional)
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		void CreateDebugMessenger();
		void DestroyDebugMessenger();

		//Physical Device - This is destroyed by the instance so does not appear in cleanup
		bool PickPhysicalDevice();
		bool IsPhysicalDeviceSuitable(VkPhysicalDevice device);

		//Queue Families
		void FindQueueFamilies(VkPhysicalDevice device, QueueFamilyIndices& indices);
		
		//Logical Device
		bool CreateLogicalDevice();

		//Graphics Queue
		VkQueue m_GraphicsQueue;
		void GetGraphicsQueue(const QueueFamilyIndices& indices);

		//Creating Surface
		VkSurfaceKHR m_Surface;
		bool CreateWindowSurface(GLFWwindow& window);
		void DestroyWindowSurface();

		//Swapchain
		VkSwapchainKHR m_SwapChain;
		VkFormat m_SwapChainFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;
		std::vector<VkImage> m_SwapChainImages;
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window);
		bool CreateSwapChain(GLFWwindow& window);
		bool CreateSwapChainImageViews();

		//Shaders
		VkShaderModule m_FragShaderModule;
		VkShaderModule m_VertShaderModule;
		static std::vector<char> ReadShaderFile(const std::string& fileName);
		VkShaderModule CreateShaderModule(const std::string& fileName);

		//Render pass
		VkRenderPass m_RenderPass;
		bool CreateRenderPass();

		//Pipeline
		VkPipeline m_GraphicsPipeline;
		VkPipelineLayout m_PipelineLayout;
		bool InitialiseGraphicsPipeline();

		//Framebuffers
		std::vector<VkFramebuffer> m_Framebuffers;
		bool CreateFrameBuffers();

		//Command Queue
		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CommandBuffer;
		bool CreateCommandPool();
		bool CreateCommandBuffer();
		bool RecordCommandBuffer(uint32_t imageIndex);

		//Create syncronisation objects
		//e.g. fences or semaphores
		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinishedSemaphore;
		VkFence		m_InFlightFence;
		bool CreateSyncObjects();

		//Wait for the previous frame to finish
		void WaitForFrame();

		//Acquire an image from the swap chain
		uint32_t AquireSwapchainImageIndex();
		//Record a command buffer which draws the scene onto that image
		void Render();
		//Submit the recorded command buffer
		void SubmitCommandData();
		//Present the swap chain image
		void Present(uint32_t& imageIndex);
	public:
		Renderer();

		bool Initialise(GLFWwindow& window);
		void Shutdown();

		void SetViewport(int x, int y, int w, int h, float maxDepth, float minDepth);
		void SetScissorRect(int x, int y, int w, int h);

		void RenderFrame();
		
		~Renderer();
	};
}