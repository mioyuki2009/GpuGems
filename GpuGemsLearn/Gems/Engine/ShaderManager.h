#pragma once
#include <map>
#include <string>
#include <memory>
#include "Renderer/Shader.h"
class ShaderManager
{
public:
	std::shared_ptr<Shader>& GetShader(const std::string& cName);
protected:
	std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
};

extern std::unique_ptr<ShaderManager> g_ShaderManager;