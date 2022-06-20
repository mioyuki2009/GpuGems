#include "MaterialInstance.h"

MaterialParam& MaterialInstance::GetMaterialParam(const std::string& paramName)
{
	const auto& p = m_MaterialParam.find(paramName);
	if (p == m_MaterialParam.end())
	{
		m_MaterialParam[paramName] = MaterialParam();
		return m_MaterialParam[paramName];
	}
	return p->second;
}

void MaterialInstance::SetValue(const std::string& paramName, int value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_INT;
	materialParam.value_int = value;
}

void MaterialInstance::SetValue(const std::string& paramName, const std::vector<int>& value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_ARRAY;
	materialParam.value_array = value;
}

void MaterialInstance::SetValue(const std::string& paramName, float value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT;
	materialParam.value_float = value;
}

void MaterialInstance::SetValue(const std::string& paramName, const glm::vec2& value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT2;
	materialParam.value_float2 = value;
}

void MaterialInstance::SetValue(const std::string& paramName, const glm::vec3& value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT3;
	materialParam.value_float3 = value;
}

void MaterialInstance::SetValue(const std::string& paramName, const glm::vec4& value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT4;
	materialParam.value_float4 = value;
}

void MaterialInstance::SetValue(const std::string& paramName, const glm::mat4& value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_MAT4;
	materialParam.value_mat4 = value;
}

void MaterialInstance::SetValue(const std::string& paramName, const std::string& value)
{
	auto& materialParam = GetMaterialParam(paramName);
	materialParam.m_Type = MaterialParam::MaterialParamType::MATERIAL_TYPE_TEXTURE;
	materialParam.value_string = value;
}

void MaterialInstance::Bind()
{
	m_Material->Bind();

	for (const auto& [pname, value] : m_MaterialParam)
	{
		switch (value.m_Type)
		{
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_INT:
			{
				m_Material->SetValue(pname, value.value_int);
				break;
			}
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_ARRAY:
			{
				m_Material->SetValue(pname, value.value_array);
				break;
			}
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT:
			{
				m_Material->SetValue(pname, value.value_float);
				break;
			}
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT2:
			{
				m_Material->SetValue(pname, value.value_float2);
				break;
			}
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT3:
			{
				m_Material->SetValue(pname, value.value_float3);
				break;
			}
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_FLOAT4:
			{
				m_Material->SetValue(pname, value.value_float4);
				break;
			}
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_MAT4:
			{
				m_Material->SetValue(pname, value.value_mat4);
				break;
			}
			case MaterialParam::MaterialParamType::MATERIAL_TYPE_TEXTURE:
			{
				m_Material->SetValue(pname, value.value_string);
				break;
			}
			default:
				assert(0 && "wrong type");
			}
	}
}
