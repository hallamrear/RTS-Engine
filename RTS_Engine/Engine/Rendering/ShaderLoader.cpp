#include <BennettPCH.h>
#include <Rendering/ShaderLoader.h>
#include <System/ServiceLocator.h>
#include <Rendering/Renderer.h>
#include <fstream>

namespace Bennett
{
	std::map<int, Shader*> ShaderLoader::m_IndexToShaderMap = {};
	std::map<std::string, int> ShaderLoader::m_NameToIndexMap = {};

	void ShaderLoader::DumpAll()
	{
		const VkDevice& device = ServiceLocator::GetRenderer().GetDevice();

		auto shaderItr = m_IndexToShaderMap.begin();

		while (shaderItr != m_IndexToShaderMap.end())
		{
			if (shaderItr->second)
			{
				//Cleans up our pointer.
				delete shaderItr->second;
				shaderItr->second = nullptr;

				shaderItr = m_IndexToShaderMap.erase(shaderItr);
			}
			else
				++shaderItr;
		}

		auto nameItr = m_NameToIndexMap.begin();
		
		while (nameItr != m_NameToIndexMap.end())
		{
			nameItr = m_NameToIndexMap.erase(nameItr);
		}
	}

	void ShaderLoader::ReloadAll()
	{
		auto shaderItr = m_IndexToShaderMap.begin();

		while (shaderItr != m_IndexToShaderMap.end())
		{
			shaderItr->second->Reload();
		}
	}

	Shader* ShaderLoader::GetShaderByName(const std::string& name)
	{
		std::map<std::string, int>::iterator nameItr = m_NameToIndexMap.find(name);

		if (nameItr == m_NameToIndexMap.end())
		{
			Log(LOG_MINIMAL, "Failed to find shader '%s' in shader index map. Returning nullptr.", name.c_str());
			return VK_NULL_HANDLE;
		}

		//Name has been found so uses the index from that map to get the shader.
		std::map<int, Shader*>::iterator shaderItr = m_IndexToShaderMap.find(nameItr->second);

		if (shaderItr == m_IndexToShaderMap.end())
		{
			Log(LOG_MINIMAL, "Failed to find shader '%s' with index '%d' in shader index map. Returning nullptr.", name.c_str(), nameItr->second);
			return VK_NULL_HANDLE;
		}

		return shaderItr->second;
	}

	bool ShaderLoader::LoadShader(const std::string& location, const std::string& preferredName)
	{
		if (location == "")
		{
			Log("No location provided for shader file.", LOG_MINIMAL);
			return false;
		}

		std::string storageName = location;

		if (preferredName != "")
		{
			storageName = preferredName;
		}
		
		Shader* shader = new Shader();
		Shader::Create(*shader, location, preferredName);

		if (shader->HasShaderLoaded() == false)
		{
			delete shader;
			shader = nullptr;
			return false;
		}

		int nextIndex = m_NameToIndexMap.size() + 1;
		m_NameToIndexMap.insert(std::make_pair(storageName, nextIndex));
		m_IndexToShaderMap.insert(std::make_pair(nextIndex, shader));

		return true;
	}

	bool ShaderLoader::ReloadShader(const std::string& name)
	{
		auto indexItr = m_NameToIndexMap.find(name);

		if (indexItr == m_NameToIndexMap.end())
		{
			Log(LOG_MINIMAL, "Failed to find shader with name '%s' for reloading.\n", name.c_str());
			return false;
		}

		auto shaderItr = m_IndexToShaderMap.find(indexItr->second);

		if (shaderItr == m_IndexToShaderMap.end())
		{
			Log(LOG_MINIMAL, "Failed to find shader with name '%s' and index '%d' for reloading.\n", name.c_str(), indexItr->second);
			return false;
		}

		return shaderItr->second->Reload();
	}
}