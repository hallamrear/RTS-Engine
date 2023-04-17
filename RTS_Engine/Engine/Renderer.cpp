#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "BennettPCH.h"
#include "Renderer.h"
#include "Vertex.h"
#include <fstream>
#include <set>
#include "Buffer.h"

namespace Bennett
{
	UniformBufferObject Renderer::UniformMatrixBuffer = UniformBufferObject();

	Renderer::Renderer()
	{
		m_PhysicalDevice = VK_NULL_HANDLE;
		m_IsInitialised = false;
		m_Instance = nullptr;
	}

	Renderer::~Renderer()
	{
		if (m_IsInitialised)
		{
			Shutdown();
		}
	}

	bool Renderer::Initialise(GLFWwindow& window)
	{
		Shutdown();

		m_IsInitialised = InitialiseCoreVulkanSystem(window) && InitialiseGraphicsPipeline();
		return m_IsInitialised;
	}

	bool Renderer::InitialiseCoreVulkanSystem(GLFWwindow& window)
	{
		bool result = CreateVulkanInstance();
		CreateDebugMessenger();
		result &= CreateWindowSurface(window);
		result &= PickPhysicalDevice();
		result &= CreateLogicalDevice();
		result &= CreateSwapChain(window);
		result &= CreateSwapChainImageViews();
		result &= CreateRenderPass();
		result &= CreateDescriptorLayout();
		result &= CreateDescriptorPool();
		result &= InitialiseGraphicsPipeline();
		result &= CreateUniformBuffers();
		result &= CreateDescriptorSets();
		result &= CreateFrameBuffers();
		result &= CreateCommandPool(); 
		result &= CreateCommandBuffer();
		result &= CreateSyncObjects();
		return result;
	}

