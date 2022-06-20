#pragma once
#include "Material.h"
class MaterialInstance
{
public:
	MaterialInstance(std::shared_ptr<Material> material) : m_Material(material) {}
	MaterialInstance(std::string shaderPath) : m_Material(std::make_shared<Material>(shaderPath)) {}
	void SetValue(const std::string& paramName, int value);
	void SetValue(const std::string& paramName, const std::vector<int>& value);
	void SetValue(const std::string& paramName, float value);
	void SetValue(const std::string& paramName, const glm::vec2& value);
	void SetValue(const std::string& paramName, const glm::vec3& value);
	void SetValue(const std::string& paramName, const glm::vec4& value);
	void SetValue(const std::string& paramName, const glm::mat4& value);
	void SetValue(const std::string& paramName, const std::string& value);
	
	void Bind();

protected:
	MaterialParam& GetMaterialParam(const std::string& paramName);

private:
	std::shared_ptr<Material> m_Material;
	std::map<std::string, MaterialParam> m_MaterialParam;
};