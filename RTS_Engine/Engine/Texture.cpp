#include "BennettPCH.h"
#include "ServiceLocator.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "Renderer.h"
#include "Buffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

namespace Bennett
{
	bool Texture::CreateStagingBuffer(const Renderer& renderer, VkBuffer& stagingBuffer, VkDeviceMemory& bufferMemory, const VkDeviceSize& imageSize)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = imageSize;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(renderer.GetDevice(), &bufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS)
		{
			Log("Failed to create texture staging buffer", LOG_SERIOUS);
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(renderer.GetDevice(), stagingBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(renderer.GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		{
			Log("Failed to allocate memory for texture.", LOG_SERIOUS);
			return false;
		}

		vkBindBufferMemory(renderer.GetDevice(), stagingBuffer, bufferMemory, 0);

		return true;
	}

	bool Texture::CreateImage(const Renderer& renderer, const uint32_t& width, const uint32_t& height)
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; 

		if (vkCreateImage(renderer.GetDevice(), &imageInfo, nullptr, &m_Image) != VK_SUCCESS)
		{
			Log("Failed to create an image.", LOG_SERIOUS);
			return false;
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(renderer.GetDevice(), m_Image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Buffer::FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(renderer.GetDevice(), &allocInfo, nullptr, &m_ImageMemory) != VK_SUCCESS) 
		{
			Log("Failed to allocate image memory", LOG_SERIOUS);
			return false;
		}

		vkBindImageMemory(renderer.GetDevice(), m_Image, m_ImageMemory, 0);
		
		return true;
	}

	bool Texture::CreateImageView(const Renderer& renderer, VkImageView& imageView, const VkImage& image, const VkFormat& format)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(renderer.GetDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			Log("Failed to create texture image view", LOG_SERIOUS);
			return true;
		}
		
		return true;
	}

	void Texture::TransitionImageLayout(Renderer& renderer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer cmd = renderer.BeginSingleTimeCommands();

		//one of the most command ways to perform layout transitions is an image memory barrier.
		//generally used to sync access to resources, like ensuring that a write to buffer completes
		//before readings from it. can also be used to transfer layouts and ownerships when VK_SHARING_MODE_EXCLUSIVE is used.
		//there is an equivalent buffer memory barrier to do this for buffers.
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		//specify objects for layout transition
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		//if you are transfering family ownership, then these two fiels should be indices
		//of the queue family. they must be set to vk_queue_family_ignored if not doing this
		//(NOT THE DEFAULT VALUE)
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		//the image and subresource range specify how the image is affected and what part of the image.
		//this is set as such because our image is not an array and does not have mipmapping levels.
		barrier.image = m_Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.dstAccessMask = 0; //todo
		barrier.srcAccessMask = 0; //todo;

		VkPipelineStageFlags sourceStage{};
		VkPipelineStageFlags destinationStage{};

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;	
		}
		else 
		{
			Log("Unsupported layout transition", LOG_SERIOUS);
		}

		vkCmdPipelineBarrier(cmd,
			sourceStage, destinationStage,
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		renderer.EndSingleTimeCommands(cmd);
	}

	void Texture::CopyBufferToImage(Renderer& renderer, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height)
	{
		VkCommandBuffer cmdBuffer = renderer.BeginSingleTimeCommands();

		//Specifying which part of the buffer is being copied to which part of the image.
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		//Buffer to image copt operations are enqueued using this.
		vkCmdCopyBufferToImage(cmdBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		renderer.EndSingleTimeCommands(cmdBuffer);
	}

	void Texture::Create(Texture& texture, const std::string& filepath)
	{
		Renderer& renderer = ServiceLocator::GetRenderer();

		//Creating texture image
		int width, height, channels;
		stbi_uc* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, STBI_rgb_alpha);

		VkDeviceSize imageSize = width * height * 4;

		if (!pixels)
		{
			Log("Failed to load texture image" + filepath, LOG_SERIOUS);
			return;
		}

		//Creating staging buffer
		VkBuffer stagingBuffer;
		VkDeviceMemory bufferMemory;
		
		bool result = texture.CreateStagingBuffer(renderer, stagingBuffer, bufferMemory, imageSize);
		if (result == false)
		{
			Log("Failed to create texture properly.", LOG_SERIOUS);
			return;
		}
		
		void* data;
		vkMapMemory(renderer.GetDevice(), bufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, imageSize);
		vkUnmapMemory(renderer.GetDevice(), bufferMemory);

		//Free up pixels from stb_image
		stbi_image_free(pixels);

		result = texture.CreateImage(renderer, width, height);
		if (result == false)
		{
			Log("Failed to create texture properly.", LOG_SERIOUS);
			return;
		}

		texture.TransitionImageLayout(renderer, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		texture.CopyBufferToImage(renderer, stagingBuffer, texture.m_Image, width, height);
		texture.TransitionImageLayout(renderer, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(renderer.GetDevice(), stagingBuffer, nullptr);
		vkFreeMemory(renderer.GetDevice(), bufferMemory, nullptr);
		
		result = texture.CreateImageView(renderer, texture.m_ImageView, texture.m_Image, VK_FORMAT_R8G8B8A8_SRGB);
		if (result == false)
		{
			Log("Failed to create texture properly.", LOG_SERIOUS);
			return;
		}

		texture.m_Width = width;
		texture.m_Height = height;
		texture.m_TextureID = TextureLoader::m_TextureIDCounter + 1;
	}

	const VkImage& Texture::GetImage() const
	{
		return m_Image;
	}

	const VkDeviceMemory& Texture::GetImageMemory() const
	{
		return m_ImageMemory;
	}

	const VkImageView& Texture::GetImageView() const
	{
		return m_ImageView;
	}

	Texture::Texture()
	{
		m_Image = NULL;
		m_ImageMemory = NULL;
	}

	Texture::~Texture()
	{
		//todo : cleanup.
		//vkDestroyImage(renderer.GetDevice(), m_Image, nullptr);
		//vkFreeMemory(renderer.GetDevice(), m_ImageMemory, nullptr);
		//vkDestroyImageView(renderer.GetDevice(), m_ImageView, nullptr);
	}

	const uint32_t& Texture::GetWidth() const
	{
		return m_Width;
	}

	const uint32_t& Texture::GetHeight() const
	{
		return m_Height;
	}

	const uint32_t& Texture::GetID() const
	{
		return m_TextureID;
	}
}