	bool Renderer::CreateCommandBuffer()
	{
		m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.commandBufferCount = m_CommandBuffers.size();
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

		vkCmdBindDescriptorSets(m_CommandBuffers[m_CurrentRenderFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[m_CurrentRenderFrame], 0, nullptr);

		VkClearValue colour = { 0.0f, 0.0f, 0.0f, 1.0f };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPass;
		renderPassInfo.framebuffer = m_Framebuffers[m_CurrentImageIndex];
		renderPassInfo.renderArea.offset = { 0 ,0 };
		renderPassInfo.renderArea.extent = m_SwapChainExtent;
		renderPassInfo.pClearValues = &colour;

		vkCmdSetViewport(m_CommandBuffers[m_CurrentRenderFrame], 0, 1, &m_Viewport);
		vkCmdSetScissor(m_CommandBuffers[m_CurrentRenderFrame], 0, 1, &m_ScissorRect);

		vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentRenderFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::StartFrame()
	{
		WaitForFrame();
		AquireSwapchainImageIndex();
		//Reset
		vkResetCommandBuffer(m_CommandBuffers[m_CurrentRenderFrame], 0);
		//Record
		RecordCommandBuffer();
		vkCmdBindPipeline(m_CommandBuffers[m_CurrentRenderFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
	}

	void Renderer::EndFrame()
	{
		vkCmdEndRenderPass(m_CommandBuffers[m_CurrentRenderFrame]);

		if (vkEndCommandBuffer(m_CommandBuffers[m_CurrentRenderFrame]) != VK_SUCCESS)
		{
			Log("Failed to record command buffer.", LOG_CRITICAL);
			return;
		}

		SubmitCommandData();
		Present(m_CurrentImageIndex);
		m_CurrentRenderFrame = (m_CurrentRenderFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::AquireSwapchainImageIndex()
	{
		vkAcquireNextImageKHR(m_Device, m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentRenderFrame], VK_NULL_HANDLE, &m_CurrentImageIndex);
	}

	void Renderer::WaitForFrame()
	{
		vkWaitForFences(m_Device, 1, &m_InFlightFences[m_CurrentRenderFrame], VK_TRUE, UINT64_MAX);
		vkResetFences(m_Device, 1, &m_InFlightFences[m_CurrentRenderFrame]);
	}

	void Renderer::UpdateUniformBuffer() const
	{
		memcpy(m_UniformBuffersMapped[m_CurrentRenderFrame], &UniformMatrixBuffer, sizeof(UniformMatrixBuffer));
	}

	void Renderer::SubmitCommandData()
	{
		UpdateUniformBuffer();

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

		vkQueuePresentKHR(m_GraphicsQueue, &presentInfo);
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
			allocInfo.memoryTypeIndex = Bennett::Buffer::FindMemoryType(*this, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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
		m_Viewport.height = h;
		m_Viewport.width = w;
		m_Viewport.x = x;
		m_Viewport.y = y;
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
			VkImageView attachments[] = { m_SwapChainImageViews[i] };

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
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

	std::vector<char> Renderer::ReadShaderFile(const std::string& fileName)
	{
		std::vector<char> readBytes = std::vector<char>();
		std::ifstream file(fileName, std::ios::binary | std::ios::ate);

		if (!file.is_open())
		{
			return readBytes;
		}

		size_t fileSize = (size_t)file.tellg();
		readBytes.resize(fileSize);
		file.seekg(0);
		file.read(readBytes.data(), fileSize);
		file.close();
		return readBytes;
	}

	VkShaderModule Renderer::CreateShaderModule(const std::string& fileName)
	{
		auto shader = ReadShaderFile(fileName.data());

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shader.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shader.data());

		VkShaderModule module;
		if (vkCreateShaderModule(m_Device, &createInfo, nullptr, &module) != VK_SUCCESS)
		{
			Log("Failed to create shader module.", LOG_SERIOUS);
		}
		return module;
	}

	bool Renderer::InitialiseGraphicsPipeline()
	{
#pragma region SHADER SETUP
		m_FragShaderModule = CreateShaderModule("FragmentShader.spv");
		m_VertShaderModule = CreateShaderModule("VertexShader.spv");

		VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
		vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderStageInfo.module = m_VertShaderModule;
		vertexShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
		fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderStageInfo.module = m_FragShaderModule;
		fragmentShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };
#pragma endregion

#pragma region INPUT ASSEMBLY

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

#pragma endregion

#pragma region VERTEX INPUT ASSEMBLY

		auto vertexAttributes = Vertex::GetAttributeDescription();
		auto vertexBindings = Vertex::GetBindingDescription();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes.data();
		vertexInputInfo.vertexAttributeDescriptionCount = vertexAttributes.size();
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
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

		//Line thickness. Anything other than 1.0f requires enabling a GPU feature.
		rasterizer.lineWidth = 1.0f;

		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

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
		dynamicState.dynamicStateCount = dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		m_Viewport.width = m_SwapChainExtent.width;
		m_Viewport.height = m_SwapChainExtent.height;
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
		VkPipelineDepthStencilStateCreateInfo stencilinfo{};
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

		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutCreateInfo.setLayoutCount = 1;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
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
		pipelineInfo.pDepthStencilState = &stencilinfo;
		pipelineInfo.pColorBlendState = &colorBlendingInfo;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;


		if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
		{
			Log("Failed to create the graphics pipeline object.", LOG_CRITICAL);
			return false;
		}

#pragma endregion

		return true;
	}

	bool Renderer::CreateDescriptorLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding{};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(m_Device, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			Log("Failed to create descriptor set layout!", LOG_SERIOUS);
			return false;
		}


		return true;
	}

	bool Renderer::CreateDescriptorPool()
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = MAX_FRAMES_IN_FLIGHT;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

		if (vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			Log("Failed to create descriptor pool", LOG_SERIOUS);
			return false;
		}

		return true;
	}

	bool Renderer::CreateDescriptorSets()
	{
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = MAX_FRAMES_IN_FLIGHT;
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(m_Device, &allocInfo, m_DescriptorSets.data()) != VK_SUCCESS)
		{
			Log("failed to create descriptor sets!", LOG_CRITICAL);
			return false;
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_UniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr;
			descriptorWrite.pTexelBufferView = nullptr;
			vkUpdateDescriptorSets(m_Device, 1, &descriptorWrite, 0, nullptr);
		}


		return true;
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

	bool Renderer::CreateRenderPass()
	{
		//Create a single color buffer using one of the images of swap chain.
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_SwapChainFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		//Currently setting to this as  we are not using multisampling yet.
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		/*
		- VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: Images used as color attachment
		- VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: Images to be presented in the swapchain
		- VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: Images to be used as destination for a memory copy operation
		*/
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.colorAttachmentCount = 1;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
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

	void Renderer::Shutdown()
	{
		if (m_IsInitialised)
		{
			vkDeviceWaitIdle(m_Device);

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				vkDestroyBuffer(m_Device, m_UniformBuffers[i], nullptr);
				vkFreeMemory(m_Device, m_UniformBuffersMemory[i], nullptr);
			}

			vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
			vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);

			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
				vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[i], nullptr);
				vkDestroyFence(m_Device, m_InFlightFences[i], nullptr);
			}

			vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

			for (auto framebuffer : m_Framebuffers)
			{
				vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
			}

			vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
			vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
			vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);

			vkDestroyShaderModule(m_Device, m_FragShaderModule, nullptr);
			vkDestroyShaderModule(m_Device, m_VertShaderModule, nullptr);

			for (auto imageView : m_SwapChainImageViews)
			{
				vkDestroyImageView(m_Device, imageView, nullptr);
			}

			vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);
			DestroyWindowSurface();
			vkDestroyDevice(m_Device, nullptr);
			DestroyVulkanInstance();
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
		createInfo.enabledExtensionCount = requiredExtensions.size();
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
			createInfo.enabledLayerCount = m_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		//We can now create the instance.
		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
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
	
