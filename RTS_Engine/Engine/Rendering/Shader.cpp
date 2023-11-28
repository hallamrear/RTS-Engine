#include <BennettPCH.h>
#include <Rendering/Shader.h>
#include <fstream>
#include <System/ServiceLocator.h>
#include <Rendering/Renderer.h>

namespace Bennett
{
	Shader::Shader()
	{
		m_Location = "";
		m_ReferenceName = "";
		m_VulkanModule = VK_NULL_HANDLE;
	}

	Shader::~Shader()
	{
		m_Location = "";
		m_ReferenceName = "";

		if (m_VulkanModule != VK_NULL_HANDLE)
		{
			CleanupVulkanShaderModule();
		}
	}

	const bool& Shader::HasShaderLoaded() const
	{
		return (m_VulkanModule != VK_NULL_HANDLE);
	}

	const std::string& Shader::GetFilename() const
	{
		return m_Location;
	}

	const std::string& Shader::GetName() const
	{
		return m_ReferenceName;
	}

	const VkShaderModule& Shader::GetVulkanModule() const
	{
		return m_VulkanModule;
	}

	bool Shader::Reload()
	{
		CleanupVulkanShaderModule();
		return CreateVulkanShaderModule(m_Location);
	}

	void Shader::Create(Shader& shader, const std::string& location, const std::string& name)
	{
		if (shader.HasShaderLoaded())
		{
			shader.CleanupVulkanShaderModule();
			shader.m_Location = "";
			shader.m_ReferenceName = "";
		}
		
		if (shader.CreateVulkanShaderModule(location))
		{
			shader.m_Location = location;
			shader.m_ReferenceName = name;
		}
		else
		{
			Log(LOG_MINIMAL, "Failed to create shader module for shader '%s' with filename'%s'\n", name.c_str(), location.c_str());
			shader.CleanupVulkanShaderModule();
			shader.m_Location = "";
			shader.m_ReferenceName = "";
		}
	}

	bool Shader::CreateVulkanShaderModule(const std::string& fileName)
	{
		std::vector<char> shaderByteArray = ReadShaderFileToByteArray(fileName.data());

		if (shaderByteArray.size() <= 0)
		{
			Log(LOG_SERIOUS, "Failed to read byte array data from shader file.\n");
			return false;
		}

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shaderByteArray.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderByteArray.data());

		if (vkCreateShaderModule(ServiceLocator::GetRenderer().GetDevice(), &createInfo, nullptr, &m_VulkanModule) != VK_SUCCESS)
		{
			Log("Failed to create shader module.", LOG_SERIOUS);
			m_VulkanModule = VK_NULL_HANDLE;
			return false;
		}

		return true;
	}

	void Shader::CleanupVulkanShaderModule()
	{
		vkDestroyShaderModule(ServiceLocator::GetRenderer().GetDevice(), m_VulkanModule, nullptr);
		m_VulkanModule = VK_NULL_HANDLE;
	}

	std::vector<char> Shader::ReadShaderFileToByteArray(const std::string& fileName)
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
}
