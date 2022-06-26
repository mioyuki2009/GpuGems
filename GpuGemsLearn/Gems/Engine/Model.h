#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <vector>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Renderer/SimplePrimitives.h"

#include "Mesh.h"

enum class Model_Type
{
    Normal_Model,
    Water,

};

class Model
{
public:
    Model(const std::string& modelPath, Model_Type type = Model_Type::Normal_Model, const std::string& shderPath = "assets/shaders/SimpleShder.glsl");
    bool Render(std::shared_ptr<Scene> scene);
    void SetDiffuseTexture(std::string textureName);
    void SetPos(const glm::vec3& pos);
    void SetDir(const glm::vec3& dir);

    void SetValue(const std::string& paramName, int value);
    void SetValue(const std::string& paramName, const std::vector<int>& value);
    void SetValue(const std::string& paramName, float value);
    void SetValue(const std::string& paramName, const glm::vec2& value);
    void SetValue(const std::string& paramName, const glm::vec3& value);
    void SetValue(const std::string& paramName, const glm::vec4& value);
    void SetValue(const std::string& paramName, const glm::mat4& value);
    void SetValue(const std::string& paramName, const std::string& value);

protected:
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, MeshTexture::MeshTextureType typeName);

protected:
    std::vector<std::shared_ptr<Mesh>> m_Meshs;

    glm::vec3 m_Pos = { 0.0f,0.0f,0.0f };
    glm::vec3 m_Rotate = { 0.0f,0.0f,0.0f };

    std::string m_ShaderPath;

    Model_Type m_Type = Model_Type::Normal_Model;
};