	std::vector<const char*> Renderer::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

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
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

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

		bool result = indices.IsComplete() && extensionsSupported && isSwapChainAdequate;
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
		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;

		//Enabling swapchain as a device extension/
		createInfo.enabledExtensionCount = m_DeviceExtensions.size();
		createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = m_ValidationLayers.size();
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

	bool Renderer::CreateWindowSurface(GLFWwindow& window)
	{
		//Platform agnostic for no reason.
		//VkWin32SurfaceCreateInfoKHR info{};
		//info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//info.hwnd = glfwGetWin32Window(&window);
		//info.hinstance = GetModuleHandle(nullptr);
		//
		//if (vkCreateWin32SurfaceKHR(m_Instance, &info, nullptr, &m_Surface) != VK_SUCCESS)
		//{
		//	Log("Failed to create window surface", LOG_CRITICAL);
		//	return false;
		//}
		//else
		//{
		//	Log("Created window surface successfully.", LOG_SAFE);
		//	return true;
		//}

		if (glfwCreateWindowSurface(m_Instance, &window, nullptr, &m_Surface) != VK_SUCCESS)
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

			Log("Failed to find format using B8G8R8A8_SRGB AND COLOR_SPACE_SRGB, returning first available format.", LOG_MINIMAL);
			return formats[0];
		}
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
		is known as “vertical blank”.

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
		as “triple buffering”, although the existence of three buffers alone does 
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

	VkExtent2D Renderer::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow& window)
	{
		//This is a check to see if the window manager allows us to differ the resolution
		//of the window between pixels and screenspace.
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			//if vulkan does not fix the extent, we can just use glfw to query the fame buyffer
			//and create an image extent matched against it.
			int w, h;
			glfwGetFramebufferSize(&window, &w, &h);
			VkExtent2D actualExtent = { static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
			return actualExtent;
		}
	}

	bool Renderer::CreateSwapChain(GLFWwindow& window)
	{
		SwapChainSupportDetails details = QuerySwapChainSupport(m_PhysicalDevice);
		VkSurfaceFormatKHR format = ChooseSwapChainSurfaceFormat(details.Formats);
		VkPresentModeKHR presentMode = ChooseSwapChainPresentMode(details.PresentModes);
		VkExtent2D extent = ChooseSwapExtent(details.Capabilities, window);

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
		* //todo : window resizing.
		* This will be used in the future for window resizing but for now assuming theres one window adn swapchain.
		*/
		createInfo.oldSwapchain = VK_NULL_HANDLE;

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
}