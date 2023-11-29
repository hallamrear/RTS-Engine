#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <optional>

namespace Bennett
{
	class Texture;
	class Window;

	enum BENNETT_ENGINE RENDERER_DRAW_MODE
	{
		SOLID,
		WIREFRAME
	};

	struct BENNETT_ENGINE UniformBufferObject
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};

	struct BENNETT_ENGINE PushConstantBuffer
	{
		glm::mat4 ModelMatrix;
		int TextureID;
		int Padding[3];
	};

	class BENNETT_ENGINE Renderer
	{
	private:
		friend class Texture;

		const Window* m_AttachedWindow;

		RENDERER_DRAW_MODE m_DrawMode;

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR Capabilities;
			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;
		};

		const std::vector<const char*> m_DeviceExtensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			"VK_EXT_line_rasterization"
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
			
			inline bool IsComplete() const
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

		bool InitialiseCoreVulkanSystem(const Window& window, HINSTANCE hInstance);

		VkViewport m_Viewport;
		VkRect2D m_ScissorRect;

		//Instance
		VkDevice m_Device;
		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		bool CreateVulkanInstance();

		/// <summary>
		/// Destroys Vulkan Instance.
		/// </summary>
		void DestroyVulkanInstance();

		//Validation Layers
		bool CheckValidationLayerSupport();

		/// <summary>
		/// Creates a VkImage
		/// todo : finish
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="format"></param>
		/// <param name="tiling"></param>
		/// <param name="usage"></param>
		/// <param name="properties"></param>
		/// <param name="image"></param>
		/// <param name="imageMemory"></param>
		/// <returns></returns>
		bool CreateImage(VkImage& image, VkDeviceMemory& imageMemory, const VkImageCreateInfo& createInfo);

		/// <summary>
		/// Destroys a VkImage.
		/// </summary>
		void CleanupImage(VkDevice& device, VkImage& image);

		/// <summary>
		/// Creates a VkImageView
		/// todo : finish
		/// </summary>
		/// <param name="imageView"></param>
		/// <param name="image"></param>
		/// <param name="format"></param>
		/// <param name="aspectFlags"></param>
		/// <returns></returns>
		bool CreateImageView(VkImageView& imageView, const VkImageViewCreateInfo& createInfo);

		/// <summary>
		/// Destroys a VkImageView.
		/// </summary>
		/// <param name="device"></param>
		/// <param name="imageView"></param>
		void CleanupImageView(VkDevice& device, VkImageView& imageView);

		//Depth/Stencil
		VkImage m_DepthImage;
		VkDeviceMemory m_DepthImageMemory;
		VkImageView m_DepthImageView;
		bool CreateDepthResources();
		void CleanupDepthResources();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		bool HasStencilComponent(VkFormat format);

		//Extensions
		std::vector<const char*> GetRequiredExtensions() const;
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
		
		/// <summary>
		/// Creates a VkPhysicalDevice.
		/// VkPhysicalDevice's are implicitly created by vkInstance and so they get destroyed when you destroy the instance.
		/// </summary>
		/// <returns></returns>
		bool CreateLogicalDevice();

		//Graphics Queue
		VkQueue m_GraphicsQueue;
		void GetGraphicsQueue(const QueueFamilyIndices& indices);

		//Creating Surface
		VkSurfaceKHR m_Surface;
		bool CreateWindowSurface(const HWND& hWnd, const HINSTANCE& hInstance);
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
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		bool CreateSwapChain();
		bool CreateSwapChainImageViews();

		/// <summary> 
		/// Cleans up assorted swapchain dependent objects. Used in rebuilding the swapchain and renderer destruction. 
		/// Destroys: 
		/// - Depth Image, Image view and memory.
		/// - Framebuffers
		/// - Swapchain image views
		/// - Render pass
		/// - Fragment and Vertex Shader modules
		/// - SwapChain
		/// </summary>	
		void CleanupSwapChain();
		void CleanupSwapChainImageViews();

		//Render pass
		VkRenderPass m_RenderPass;
		bool CreateRenderPass();
		void CleanupRenderPass(VkDevice& device, VkRenderPass& renderpass);

		//Pipeline
		VkPipeline m_SolidGraphicsPipeline;
		VkPipeline m_WireframeGraphicsPipeline;
		VkPipelineLayout m_PipelineLayout;
		bool InitialiseGraphicsPipeline();
		void ShutdownGraphicsPipeline(VkDevice& device, VkPipeline& pipeline);
		void ShutdownGraphicsPipelineLayout(VkDevice& device, VkPipelineLayout& layout);

		//Descriptor layout
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorPool m_DescriptorPool;
		bool CreateDescriptorLayout();
		void CleanupDescriptorLayout(VkDevice& device, VkDescriptorSetLayout& layout);
		//Create descriptor pools
		bool CreateDescriptorPool();
		void CleanupDescriptorPool(VkDevice& device, VkDescriptorPool& descriptorPool);
		bool AllocateDescriptorSets();
		void DeallocateDescriptorSets(VkDevice& device, VkDescriptorPool& descriptorPool, std::vector<VkDescriptorSet>& descriptorSets);
		
		//Create descriptor sets
		std::vector<VkDescriptorSet> m_DescriptorSets;

		//Framebuffers
		int m_CurrentRenderFrame = 0;
		const int MAX_FRAMES_IN_FLIGHT = 2;
		std::vector<VkFramebuffer> m_Framebuffers;
		bool CreateFrameBuffers();
		void CleanupFrameBuffer(VkDevice& device, VkFramebuffer& framebuffer);

		//Command Queue
		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		bool CreateCommandPool();
		bool CreateCommandBuffer();
		void CleanupCommandPoolAndBuffers(VkDevice& device, VkCommandPool& commandBuffer, std::vector<VkCommandBuffer>& commandBuffers);
		bool RecordCommandBuffer();

		//Create syncronisation objects
		//e.g. fences or semaphores
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		bool CreateSyncObjects();
		void CleanupSyncObjects();

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
		void CleanupUniformBuffers();

		void BeginRenderPass();
		void EndRenderPass();

		static PushConstantBuffer m_PushConstantBuffer;

		/// <summary>
		/// Texture Sampler.
		/// </summary>
		VkSampler m_TextureSampler;

		bool CreateTextureSampler();

		/// <summary>
		/// Destroys a VkSampler.
		/// </summary>
		void CleanupSampler(VkDevice& device, VkSampler& sampler);

		/// <summary>
		/// Frees device memory objects.
		/// </summary>
		/// <param name="device"></param>
		/// <param name="memory"></param>
		void FreeDeviceMemory(VkDevice& device, VkDeviceMemory& memory);

		/// <summary>
		/// Destroys a VkBuffer.
		/// </summary>
		/// <param name="device"></param>
		/// <param name="buffer"></param>
		void CleanupBuffer(VkDevice& device, VkBuffer& buffer);

		/// <summary>
		/// Destroys a semaphore.
		/// </summary>
		/// <param name="device"></param>
		/// <param name="semaphore"></param>
		void CleanupSemaphore(VkDevice& device, VkSemaphore& semaphore);

		/// <summary>
		/// Destroys a fence.
		/// </summary>
		/// <param name="device"></param>
		/// <param name="fence"></param>
		void CleanupFence(VkDevice& device, VkFence& fence);

	public:
		static UniformBufferObject UniformMatrixBuffer;

		const VkDevice& GetDevice() const;
		const VkCommandBuffer& GetCommandBuffer() const;
		const VkPhysicalDevice& GetPhysicalDevice() const;
		const VkCommandPool& GetCommandPool() const;
		const VkQueue& GetGraphicsQueue() const;

		Renderer();
		~Renderer();

		bool Initialise(const Window& renderWindow);
		void Shutdown();

		void SetViewport(int x, int y, int w, int h, float maxDepth, float minDepth);
		void SetScissorRect(int x, int y, int w, int h);

		bool RecreateSwapChain();

		void StartFrame();
		void BindDescriptorSet() const;
		void UpdateUniformBuffers() const;
		bool UpdateDescriptorSets(const std::vector<Texture*>& textureList);
		void PushModelMatrix(const glm::mat4& modelMatrix) const;
		void PushTextureID(const int& texID) const;
		void EndFrame();

		void SetDrawMode(const RENDERER_DRAW_MODE& mode);
		void WaitForRendererIdle();

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		void RebuildDefaultShaders() const;
	};
}