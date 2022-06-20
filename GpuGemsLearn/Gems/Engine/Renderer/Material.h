#pragma once
#include <memory>
#include <map>
#include "../ShaderManager.h"
#include "../TextureManager.h"
class Material
{
public:
	Material(const std::string& shaderPath)
	{
		m_Shader = g_ShaderManager->GetShader(shaderPath);
		m_ShaderID = m_Shader->m_RendererID;
		GLint numActiveUniforms = 0;
		glGetProgramInterfaceiv(m_ShaderID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);
		std::vector<GLchar> nameData(256);
		std::vector<GLenum> properties = { GL_NAME_LENGTH,GL_TYPE,GL_ARRAY_SIZE };
		std::vector<GLint> values(properties.size());

		for (int i = 0; i < numActiveUniforms; i++)
		{
			glGetProgramResourceiv(m_ShaderID, GL_UNIFORM, i, (GLsizei)properties.size(),
				&properties[0], (GLsizei)values.size(), NULL, &values[0]);
			nameData.resize(values[0]); //The length of the name.
			glGetProgramResourceName(m_ShaderID, GL_UNIFORM, i, (GLsizei)nameData.size(), NULL, &nameData[0]);
			std::string name((char*)&nameData[0], nameData.size() - 1);
			GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
			m_MaterialParam[name] = location;
			m_ParamType[name] = values[1];

			if (values[1] == GL_SAMPLER_2D)
			{
				m_MaterialParam[name] = m_index;
				m_index++;
			}
		}
	};
	void Bind()
	{
		m_Shader->Bind();
	}

	template <typename T>
	void SetValue(std::string name, T value) {
		const auto& valueIndex = m_MaterialParam.find(name);
		const auto& typeIndex = m_ParamType.find(name);

		if (typeIndex == m_ParamType.end())
		{
			assert("wrong param");
		}

		if (valueIndex != m_MaterialParam.end())
		{
			SetValueIndex(valueIndex->second, typeIndex->second, value);
		}
		else 
		{
			assert("wrong param");
		}
	}


protected:

#define MATERIAL_TYPE_CHECK(type)	\
	if (glType != type)	\
	{	\
		assert(0 && "wrong type");	\
		return;	\
	}
	void SetValueIndex(GLint index, GLenum glType, int value) 
	{
		MATERIAL_TYPE_CHECK(GL_INT);
		m_Shader->UploadUniformInt(index, value);
	}

	void SetValueIndex(GLint index, GLenum glType, std::vector<int> value) 
	{
		MATERIAL_TYPE_CHECK(GL_INT);
		m_Shader->UploadUniformIntArray(index, value.data(), (uint32_t)value.size());
	}

	void SetValueIndex(GLint index, GLenum glType, float value) 
	{
		MATERIAL_TYPE_CHECK(GL_FLOAT);
		m_Shader->UploadUniformFloat(index, value);
	}

	void SetValueIndex(GLint index, GLenum glType, const glm::vec2& value) 
	{
		MATERIAL_TYPE_CHECK(GL_FLOAT_VEC2);
		m_Shader->UploadUniformFloat2(index, value);
	}

	void SetValueIndex(GLint index, GLenum glType, const glm::vec3& value) 
	{
		MATERIAL_TYPE_CHECK(GL_FLOAT_VEC3);
		m_Shader->UploadUniformFloat3(index, value);
	}

	void SetValueIndex(GLint index, GLenum glType, const glm::vec4& value) 
	{
		MATERIAL_TYPE_CHECK(GL_FLOAT_VEC4);
		m_Shader->UploadUniformFloat4(index, value);
	}

	void SetValueIndex(GLint index, GLenum glType, const glm::mat4& value) 
	{
		MATERIAL_TYPE_CHECK(GL_FLOAT_MAT4);
		m_Shader->UploadUniformMat4(index, value);
	}

	void SetValueIndex(GLint index, GLenum glType, const std::string& value)
	{
		MATERIAL_TYPE_CHECK(GL_SAMPLER_2D);
		const auto& texture = g_TextureManager->GetTexture(value);
		m_Shader->Bind();
		texture->Bind(index);
	}

protected:
	std::map<std::string, GLint> m_MaterialParam;
	std::map<std::string, GLint> m_ParamType;
	uint32_t m_ShaderID;
	std::shared_ptr<Shader> m_Shader;

private:
	uint32_t m_index = 0;
};

struct MaterialParam
{
	enum class MaterialParamType
	{
		MATERIAL_TYPE_NONE,
		MATERIAL_TYPE_INT,
		MATERIAL_TYPE_ARRAY,
		MATERIAL_TYPE_FLOAT,
		MATERIAL_TYPE_FLOAT2,
		MATERIAL_TYPE_FLOAT3,
		MATERIAL_TYPE_FLOAT4,
		MATERIAL_TYPE_MAT4,
		MATERIAL_TYPE_TEXTURE,
	};

	MaterialParamType m_Type = MaterialParamType::MATERIAL_TYPE_NONE;
	int value_int;
	std::vector<int> value_array;
	float value_float;
	glm::vec2 value_float2;
	glm::vec3 value_float3;
	glm::vec4 value_float4;
	glm::mat4 value_mat4;
	std::string value_string;
};