#pragma once

typedef uint64_t VkDeviceSize;
class VkImage_T; typedef VkImage_T* VkImage;
class VkDeviceMemory_T; typedef VkDeviceMemory_T* VkDeviceMemory;
class VkBuffer_T; typedef VkBuffer_T* VkBuffer;
class VkImageView_T; typedef VkImageView_T* VkImageView;
enum VkFormat;
enum VkImageLayout;

namespace Bennett
{
	class Renderer;

	class Texture
	{
	private:
		uint32_t m_TextureID;
		uint32_t m_Width;
		uint32_t m_Height;
		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;

		bool CreateStagingBuffer(const Renderer& renderer, VkBuffer& stagingBuffer, VkDeviceMemory& bufferMemory, const VkDeviceSize& imageSize);
		bool CreateImage(const Renderer& renderer, const uint32_t& width, const uint32_t& height);
		bool CreateImageView(const Renderer& renderer, VkImageView& imageView, const VkImage& image, const VkFormat& format);
		void TransitionImageLayout(Renderer& renderer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyBufferToImage(Renderer& renderer, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);

	protected:

	public:
		static void Create(Texture& texture, const std::string& filepath);

		const VkImage& GetImage() const;
		const VkDeviceMemory& GetImageMemory() const;
		const VkImageView& GetImageView() const;

		Texture();
		~Texture();
		const uint32_t& GetWidth() const;
		const uint32_t& GetHeight() const;
		const uint32_t& GetID() const;
	};
}
