#include "ShaderManager.h"

std::unique_ptr<ShaderManager> g_ShaderManager = std::make_unique<ShaderManager>();

std::shared_ptr<Shader>& ShaderManager::GetShader(const std::string& cName)
{
	const auto& texturePair = m_Shaders.find(cName);
	if (texturePair == m_Shaders.end())
	{
		m_Shaders[cName] = std::make_shared<Shader>(cName);
		return m_Shaders[cName];
	}
	else
	{
		return texturePair->second;
	}
}
