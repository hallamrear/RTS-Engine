#include <BennettPCH.h>
#include <System/ServiceLocator.h>
#include <System/Manager/FileManagement.h>
#include <Rendering/Renderer.h>
#include <Rendering/Buffer.h>
#include <Rendering/Texture.h>
#include <Rendering/Window.h>
#include <Rendering/Vertex.h>
#include <Rendering/ShaderLoader.h>

#include <set>

#define INIT_CHECK(func) if(func != true) { m_AttachedWindow = NULL; return false; } 
#define DESCRIPTOR_COUNT 2

namespace Bennett
{
	UniformBufferObject Renderer::UniformMatrixBuffer = UniformBufferObject();
	PushConstantBuffer Renderer::PushConstants = PushConstantBuffer();
	const CustomPipeline* Renderer::m_CurrentPipeline = nullptr;
	const CustomPipeline* Renderer::m_PendingPipeline = nullptr;

	std::vector<Vertex> Renderer::m_DebugLineList = std::vector<Vertex>();
	int Renderer::m_CurrentDebugLineCount = 0;

	/// <summary>
	/// Function comes from an extension 'VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME' so we need a variable to store the func addr.
	/// </summary>
	PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;

	Renderer::Renderer()
	{
		m_SwapChain = VK_NULL_HANDLE;

		m_DepthImage = VkImage{};
		m_DepthImageMemory = VkDeviceMemory{};
		m_Viewport = VkViewport{};
		m_CommandPool = VkCommandPool{};
		m_CurrentImageIndex = -1;
		m_DebugMessenger = VkDebugUtilsMessengerEXT{};
		m_PhysicalDevice = VK_NULL_HANDLE;
		m_IsInitialised = false;
		m_Instance = nullptr;
		m_AttachedWindow = nullptr;
		m_CurrentRenderFrame = 0;
		m_DebugTexture = new Texture();
		m_CurrentPipeline = nullptr;
		m_PendingPipeline = nullptr;
		m_PipelineNeedsChanging = false;
	}

	Renderer::~Renderer()
	{
		if (m_IsInitialised)
		{
			Shutdown();
		}

		delete m_DebugTexture;
		m_DebugTexture = nullptr;
	}

	bool Renderer::Initialise(const Window& renderWindow)
	{
		Shutdown();
		m_IsInitialised = InitialiseCoreVulkanSystem(renderWindow, GetModuleHandle(NULL));
		return m_IsInitialised;
	}

	bool Renderer::InitialiseCoreVulkanSystem(const Window& window, HINSTANCE hInstance)
	{
		m_AttachedWindow = &window;

		INIT_CHECK(CreateVulkanInstance())
		CreateDebugMessenger();
		INIT_CHECK(CreateWindowSurface(m_AttachedWindow->GetWindowHandle(), hInstance))
		INIT_CHECK(PickPhysicalDevice())
		INIT_CHECK(CreateLogicalDevice())
		INIT_CHECK(RecreateSwapChain())
		INIT_CHECK(CreateCommandPool())
		INIT_CHECK(CreateTextureSampler())
		INIT_CHECK(CreateDescriptorLayout())
		INIT_CHECK(InitialiseGraphicsPipeline())
		INIT_CHECK(CreateUniformBuffers())
		INIT_CHECK(CreateCommandBuffer())
		INIT_CHECK(CreateSyncObjects())
		INIT_CHECK(CreateRenderingDebugAssets())

		return true;
	}

	bool Renderer::CreateRenderingDebugAssets()
	{
		if (m_DebugTexture->Loaded())
		{
			Texture::Destroy(*m_DebugTexture);
		}
		Texture::Create(*m_DebugTexture, ServiceLocator::GetResourceFolderLocation() + "Required/debug.png");

		CustomPipelineDetails lineRenderingPipelineDetails{};
		lineRenderingPipelineDetails.Cullmode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
		lineRenderingPipelineDetails.PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
		lineRenderingPipelineDetails.Topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

		bool pipelineSuccessful = CreateCustomPipeline(m_DebugLinePipeline, lineRenderingPipelineDetails);
		if (pipelineSuccessful != true)
		{
			Log("Failed to create debug line rendering graphics custom pipeline.", LOG_SERIOUS);
			return false;
		}

		m_DebugLineList = std::vector<Vertex>();
		m_DebugLineList.resize(MAX_DEBUG_LINE_COUNT * 2);
		m_CurrentDebugLineCount = 0;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeof(Vertex) * MAX_DEBUG_LINE_COUNT * 2;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (VertexBuffer::Create(m_DebugLineVertexBuffer, bufferInfo, (void*)m_DebugLineList.data(), m_DebugLineList.size()) == false)
		{
			return false;
		}

		return true;
	}

