#pragma once
#include <Rendering/Shader.h>
#include <map>
#include <vulkan/vulkan.h>

namespace Bennett
{
	class BENNETT_ENGINE ShaderLoader
	{
	private:
		static std::map<int, Shader*> m_IndexToShaderMap;
		static std::map<std::string, int> m_NameToIndexMap;

	protected:

	public:
		static void DumpAll();
		static void ReloadAll();

		static Shader* GetShaderByName(const std::string& name);
		static bool LoadShader(const std::string& location, const std::string& preferredName = "");
		static bool ReloadShader(const std::string& name);
	};
}
