#include "TextureManager.h"

std::unique_ptr<TextureManager> g_TextureManager = std::make_unique<TextureManager>();

std::shared_ptr<Texture2D>& TextureManager::GetTexture(const std::string& cName)
{
	const auto& texturePair = m_Textures.find(cName);
	if (texturePair == m_Textures.end())
	{
		m_Textures[cName] = std::make_shared<Texture2D>(cName);
		return m_Textures[cName];
	}
	else
	{
		return texturePair->second;
	}
}