	bool Renderer::CreateCommandBuffer()
	{
		m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;

		if (vkAllocateCommandBuffers(m_Device, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			Log("Failed to allocate command buffer.", LOG_CRITICAL);
			return false;
		}

		return true;
	}

	void Renderer::WaitForRendererIdle()
	{
		vkDeviceWaitIdle(m_Device);
	}

	void Renderer::CleanupCommandPoolAndBuffers(VkDevice& device, VkCommandPool& commandPool, std::vector<VkCommandBuffer>& commandBuffers)
	{
		vkFreeCommandBuffers(device, commandPool, (uint32_t)commandBuffers.size(), commandBuffers.data());
		commandBuffers.clear();
		vkDestroyCommandPool(device, commandPool, nullptr);
		commandPool = VK_NULL_HANDLE;
	}

	bool Renderer::RecordCommandBuffer()
	{
		/*
		* Flags parameter determins how you use the command buffer.
		* VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT - The command buffer will be rerecorded right after executing it once.
		* VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT - This is a secondary command buffer used entirely within a single render pass.
		* VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT - The command buffer can be resubmitted while it is also pending exectution.
		*/
		
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;
		beginInfo.pNext = nullptr;

		if (vkBeginCommandBuffer(m_CommandBuffers[m_CurrentRenderFrame], &beginInfo) != VK_SUCCESS)
		{
			Log("Failed to begin recording command buffer.", LOG_CRITICAL);
			return false;
		}

		vkCmdSetViewport(m_CommandBuffers[m_CurrentRenderFrame], 0, 1, &m_Viewport);
		vkCmdSetScissor(m_CommandBuffers[m_CurrentRenderFrame], 0, 1, &m_ScissorRect);

		return true;
	}

	void Renderer::BeginRenderPass()
	{
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { { 0.245f, 0.245f, 0.245f, 1.0f } };
		clearValues[0].color = { { 0.21568627450980393f, 0.25882352941176473f, 0.34901960784313724f, 1.0f } };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.framebuffer = m_Framebuffers[m_CurrentImageIndex];
		renderPassInfo.renderArea.offset = { 0 ,0 };
		renderPassInfo.renderArea.extent = m_SwapChainExtent;
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentRenderFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::EndRenderPass()
	{
		vkCmdEndRenderPass(m_CommandBuffers[m_CurrentRenderFrame]);
	}

	void Renderer::CleanupSwapChain()
	{
		CleanupDepthResources();

		for (size_t i = 0; i < m_Framebuffers.size(); i++)
		{
			CleanupFrameBuffer(m_Device, m_Framebuffers[i]);
		}
		m_Framebuffers.clear();

		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
		{
			CleanupImageView(m_Device, m_SwapChainImageViews[i]);
		}
		m_SwapChainImageViews.clear();

		CleanupRenderPass(m_Device, m_RenderPass);

		//Swapchain image creation is handled by the swapchain itself.
		//so we do not need to clear them up (m_SwapChainImages).
		vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
		m_SwapChain = VK_NULL_HANDLE;

		//^^ doesnt mean i cant tidy up the data.
		// todo: Check this is ok
		for (size_t i = 0; i < m_SwapChainImages.size(); i++)
		{
			m_SwapChainImages[i] = VK_NULL_HANDLE;
		}
		m_SwapChainImages.clear();
	}

	void Renderer::CleanupSwapChainImageViews()
	{
		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
		{
			CleanupImageView(m_Device, m_SwapChainImageViews[i]);
		}
	}

	bool Renderer::RecreateSwapChain()
	{
		WaitForRendererIdle();

		CleanupSwapChain();

		INIT_CHECK(CreateSwapChain())
		INIT_CHECK(CreateRenderPass())
		INIT_CHECK(CreateSwapChainImageViews())
		INIT_CHECK(CreateDepthResources())
		INIT_CHECK(CreateFrameBuffers())

		SetScissorRect(0, 0, m_SwapChainExtent.width, m_SwapChainExtent.height);
		SetViewport(0, 0, m_SwapChainExtent.width, m_SwapChainExtent.height, 1.0f, 0.0f);

		return true;
	}

	bool Renderer::CreateTextureSampler()
	{
		/*
			VK_SAMPLER_ADDRESS_MODE_REPEAT: Repeat the texture when going beyond the image dimensions.
			VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT: Like repeat, but inverts the coordinates to mirror the image when going beyond the dimensions.
			VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE: Take the color of the edge closest to the coordinate beyond the image dimensions.
			VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE: Like clamp to edge, but instead uses the edge opposite to the closest edge.
			VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER: Return a solid color when sampling beyond the dimensions of the image.
		*/

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &props);

		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = VK_FILTER_LINEAR;
		createInfo.minFilter = VK_FILTER_LINEAR;
		createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		createInfo.anisotropyEnable = VK_TRUE;
		createInfo.maxAnisotropy = props.limits.maxSamplerAnisotropy;
		createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		createInfo.unnormalizedCoordinates = VK_FALSE;
		createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		createInfo.mipLodBias = 0.0f;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = 0.0f;
		createInfo.compareEnable = VK_FALSE;
		createInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		if (vkCreateSampler(m_Device, &createInfo, nullptr, &m_TextureSampler) != VK_SUCCESS) 
		{
			Log("Failed to create texture sampler.", LOG_SERIOUS);
			return false;
		}

		return true;
	}

	void Renderer::CleanupSampler(VkDevice& device, VkSampler& sampler)
	{
		vkDestroySampler(device, sampler, nullptr);
		sampler = VK_NULL_HANDLE;
	}

	void Renderer::StartFrame()
	{
		WaitForFrame();
		AquireSwapchainImageIndex();

		//Reset
		vkResetCommandBuffer(m_CommandBuffers[m_CurrentRenderFrame], 0);
		//Record
		RecordCommandBuffer();

		BeginRenderPass();

		if (m_PipelineNeedsChanging)
		{
			m_CurrentPipeline = m_PendingPipeline;
			m_PendingPipeline = nullptr;
			m_PipelineNeedsChanging = false;
			assert(m_CurrentPipeline != nullptr);
		}

		assert(m_CurrentPipeline != nullptr);
		vkCmdBindPipeline(m_CommandBuffers[m_CurrentRenderFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_CurrentPipeline->m_Pipeline);
	}

	const CustomPipeline* Renderer::GetCurrentGraphicsPipeline() const
	{
		return m_CurrentPipeline;
	}

	void Renderer::DrawAllPendingLines()
	{
		if (m_CurrentDebugLineCount > 0)
		{
			VkCommandBuffer debugLineDrawCommandBuffer = BeginSingleTimeCommands();
			//Update line list vertex buffer with pending lines.
			vkCmdUpdateBuffer(
				debugLineDrawCommandBuffer,
				m_DebugLineVertexBuffer.Object(),
				0,
				(sizeof(Vertex) * m_CurrentDebugLineCount * 2),
				m_DebugLineList.data());

			EndSingleTimeCommands(debugLineDrawCommandBuffer);

			//Draw all.
			const CustomPipeline& tempPipeline = *m_CurrentPipeline;
			SetCustomGraphicsPipeline(m_DebugLinePipeline);
			m_DebugLineVertexBuffer.Bind();
			PushConstants.ModelMatrix = glm::mat4(1.0f);
			UpdatePushConstants();
			PushDescriptorSet(nullptr);
			UpdateUniformBuffers();
			vkCmdDraw(GetCommandBuffer(), m_CurrentDebugLineCount * 2, 1, 0, 0);
			SetCustomGraphicsPipeline(tempPipeline);

			//Reset the memory index.
			m_CurrentDebugLineCount = 0;
		}
	}

	void Renderer::EndFrame()
	{
		DrawAllPendingLines();

		EndRenderPass();

		if (vkEndCommandBuffer(m_CommandBuffers[m_CurrentRenderFrame]) != VK_SUCCESS)
		{
			Log("Failed to record command buffer.", LOG_CRITICAL);
			return;
		}

		SubmitCommandData();
		Present(m_CurrentImageIndex);
		m_CurrentRenderFrame = (m_CurrentRenderFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	VkCommandBuffer Renderer::BeginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void Renderer::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_GraphicsQueue);

		vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);
	}

	void Renderer::RebuildDefaultShaders() const
	{
		if (m_BuildShadersAtRuntime)
		{
			const std::string& location = ServiceLocator::GetResourceFolderLocation();
			std::vector<std::filesystem::path> shaderPaths = {};
			FileManagement::GetListOfFilePathsWithExtension(".frag", location, true, shaderPaths);
			FileManagement::GetListOfFilePathsWithExtension(".vert", location, true, shaderPaths);

			for (std::filesystem::path& itr : shaderPaths)
			{
				std::cout << itr.string() << std::endl;
			}

			std::filesystem::path outputLocation = "";
			std::string rebuildShadersCommand = "";

			for (size_t i = 0; i < shaderPaths.size(); i++)
			{
				outputLocation = shaderPaths[i];
				outputLocation.replace_extension(".spv");
				rebuildShadersCommand = "%VULKAN_SDK%\\Bin\\glslc.exe " + shaderPaths[i].string() + " -o " + outputLocation.string();
				system(rebuildShadersCommand.c_str());
			}

			//todo : load the new vulkan shaders into the pipeline.
			//Will need to recreate the pipeline using the new objects.
		}
	}

	void Renderer::AquireSwapchainImageIndex()
	{
		VkResult result = vkAcquireNextImageKHR(m_Device, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentRenderFrame], VK_NULL_HANDLE, &m_CurrentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
		{
			Log("failed to acquire swap chain image!", LOG_CRITICAL);
		}
	}

	void Renderer::WaitForFrame()
	{
		vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentRenderFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentRenderFrame]);
	}

	void Renderer::UpdateUniformBuffers() const
	{
		memcpy(m_UniformBuffersMapped[m_CurrentRenderFrame], &UniformMatrixBuffer, sizeof(UniformMatrixBuffer));
	}

	void Renderer::SetCustomGraphicsPipelineNextFrame(const CustomPipeline& pipeline)
	{
		m_PendingPipeline = &pipeline;
		m_PipelineNeedsChanging = true;
	}

	void Renderer::SetCustomGraphicsPipeline(const CustomPipeline& pipeline, const VkCommandBuffer& commandBuffer) const
	{
		m_CurrentPipeline = &pipeline;
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_CurrentPipeline->m_Pipeline);
	}

	void Renderer::SetCustomGraphicsPipeline(const CustomPipeline& pipeline) const
	{
		m_CurrentPipeline = &pipeline;
		vkCmdBindPipeline(m_CommandBuffers[m_CurrentRenderFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_CurrentPipeline->m_Pipeline);
	}

	void Renderer::SetWireframeGraphicsPipeline() const
	{
		SetCustomGraphicsPipeline(m_WireframePipeline);
	}

	void Renderer::SetSolidGraphicsPipeline() const
	{
		SetCustomGraphicsPipeline(m_SolidPipeline);
	}

	void Renderer::UpdatePushConstants() const
	{
		vkCmdPushConstants(m_CommandBuffers[m_CurrentRenderFrame], m_CurrentPipeline->m_Layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantBuffer), &PushConstants);
	}

	void Renderer::SubmitCommandData()
	{
		UpdateUniformBuffers();

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphore[] = { m_ImageAvailableSemaphores[m_CurrentRenderFrame] };
		VkSemaphore signalSemaphore[] = { m_RenderFinishedSemaphores[m_CurrentRenderFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentRenderFrame];
		submitInfo.commandBufferCount = 1;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphore;

		if (vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentRenderFrame]) != VK_SUCCESS)
		{
			Log("Failed to submit the graphics queue.", LOG_CRITICAL);
		}
	}

	void Renderer::Present(uint32_t& imageIndex)
	{
		VkSemaphore waitSemaphore[] = { m_ImageAvailableSemaphores[m_CurrentRenderFrame] };
		VkSemaphore signalSemaphore[] = { m_RenderFinishedSemaphores[m_CurrentRenderFrame] };
		VkSwapchainKHR swapchain[] = {m_SwapChain};
		//SUBMITTING
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphore;
		presentInfo.pSwapchains = swapchain;
		presentInfo.swapchainCount = 1;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		VkResult result = vkQueuePresentKHR(m_GraphicsQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			Log("failed to present swap chain image!", LOG_CRITICAL);
		}
	}

	bool Renderer::CreateUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		m_UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);
		m_UniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = bufferSize;
			bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(m_Device, &bufferInfo, nullptr, &m_UniformBuffers[i]))
			{
				Log("Failed to create uniform buffer.", LOG_SERIOUS);
				return false;
			}

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(m_Device, m_UniformBuffers[i], &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = Bennett::Buffer::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &m_UniformBuffersMemory[i]) != VK_SUCCESS)
			{
				Log("Failed to allocate memory for uniform buffer.", LOG_SERIOUS);
				return false;
			}

			vkBindBufferMemory(m_Device, m_UniformBuffers[i], m_UniformBuffersMemory[i], 0);
			vkMapMemory(m_Device, m_UniformBuffersMemory[i], 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
		}

		return true;
	}

	void Renderer::CleanupUniformBuffers()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			CleanupBuffer(m_Device, m_UniformBuffers[i]);
			vkUnmapMemory(m_Device, m_UniformBuffersMemory[i]);
			FreeDeviceMemory(m_Device, m_UniformBuffersMemory[i]);
		}
		m_UniformBuffers.clear();
		m_UniformBuffersMemory.clear();
	}

	void Renderer::FreeDeviceMemory(VkDevice& device, VkDeviceMemory& memory)
	{
		vkFreeMemory(device, memory, nullptr);
		memory = VK_NULL_HANDLE;
	}

	void Renderer::CleanupBuffer(VkDevice& device, VkBuffer& buffer)
	{
		vkDestroyBuffer(device, buffer, nullptr);
		buffer = VK_NULL_HANDLE;
	}

	void Renderer::CleanupSemaphore(VkDevice& device, VkSemaphore& semaphore)
	{
		vkDestroySemaphore(device, semaphore, nullptr);
		semaphore = VK_NULL_HANDLE;
	}

	void Renderer::CleanupFence(VkDevice& device, VkFence& fence)
	{
		vkDestroyFence(device, fence, nullptr);
		fence = VK_NULL_HANDLE;
	}

	void Renderer::DrawDebugLine(const glm::vec3& start, const glm::vec3& end) const
	{
		if (m_CurrentDebugLineCount == MAX_DEBUG_LINE_COUNT)
		{
			Log("Trying to draw too many debug lines in one frame. Max is 100.\n", LOG_STATUS::LOG_MINIMAL);
			return;
		}

		m_DebugLineList[(m_CurrentDebugLineCount * 2) + 0] = start;
		m_DebugLineList[(m_CurrentDebugLineCount * 2) + 1] = end;
		m_CurrentDebugLineCount++;
	}

	void Renderer::DrawDebugLine(const glm::vec3& start, const glm::vec3& dir, const float& length) const
	{
		glm::vec3 end = start + (dir * length);
		DrawDebugLine(start, end);
	}

	const VkDevice& Renderer::GetDevice() const
	{
		return m_Device;
	}

	const VkCommandBuffer& Renderer::GetCommandBuffer() const
	{
		return m_CommandBuffers[m_CurrentRenderFrame];
	}

	const VkPhysicalDevice& Renderer::GetPhysicalDevice() const
	{
		return m_PhysicalDevice;
	}

	const VkCommandPool& Renderer::GetCommandPool() const
	{
		return m_CommandPool;
	}

	const VkQueue& Renderer::GetGraphicsQueue() const
	{
		return m_GraphicsQueue;
	}

	void Renderer::SetViewport(int x, int y, int w, int h, float maxDepth = 1.0f, float minDepth = 0.0f)
	{
		m_Viewport.height = (float)h;
		m_Viewport.width = (float)w;
		m_Viewport.x = (float)x;
		m_Viewport.y = (float)y;
		m_Viewport.maxDepth = maxDepth;
		m_Viewport.minDepth = minDepth;
	}

	void Renderer::SetScissorRect(int x, int y, int w, int h)
	{
		m_ScissorRect.extent.width = w;
		m_ScissorRect.extent.height = h;
		m_ScissorRect.offset.x = x;
		m_ScissorRect.offset.y = y;
	}
	
	bool Renderer::CreateCommandPool()
	{
		QueueFamilyIndices familyIndices;
		FindQueueFamilies(m_PhysicalDevice, familyIndices);
		
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = familyIndices.GraphicsFamily.value();

		if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		{
			Log("Failed to create the command pool.", LOG_CRITICAL);
			return false;
		}

		return true;
	}

	bool Renderer::CreateFrameBuffers()
	{
		m_Framebuffers.resize(m_SwapChainImageViews.size());

		for (size_t i = 0; i < m_SwapChainImageViews.size(); i++)
		{
			std::array<VkImageView, 2> attachments = { m_SwapChainImageViews[i], m_DepthImageView };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = (unsigned int)attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = m_SwapChainExtent.width;
			framebufferInfo.height = m_SwapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_Framebuffers[i]) != VK_SUCCESS)
			{
				Log("Failed to create a framebuffer", LOG_CRITICAL);
				return false;
			}
		}

		return true;
	}

	void Renderer::CleanupFrameBuffer(VkDevice& device, VkFramebuffer& framebuffer)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);		
		framebuffer = VK_NULL_HANDLE;
	}
		
	bool Renderer::InitialiseGraphicsPipeline()
	{
		#pragma region SHADER SETUP
		//Get resource folder location.
		const std::string& resourceLocation = ServiceLocator::GetResourceFolderLocation();

		RebuildDefaultShaders();

		bool shaderLoadResult = false;

		shaderLoadResult = ShaderLoader::LoadShader(resourceLocation + "Required/DefaultVertex.spv", "DefaultVertex");
		if (shaderLoadResult != true)
		{
			Log("Failed to load default vertex shader for graphics pipeline.", LOG_SERIOUS);
			return false;
		}

		shaderLoadResult = ShaderLoader::LoadShader(resourceLocation + "Required/DefaultFragment.spv", "DefaultFragment");
		if (shaderLoadResult != true)
		{
			Log("Failed to load default fragment shader for graphics pipeline.", LOG_SERIOUS);
			return false;
		}	

		bool pipelineSuccessful = false;
		CustomPipelineDetails pipelineDetails{};
		pipelineSuccessful = CreateCustomPipeline(m_SolidPipeline, pipelineDetails);
		if (pipelineSuccessful != true)
		{
			Log("Failed to create solid graphics custom pipeline.", LOG_SERIOUS);
			return false;
		}

		pipelineDetails.Cullmode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
		pipelineDetails.PolygonMode = VkPolygonMode::VK_POLYGON_MODE_LINE;
		pipelineSuccessful = CreateCustomPipeline(m_WireframePipeline, pipelineDetails);
		if (pipelineSuccessful != true)
		{
			Log("Failed to create wireframe graphics custom pipeline.", LOG_SERIOUS);
			return false;
		}

		m_CurrentPipeline = &m_SolidPipeline;

		return true;
	}

	bool Renderer::CreateDescriptorLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;

		std::array<VkDescriptorSetLayoutBinding, DESCRIPTOR_COUNT> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = (uint32_t)bindings.size();
		layoutInfo.pBindings = bindings.data();
		layoutInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;
		layoutInfo.pNext = nullptr;

		if (vkCreateDescriptorSetLayout(m_Device, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			Log("Failed to create descriptor set layout!", LOG_SERIOUS);
			return false;
		}

		vkCmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)vkGetDeviceProcAddr(m_Device, "vkCmdPushDescriptorSetKHR");
		if (!vkCmdPushDescriptorSetKHR)
		{
			Log("Could not get a valid function pointer for vkCmdPushDescriptorSetKHR", LOG_SERIOUS);
			return false;
		}

		return true;
	}

	void Renderer::PushDescriptorSet(const Texture* texture) const
	{
		const Texture* renderTexture = texture;

		if (renderTexture == nullptr)
		{
			//Log("Passed texture pointer is not loaded, using debug texture instead.", LOG_SERIOUS);
			renderTexture = m_DebugTexture;
		}

		if (!renderTexture->Loaded())
		{
			//Log("Passed texture pointer is not loaded, using debug texture instead.", LOG_SERIOUS);
			renderTexture = m_DebugTexture;
		}

		std::array<VkWriteDescriptorSet, DESCRIPTOR_COUNT> descriptorWrites = {};

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_UniformBuffers[m_CurrentRenderFrame];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);
		
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = renderTexture->GetImageView();
		imageInfo.sampler = m_TextureSampler;

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		assert(m_CurrentPipeline != nullptr);
		vkCmdPushDescriptorSetKHR(m_CommandBuffers[m_CurrentRenderFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_CurrentPipeline->m_Layout, 0, descriptorWrites.size(), descriptorWrites.data());
	}

	bool Renderer::CreateSyncObjects()
	{
		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		//Create the fence with the signal bit on so that it doesnt hang on the first frame.
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS)
			{
				Log("Failed to create image availability semaphore.", LOG_CRITICAL);
				return false;
			}

			if (vkCreateSemaphore(m_Device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS)
			{
				Log("Failed to create render finished semaphore.", LOG_CRITICAL);
				return false;
			}

			if (vkCreateFence(m_Device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
			{
				Log("Failed to create fence.", LOG_CRITICAL);
				return false;
			}
		}

		return true;
	}

	void Renderer::CleanupSyncObjects()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			CleanupSemaphore(m_Device, m_ImageAvailableSemaphores[i]);
			CleanupSemaphore(m_Device, m_RenderFinishedSemaphores[i]);
			CleanupFence(m_Device, m_InFlightFences[i]);
		}
		m_ImageAvailableSemaphores.clear();
		m_RenderFinishedSemaphores.clear();
		m_InFlightFences.clear();
	}

	bool Renderer::CreateRenderPass()
	{	
		/*
		- VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
		- VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: Images to be presented in the swapchain
		- VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: Images to be used as destination for a memory copy operation
		*/

		//Create a single color buffer using one of the images of swap chain.
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapChainFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		//Currently setting to this as  we are not using multisampling yet.
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.colorAttachmentCount = 1;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = (uint32_t)attachments.size();
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			Log("Failed to create render pass.", LOG_CRITICAL);
			return false;
		}

		return true;
	}

	void Renderer::CleanupRenderPass(VkDevice& device, VkRenderPass& renderpass)
	{
		vkDestroyRenderPass(device, renderpass, nullptr);
		renderpass = VK_NULL_HANDLE;
	}

	void Renderer::CleanupDescriptorPool(VkDevice& device, VkDescriptorPool& descriptorPool)
	{
		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
		descriptorPool = VK_NULL_HANDLE;
	}

	void Renderer::CleanupDescriptorLayout(VkDevice& device, VkDescriptorSetLayout& layout)
	{
		vkDestroyDescriptorSetLayout(device, layout, nullptr);
		layout = VK_NULL_HANDLE;
	}

	void Renderer::DeallocateDescriptorSets(VkDevice& device, VkDescriptorPool& descriptorPool, std::vector<VkDescriptorSet>& descriptorSets)
	{
		if (descriptorPool != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(device, descriptorPool, descriptorSets.size(), descriptorSets.data());
		}
	}

	void Renderer::Shutdown()
	{
		if (m_IsInitialised)
		{
			WaitForRendererIdle();

			Texture::Destroy(*m_DebugTexture);

			ShaderLoader::DumpAll();

			CleanupSampler(m_Device, m_TextureSampler);

			CleanupUniformBuffers();

			DeallocateDescriptorSets(m_Device, m_DescriptorPool, m_DescriptorSets);
			m_DescriptorSets.clear();
			CleanupDescriptorPool(m_Device, m_DescriptorPool);
			CleanupDescriptorLayout(m_Device, m_DescriptorSetLayout);

			CleanupSyncObjects();

			CleanupCommandPoolAndBuffers(m_Device, m_CommandPool, m_CommandBuffers);
			
			CleanupSwapChain();

			DestroyWindowSurface();

			m_CurrentPipeline = nullptr;
			DestroyCustomPipeline(m_SolidPipeline);
			DestroyCustomPipeline(m_WireframePipeline);

			DestroyDebugMessenger();

			vkDestroyDevice(m_Device, nullptr);
			DestroyVulkanInstance();

			PushConstants.ModelMatrix = glm::mat4(1.0f);

			m_CurrentImageIndex = NULL;
			m_AttachedWindow = nullptr;
			m_IsInitialised = false;
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL Renderer::VulkanDebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData)
	{
		LOG_STATUS status = LOG_SAFE;

		switch (messageSeverity)
		{
			//Diagnostic Message
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			status = LOG_SERIOUS;
			break;

			//Informational Message like the creation of a resource.
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:  
			status = LOG_SAFE;
			break;

			//Message about behaviour that is not an error but probably a bug.
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			status = LOG_MINIMAL;
			break;

			//Message from shader debug printf.
		case VK_DEBUG_REPORT_ERROR_BIT_EXT:
			status = LOG_SERIOUS;
			break;

			//Message about a invalid behaviour that may cause crashes
		default:
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			status = LOG_CRITICAL;
			break;
		}

		std::string logMessage = "";

		switch (messageType)
		{
		// Some event has happened that is unrelated to the specification or performance
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: 
			logMessage = "General : ";
			break;

		// Something has happened that violates the specification or indicates a possible mistake
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			logMessage = "Possible Mistake or Spec Break : ";
			break;

		//Potential non-optimal use of Vulkan
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT :
			logMessage = "Non-Optimal : ";
			break;

		default:
			logMessage = "Unsure how you reached here : ";
			break;
		}

		logMessage += pCallbackData->pMessage;

		Log(logMessage, status);

		return VK_FALSE;
	}

	bool Renderer::CreateVulkanInstance()
	{
		//Technically optional but may provide
		//useful information to the driver to optimise
		//our specific application.
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pEngineName = "Bennett";
		appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 0, 0);
		appInfo.pApplicationName = "RTS Game";
		appInfo.applicationVersion = appInfo.engineVersion;


		/*
		* Like directX, alot of information is passed using structs rather than function params.
		* Next struct is not optional as it tells the vulkan driver about global extensions and validation layers
		* that we want to use. Global in this context means they apply to the entire program rather than
		* a specific device.
		*/
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		/*
		* The next two parameters specify the desired global extensions.
		* As vulkan is platform agnostic, we need to use an extension to interface
		* with the window system. We use glfw's built in funcitons to find these.
		*/
		std::vector<const char*> requiredExtensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = (uint32_t)requiredExtensions.size();
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
		
		/*
		* The last two struct members determine the global validation layers to enable.
		*/
		if (m_EnableValidationLayers && !CheckValidationLayerSupport())
		{
			Log("Validation layers are enabled but failed to support them.", LOG_SERIOUS);
			createInfo.enabledLayerCount = 0;
		}
		
		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = (uint32_t)m_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		//We can now create the instance.
		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		if (result != VK_SUCCESS)
		{
			Log("Failed to create vulkan instance.", LOG_CRITICAL);
			return false;
		}
		
		return true;
	}

	void Renderer::DestroyVulkanInstance()
	{
		Log("Destroying vulkan instance", LOG_MINIMAL);
		vkDestroyInstance(m_Instance, nullptr);
		m_Instance = VK_NULL_HANDLE;
	}

	bool Renderer::CheckValidationLayerSupport()
	{
		/*
		* Check if all the requested layers are available by first listing all of the
		* available layers using vkEnumerateInstanceLayerProperties and store in the vector.
		*/
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		/*
		* Next, check all the layers that are needed are
		* also layers in this vector.
		*/
		for (const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;
			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					std::string str = std::string("Found appropriate validation layer: ");
					str += layerName;
					Log(str, LOG_SAFE);
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}

		return true;

	}

	bool Renderer::CreateImage(VkImage& image, VkDeviceMemory& imageMemory, const VkImageCreateInfo& createInfo)
	{
		if (vkCreateImage(m_Device, &createInfo, nullptr, &image) != VK_SUCCESS)
		{
			Log("Failed to create image", LOG_SERIOUS);
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_Device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
		{
			Log("Failed to allocate image memory", LOG_SERIOUS);
			return false;
		}

		vkBindImageMemory(m_Device, image, imageMemory, 0);

		return true;
	}

	bool Renderer::CreateImageView(VkImageView& imageView, const VkImageViewCreateInfo& createInfo)
	{
		if (vkCreateImageView(m_Device, &createInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			Log("Failed to create image view", LOG_SERIOUS);
			return false;
		}

		return true;
	}
	
	void Renderer::CleanupImage(VkDevice& device, VkImage& image)
	{
		vkDestroyImage(device, image, nullptr);
		image = VK_NULL_HANDLE;
	}

	void Renderer::CleanupImageView(VkDevice& device, VkImageView& image)
	{
		vkDestroyImageView(device, image, nullptr);
		image = VK_NULL_HANDLE;
	}

	bool Renderer::CreateDepthResources()
	{
		VkFormat depthFormat = FindDepthFormat();

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_SwapChainExtent.width;
		imageInfo.extent.height = m_SwapChainExtent.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = depthFormat;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0;

		if(CreateImage(m_DepthImage, m_DepthImageMemory, imageInfo) == false)
			return false;

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_DepthImage;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = depthFormat;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (CreateImageView(m_DepthImageView, viewInfo) == false)
			return false;

		return true;
	}

	void Renderer::CleanupDepthResources()
	{
		CleanupImage(m_Device, m_DepthImage);
		CleanupImageView(m_Device, m_DepthImageView);
		FreeDeviceMemory(m_Device, m_DepthImageMemory);
	}

	VkFormat Renderer::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates) 
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		throw std::runtime_error("failed to find supported format!");
	}

	VkFormat Renderer::FindDepthFormat()
	{
		return FindSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	bool Renderer::HasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;;
	}

	std::vector<const char*> Renderer::GetRequiredExtensions() const
	{	
		std::vector<const char*> extensions = std::vector<const char*>();
		//Required for creating a surface in win32.
		extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

		if (m_EnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool Renderer::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		//You can also use a nested loop like in 'CheckValidationLayerSupport()'
		std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());
		for (const auto& extensions : availableExtensions)
		{
			requiredExtensions.erase(extensions.extensionName);
		}

		return requiredExtensions.empty();
	}

	void Renderer::CreateDebugMessenger()
	{
		if (!m_EnableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
				
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

#ifdef _DEBUG
		createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
#endif

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = VulkanDebugCallback;
		createInfo.pUserData = nullptr; // Optional

		if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
		{
			Log("Failed to create the debug messenger.", LOG_SERIOUS);
		}
	}

	void Renderer::DestroyDebugMessenger()
	{
		if (m_EnableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
			m_DebugMessenger = VK_NULL_HANDLE;
		}
	}

	bool Renderer::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

		if (deviceCount == 0)
		{
			Log("Failed to find any physical devices.", LOG_SERIOUS);
			return false;
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());

		for (auto& device : devices)
		{
			if (IsPhysicalDeviceSuitable(device))
			{
				m_PhysicalDevice = device;
				break;
			}
		}
		
		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			Log("Failed to find any physical devices.", LOG_SERIOUS);
			return false;
		}

		VkPhysicalDeviceProperties selectedDeviceProperties;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &selectedDeviceProperties);
		std::string str = "Selected device name: ";
		str += selectedDeviceProperties.deviceName;
		Log(str, LOG_SAFE);

		return true;
	}

	/*
	* There are more details that can be queried from a device e.g. memory, queue families etc..
	* This function only polls for a dedicated GPU  that also supports geometry shaders.
	*/
	bool Renderer::IsPhysicalDeviceSuitable(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;
		FindQueueFamilies(device, indices);
		bool extensionsSupported = CheckDeviceExtensionSupport(device);
		bool isSwapChainAdequate = false;
		if(extensionsSupported)
		{
			auto swapChainSupportDetails =
				QuerySwapChainSupport(device);

			isSwapChainAdequate = 
				!swapChainSupportDetails.Formats.empty() &&
				!swapChainSupportDetails.PresentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		bool result = indices.IsComplete() && extensionsSupported && isSwapChainAdequate && supportedFeatures.samplerAnisotropy;
		return result;
	}

	void Renderer::FindQueueFamilies(VkPhysicalDevice device, QueueFamilyIndices& indices)
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.GraphicsFamily = i;
			}

			if (indices.IsComplete())
				break;

			i++;
		}
	}

	bool Renderer::CreateLogicalDevice()
	{
		QueueFamilyIndices indices;
		FindQueueFamilies(m_PhysicalDevice, indices);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.queueFamilyIndex = indices.GraphicsFamily.value();

		//You can create a small number of queues fpr each queue family but I wont needmore than one.
		//You can create all of the command buffers on different thread and submit at once on the main thread
		//in a single low overhead call.Because of this, vulkan lets you set priorities for each queue using a floating point number between 0.0 and 1.0
		//Thsi is still needed if theres a single queue.
		float priority = 1.0f;
		queueCreateInfo.pQueuePriorities = &priority;

		//Specifying used device features e.g. geometry shaders

		VkPhysicalDeviceUniformBufferStandardLayoutFeatures ubopackingalignment_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_UNIFORM_BUFFER_STANDARD_LAYOUT_FEATURES, nullptr };
		VkPhysicalDeviceDescriptorIndexingFeatures indexing_features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT, &ubopackingalignment_features };
		VkPhysicalDeviceFeatures2 bindlessFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, &indexing_features };
		vkGetPhysicalDeviceFeatures2(m_PhysicalDevice, &bindlessFeatures);
		bindlessFeatures.features.samplerAnisotropy = VK_TRUE;
		bool isBindlessSupported = indexing_features.descriptorBindingPartiallyBound && indexing_features.runtimeDescriptorArray && ubopackingalignment_features.uniformBufferStandardLayout;

		if (isBindlessSupported == false)
		{
			Log("Bindless texturing not supported.", LOG_SERIOUS);
			return false;
		}

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = nullptr;

		//Enabling swapchain as a device extension/
		createInfo.enabledExtensionCount = (uint32_t)m_DeviceExtensions.size();
		createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

		indexing_features.descriptorBindingPartiallyBound = VK_TRUE;
		indexing_features.runtimeDescriptorArray = VK_TRUE;
		createInfo.pNext = &bindlessFeatures;

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = (uint32_t)m_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
		{
			Log("Failed to create logical device.", LOG_CRITICAL);
			return false;
		}

		Log("Created Vulkan logical device.", LOG_SAFE);
		GetGraphicsQueue(indices);

		return true;
	}

	void Renderer::GetGraphicsQueue(const QueueFamilyIndices& indices)
	{
		vkGetDeviceQueue(m_Device, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
		Log("Got graphics queue.", LOG_SAFE);
	}

	bool Renderer::CreateWindowSurface(const HWND& hWnd, const HINSTANCE& hInstance)
	{
		VkWin32SurfaceCreateInfoKHR info{};
		info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		info.hwnd = hWnd;
		info.hinstance = hInstance;
		
		if (vkCreateWin32SurfaceKHR(m_Instance, &info, nullptr, &m_Surface) != VK_SUCCESS)
		{
			Log("Failed to create window surface", LOG_CRITICAL);
			return false;
		}
		else
		{
			Log("Created window surface successfully.", LOG_SAFE);
			return true;
		}
	}

	void Renderer::DestroyWindowSurface()
	{
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		m_Surface = VK_NULL_HANDLE;
	}
	
	Renderer::SwapChainSupportDetails Renderer::QuerySwapChainSupport(VkPhysicalDevice device)
	{
		SwapChainSupportDetails details{};

		//Getting basic surface capabilities.
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.Capabilities);

		//Locating and storing all available formats.
		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			details.Formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.Formats.data());
		}

		//Locating and storing all available present modes.
		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			details.PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.PresentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR Renderer::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
	{
		for (const auto& format : formats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&	
				format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				Log("Found format using B8G8R8A8_SRGB AND COLOR_SPACE_SRGB.", LOG_SAFE);
				return format;
			}

		}

		Log("Failed to find format using B8G8R8A8_SRGB AND COLOR_SPACE_SRGB, returning first available format.", LOG_MINIMAL);
		return formats[0];
	}

	/*
	VK_PRESENT_MODE_IMMEDIATE_KHR: 
		Images submitted by your application are transferred to the screen right 
		away, which may result in tearing.

	VK_PRESENT_MODE_FIFO_KHR: 
		The swap chain is a queue where the display
		takes an image from the front of the queue when the display is refreshed
		and the program inserts rendered images at the back of the queue. If the
		queue is full then the program has to wait. This is most similar to vertical
		sync as found in modern games. The moment that the display is refreshed
		is known as �vertical blank�.

	VK_PRESENT_MODE_FIFO_RELAXED_KHR: 
		This mode only differs from the previous one if the application is late and
		the queue was empty at the last vertical blank. Instead of waiting for the 
		next vertical blank, the image is transferred right away when it finally 
		arrives. This may result in visible tearing.

	VK_PRESENT_MODE_MAILBOX_KHR: 
		This is another variation of the second mode. Instead of blocking the 
		application when the queue is full, the	images that are already queued 
		are simply replaced with the newer ones. This mode can be used to render 
		frames as fast as possible while still avoiding tearing, resulting in
		fewer latency issues than standard vertical	sync. This is commonly known
		as �triple buffering�, although the existence of three buffers alone does 
		not necessarily mean that the framerate	is unlocked.
	*/
	VkPresentModeKHR Renderer::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
	{
		for (const auto& mode : presentModes)
		{
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
				return mode;
		}

		return VK_PRESENT_MODE_IMMEDIATE_KHR;
	}

	VkExtent2D Renderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		//This is a check to see if the window manager allows us to differ the resolution
		//of the window between pixels and screenspace.
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			//if vulkan does not fix the extent, we can just use glfw/win32 to query the frame buffer
			//and create an image extent matched against it.
			glm::vec2 size = m_AttachedWindow->GetSize();
			VkExtent2D actualExtent = { static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y) };
			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
			return actualExtent;
		}
	}

	bool Renderer::CreateSwapChain()
	{
		SwapChainSupportDetails details = QuerySwapChainSupport(m_PhysicalDevice);
		VkSurfaceFormatKHR format = ChooseSwapChainSurfaceFormat(details.Formats);
		VkPresentModeKHR presentMode = ChooseSwapChainPresentMode(details.PresentModes);
		VkExtent2D extent = ChooseSwapExtent(details.Capabilities);

		//specifying how many images wanted in the swap chain.
		//there is a +1 to avoid waiting for the driver to aquire another image
		uint32_t imageCount = details.Capabilities.minImageCount + 1;

		//Quick check to max sure the +1 hasnt gone over the maximum allowed images.
		if (details.Capabilities.maxImageCount > 0 &&
			imageCount > details.Capabilities.maxImageCount)
		{
			imageCount = details.Capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = format.format;
		createInfo.imageColorSpace = format.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		//todo : figure out where the hell indices.presentFamily came from.
		//if you want to use multiple frame buffersor one from memory
		//for something like post processing then you need
		//to specify that in the swapchain.
		QueueFamilyIndices indices;
		FindQueueFamilies(m_PhysicalDevice, indices);
		//uint32_t familyIndicesList[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };
		//
		//if (indices.GraphicsFamily != indices.PresentFamily)
		{
			//Images can be used across multiple queue
			//families without explicit ownership transfers.
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			//createInfo.pQueueFamilyIndices = familyIndicesList;
		}
		//else
		{
			//An image is owned by one queue family at a time and ownership must be
			//explicitly transferred before using it in another queue family.
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}

		/*
		* Specify that a certain transform should be applied to the swapchain if supported.
		* Just want to apply whatever is currently being applied.
		*/
		createInfo.preTransform = details.Capabilities.currentTransform; 
		/*
		* Specifies the alpha channel should be used for blending with other windows.
		* Almost always want to ignore this.
		*/
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		/*
		* If clipped is true then we do not care about the color of pixels obscured.
		* May need to read them in future but for now it doesnt matter.
		*/
		createInfo.presentMode = presentMode;
		createInfo.clipped = true;

		/*
		* This will be used in the future for window resizing but for now assuming theres one window adn swapchain.
		*/
		createInfo.oldSwapchain = m_SwapChain;

		if (vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
		{
			Log("Failed to create the swapchain.", LOG_CRITICAL);
			return false;
		}

		Log("Successfully created the swapchain.", LOG_SAFE);

		//Store variables
		m_SwapChainExtent = extent;
		m_SwapChainFormat = format.format;

		//Get all the images
		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);
		m_SwapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data());

		return true;
	}

	bool Renderer::CreateSwapChainImageViews()
	{
		if (m_SwapChainImages.size() <= 0)
		{
			Log("Tried to create swap chain image views with no swap chain images.", LOG_CRITICAL);
			return false;
		}

		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo{}; 
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_SwapChainImages[i];
			//Specifies how data should be interpreted
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_SwapChainFormat;
			//Allows you to swizzle the channels around
			//Map all channels to red, map constants etc. etc.
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			//SubresourceRange describes the image purpose and what part should be accessed.
			//Currently they are only used as colour targets with no mipmap levels or layers.
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.layerCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.levelCount = 1;

			if (vkCreateImageView(m_Device, &createInfo, nullptr, &m_SwapChainImageViews[i]) != VK_SUCCESS)
			{
				Log("Failed to create an image view from swapchain image.", LOG_CRITICAL);
			}
		}

		return true;
	}

	bool Renderer::CreateCustomPipeline(CustomPipeline& pipeline, const CustomPipelineDetails& details)
	{
#pragma region SHADER SETUP
		Shader* vertexShader = ShaderLoader::GetShaderByName(details.VertexShaderName);
		if (vertexShader == nullptr)
		{
			Log("Failed to find vertex shader for custom pipeline.\n", LOG_MINIMAL);
			return false;
		}

		Shader* pixelShader = ShaderLoader::GetShaderByName(details.PixelShaderName);
		if (pixelShader == nullptr)
		{
			Log("Failed to find pixel shader for custom pipeline.\n", LOG_MINIMAL);
			return false;
		}

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
		vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageInfo.module = vertexShader->GetVulkanModule();
		vertexShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
		fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStageInfo.module = pixelShader->GetVulkanModule();
		fragmentShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };
