#pragma once
#include <string>
#include <vulkan/vulkan.h>

namespace Bennett
{
	class BENNETT_ENGINE Shader
	{
	private:
		std::vector<char> ReadShaderFileToByteArray(const std::string& fileName);
		bool CreateVulkanShaderModule(const std::string& fileName);
		void CleanupVulkanShaderModule();

		std::string m_ReferenceName;
		std::string m_Location;
		VkShaderModule m_VulkanModule;

	public:
		Shader();
		~Shader();
		const bool& HasShaderLoaded() const;
		const std::string& GetFilename() const;
		const std::string& GetName() const;
		const VkShaderModule& GetVulkanModule() const;
		bool Reload();

		static void Create(Shader& shader, const std::string& location, const std::string& name = "");
	};
}
