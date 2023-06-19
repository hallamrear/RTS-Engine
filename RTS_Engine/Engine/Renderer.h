#pragma once
#include <optional>
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Bennett
{
	class Texture;
	class Window;

	struct UniformBufferObject
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};

	struct PushConstantBuffer
	{
		glm::mat4 ModelMatrix;
		int TextureID;
		int Padding[3];
	};

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
		const std::vector<const char*> m_ValidationLayers = 
		{ 
			"VK_LAYER_KHRONOS_validation"
		};

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

		bool CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		bool CreateImageView(VkImageView& imageView, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

		//Depth/Stencil
		VkImage m_DepthImage;
		VkDeviceMemory m_DepthImageMemory;
		VkImageView m_DepthImageView;
		bool CreateDepthResources();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		bool HasStencilComponent(VkFormat format);

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

		//Create descriptor layout
		VkDescriptorSetLayout m_DescriptorSetLayout;
		bool CreateDescriptorLayout();
		bool CreateDescriptorPool();
		bool AllocateDescriptorSets();
		//Create descriptor pools
		VkDescriptorPool m_DescriptorPool;
		//Create descriptor sets
		std::vector<VkDescriptorSet> m_DescriptorSets;

		//Framebuffers
		int m_CurrentRenderFrame = 0;
		const int MAX_FRAMES_IN_FLIGHT = 2;
		std::vector<VkFramebuffer> m_Framebuffers;
		bool CreateFrameBuffers();

		//Command Queue
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		bool CreateCommandPool();
		bool CreateCommandBuffer();
		bool RecordCommandBuffer();

		//Create syncronisation objects
		//e.g. fences or semaphores
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		bool CreateSyncObjects();

		//Wait for the previous frame to finish
		void WaitForFrame();

		uint32_t m_CurrentImageIndex;
		//Acquire an image from the swap chain
		void AquireSwapchainImageIndex();
		//Submit the recorded command buffer
		void SubmitCommandData();
		//Present the swap chain image
		void Present(uint32_t& imageIndex);


		//Uniform Buffers
		std::vector<VkBuffer> m_UniformBuffers;
		std::vector<VkDeviceMemory> m_UniformBuffersMemory;
		std::vector<void*> m_UniformBuffersMapped;
		bool CreateUniformBuffers();

		void BeginRenderPass();
		void EndRenderPass();

		static PushConstantBuffer m_PushConstantBuffer;

		bool CreateTextureSampler();
		VkSampler m_TextureSampler;

	public:
		static UniformBufferObject UniformMatrixBuffer;

		const VkDevice& GetDevice() const;
		const VkCommandBuffer& GetCommandBuffer() const;
		const VkPhysicalDevice& GetPhysicalDevice() const;
		const VkCommandPool& GetCommandPool() const;
		const VkQueue& GetGraphicsQueue() const;

		Renderer();

		bool Initialise(Window& window);
		void Shutdown();

		void SetViewport(int x, int y, int w, int h, float maxDepth, float minDepth);
		void SetScissorRect(int x, int y, int w, int h);

		void StartFrame();
		void BindDescriptorSet() const;
		void UpdateUniformBuffers() const;
		bool UpdateDescriptorSets(const std::vector<Texture*>& textureList);
		void PushModelMatrix(const glm::mat4& modelMatrix) const;
		void PushTextureID(const int& texID) const;
		void EndFrame();

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		void RebuildDefaultShaders();
		
		~Renderer();
	};
}