#pragma endregion

#pragma region INPUT ASSEMBLY
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = details.Topology;
		inputAssembly.primitiveRestartEnable = VK_FALSE;
#pragma endregion

#pragma region VERTEX INPUT ASSEMBLY
		VkVertexInputBindingDescription vertexBindings = details.InputBindingDescription;
		std::vector<VkVertexInputAttributeDescription> vertexAttributes = details.InputAttributeDescription;
				 
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes.data();
		vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)vertexAttributes.size();
		vertexInputInfo.pVertexBindingDescriptions = &vertexBindings;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
#pragma endregion

#pragma region RASTERIZER
		//Setting up the actual rasterizer.
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		//If rasterizerDisacardEnable is set to true, the rasterizer stage is never ran.
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		//If depthClampEnable is set to true, fragments that are beyond the near and far planes
		//are clamped to them as opposed to discarding them. This is useful for shadow maps 
		//and requires and enabling a GPU feature.
		rasterizer.depthClampEnable = VK_FALSE;

		/*
		* VK_POLYGON_MODE_FILL - Fill the area of the polygon with fragments.
		* VK_POLYGON_MODE_LINE - Polygon edges are drawn as lines.
		* VK_POLYGON_MODE_POINT - Polygon vertices are drawn as points.
		*
		* Using any other mode than fill requires enabling a GPU feature.
		*/
		rasterizer.polygonMode = details.PolygonMode;

		//Line thickness. Anything other than 1.0f requires enabling a GPU feature.
		rasterizer.lineWidth = details.LineWidth;

		rasterizer.cullMode = details.Cullmode;
		rasterizer.frontFace = details.FrontFace;

		//Altering depth values and biasing.
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;
#pragma endregion

