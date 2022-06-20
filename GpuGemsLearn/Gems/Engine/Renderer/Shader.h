#pragma once
#include <string>
#include <unordered_map>
#include "glad/glad.h"
#include "glm/glm.hpp"
class Shader
{
	friend class Material;
public:
	Shader(const std::string& filepath);
	Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	virtual ~Shader();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual const std::string& GetName() const { return m_Name; }

	void UploadUniformInt(GLint index, int value);
	void UploadUniformIntArray(GLint index, int* values, uint32_t count);

	void UploadUniformFloat(GLint index, float value);
	void UploadUniformFloat2(GLint index, const glm::vec2& value);
	void UploadUniformFloat3(GLint index, const glm::vec3& value);
	void UploadUniformFloat4(GLint index, const glm::vec4& value);

	void UploadUniformMat3(GLint index, const glm::mat3& matrix);
	void UploadUniformMat4(GLint index, const glm::mat4& matrix);
private:
	std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
private:
	uint32_t m_RendererID;
	std::string m_Name;
};
