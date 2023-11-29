#pragma once

typedef uint64_t VkDeviceSize;
struct VkImage_T; typedef VkImage_T* VkImage;
struct VkDeviceMemory_T; typedef VkDeviceMemory_T* VkDeviceMemory;
struct VkBuffer_T; typedef VkBuffer_T* VkBuffer;
struct VkImageView_T; typedef VkImageView_T* VkImageView;
enum VkFormat;
enum VkImageLayout;

namespace Bennett
{
	class Renderer;
	class Buffer;

	class BENNETT_ENGINE Texture
	{
	private:
		uint32_t m_TextureID;
		uint32_t m_Width;
		uint32_t m_Height;
		VkImage m_Image;
		VkImageView m_ImageView;
		VkDeviceMemory m_ImageMemory;

		void TransitionImageLayout(Renderer& renderer, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void CopyBufferToImage(Renderer& renderer, Buffer& buffer, VkImage& image, uint32_t width, uint32_t height);

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