#pragma region DYNAMIC STATES
		std::vector<VkDynamicState> dynamicStates =
		{
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		//When opting for dynamic viewports, you need to enable resepectic states for the pipeline.
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		m_Viewport.width = (float)m_SwapChainExtent.width;
		m_Viewport.height = (float)m_SwapChainExtent.height;
		m_Viewport.minDepth = 0.0f;
		m_Viewport.maxDepth = 1.0f;
		m_Viewport.x = 0.0f;
		m_Viewport.y = 0.0f;

		m_ScissorRect.offset = { 0, 0 };
		m_ScissorRect.extent = m_SwapChainExtent;

		//Specify their count at pipeline creation time.
		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.pScissors = &m_ScissorRect;
		viewportState.scissorCount = 1;
		viewportState.pViewports = &m_Viewport;
		viewportState.viewportCount = 1;
#pragma endregion

#pragma region Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f;
		multisampling.pSampleMask = nullptr;
		multisampling.alphaToCoverageEnable = VK_FALSE;
		multisampling.alphaToOneEnable = VK_FALSE;
		//Will be revisited later so leave disabled for now.
#pragma endregion

#pragma region Depth/Stencil
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.depthTestEnable = VK_TRUE;
		depthStencilInfo.depthWriteEnable = VK_TRUE;
		depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilInfo.maxDepthBounds = 1.0f;
		depthStencilInfo.minDepthBounds = 0.0f;
		depthStencilInfo.stencilTestEnable = VK_FALSE;
		depthStencilInfo.front = {};
		depthStencilInfo.back = {};
#pragma endregion

#pragma region Colour Blending
		VkPipelineColorBlendAttachmentState colorBlend{};
		colorBlend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlend.blendEnable = VK_TRUE;
		colorBlend.alphaBlendOp = VK_BLEND_OP_ADD;
		colorBlend.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlend.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlend.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlendingInfo{};
		colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendingInfo.attachmentCount = 1;
		colorBlendingInfo.pAttachments = &colorBlend;
		colorBlendingInfo.logicOpEnable = VK_FALSE;
		colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendingInfo.blendConstants[0] = 0.0f;
		colorBlendingInfo.blendConstants[1] = 0.0f;
		colorBlendingInfo.blendConstants[2] = 0.0f;
		colorBlendingInfo.blendConstants[3] = 0.0f;
#pragma endregion

#pragma region Creating Pipeline

		VkPushConstantRange pushConstantInfo{};
		pushConstantInfo.offset = 0;
		pushConstantInfo.size = sizeof(PushConstantBuffer);
		pushConstantInfo.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT;

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
		pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantInfo;

		if (vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &pipeline.m_Layout) != VK_SUCCESS)
		{
			Log("Failed to create pipeline layout.", LOG_CRITICAL);
			return false;
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencilInfo;
		pipelineInfo.pColorBlendState = &colorBlendingInfo;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.layout = pipeline.m_Layout;
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline.m_Pipeline) != VK_SUCCESS)
		{
			Log("Failed to create a custom graphics pipeline object.", LOG_CRITICAL);
			return false;
		}

