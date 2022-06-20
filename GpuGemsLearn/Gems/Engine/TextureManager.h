#pragma once
#include <map>
#include <string>
#include <memory>
#include "Renderer/Texture.h"
class TextureManager
{
public:
	std::shared_ptr<Texture2D>& GetTexture(const std::string& cName);
protected:
	std::map<std::string, std::shared_ptr<Texture2D>> m_Textures;
};

extern std::unique_ptr<TextureManager> g_TextureManager;