#pragma endregion
		return true;
	}

	void Renderer::DestroyCustomPipeline(CustomPipeline& pipeline)
	{
		Renderer& renderer = ServiceLocator::GetRenderer();
		vkDestroyPipeline(renderer.GetDevice(), pipeline.m_Pipeline, nullptr);
		pipeline.m_Pipeline = VK_NULL_HANDLE;
		vkDestroyPipelineLayout(renderer.GetDevice(), pipeline.m_Layout, nullptr);
		pipeline.m_Layout = VK_NULL_HANDLE;
	}


	#pragma region No longer used.

	/*

	void Renderer::BindDescriptorSet() const
	{
		Log("Binding descriptor sets is no longer used. You should be using PushDescriptorSets instead.", LOG_SERIOUS);
		return;

		assert(m_CurrentPipeline != nullptr);
		vkCmdBindDescriptorSets(m_CommandBuffers[m_CurrentRenderFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_CurrentPipeline->m_Layout, 0, 1, &m_DescriptorSets[m_CurrentRenderFrame], 0, nullptr);
	}

	bool Renderer::CreateDescriptorPool()
	{
		Log("Allocating descriptor pools is no longer used. You should be using PushDescriptorSets instead.", LOG_SERIOUS);
		return false;

		std::array<VkDescriptorPoolSize, DESCRIPTOR_COUNT> poolSize{};
		poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize[0].descriptorCount = MAX_FRAMES_IN_FLIGHT;
		poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize[1].descriptorCount = MAX_FRAMES_IN_FLIGHT;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT_EXT | VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT | VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT;
		poolInfo.poolSizeCount = (uint32_t)poolSize.size();
		poolInfo.pPoolSizes = poolSize.data();
		poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

		if (vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			Log("Failed to create descriptor pool", LOG_SERIOUS);
			return false;
		}

		return true;
	}

	bool Renderer::AllocateDescriptorSets()
	{
		Log("Allocating descriptor sets is no longer used. You should be using PushDescriptorSets instead.", LOG_SERIOUS);
		return false;

		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
		allocInfo.pSetLayouts = layouts.data();
		allocInfo.pNext = nullptr;

		m_DescriptorSets.clear();
		m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);

		if (vkAllocateDescriptorSets(m_Device, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS)
		{
			Log("failed to create descriptor sets!", LOG_CRITICAL);
			return false;
		}

		return true;
	}

	bool Renderer::UpdateDescriptorSets(const Texture* texture) const
	{
		Log("UpdateDescriptorSets is no longer used. You should be using PushDescriptorSets.", LOG_SERIOUS);
		return false;

		const Texture* renderTexture = texture;

		if (!texture->Loaded())
		{
			Log("Passed texture pointer is not loaded, using debug texture instead.", LOG_SERIOUS);
			renderTexture = m_DebugTexture;
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_UniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = renderTexture->GetImageView();
			imageInfo.sampler = m_TextureSampler;

			std::array<VkWriteDescriptorSet, DESCRIPTOR_COUNT> descriptorWrites = {};
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = m_DescriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = m_DescriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(m_Device, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
		}

		return true;
	}
	*/

#pragma endregion

}

