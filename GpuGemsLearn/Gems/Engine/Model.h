#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <vector>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Renderer/SimplePrimitives.h"

struct MeshVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MeshLightProperty {
    glm::vec3 ambient{ 0.0f,0.0f,0.0f };
    glm::vec3 diffuse{ 1.0f,1.0f,1.0f };
    glm::vec3 specular{ 0.0f,0.0f,0.0f };
    float shininess = 0;
};

struct MeshTexture {
    enum class MeshTextureType {
        Diffuse,
        Specular,
        Normal,
        Height,
    };
    MeshTextureType m_type;
    std::string m_MeshPath;
};

struct Mesh
{
    Mesh(const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MeshTexture>& textures)
    {
        Vertices = vertices;
        Indices = indices;
        Textures = textures;
        SimplePrimitivePtr = std::make_shared<SimplePrimitive>(vertices, indices);
        NormalMaterialInstance = std::make_shared<MaterialInstance>("assets/shaders/SimpleShder.glsl");

        NormalMaterialInstance->SetValue("u_Model", glm::mat4(1.0f));

    }
    
    void SetPosAndRotate(const glm::vec3& pos, const glm::vec3& rotate)
    {
        glm::qua<float> q = glm::qua<float>(glm::radians(rotate));
        glm::mat4 model = glm::mat4_cast(q) * glm::translate(glm::mat4(1.0f), pos);
        NormalMaterialInstance->SetValue("u_Model", model);
    }

    void Render(std::shared_ptr<Scene> scene)
    {
        NormalMaterialInstance->SetValue("material.ambient", m_Prop.ambient);
        NormalMaterialInstance->SetValue("material.diffuse", m_Prop.diffuse);
        NormalMaterialInstance->SetValue("material.specular", m_Prop.specular);
        NormalMaterialInstance->SetValue("material.shininess", m_Prop.shininess);

        SimplePrimitivePtr->Render(scene, NormalMaterialInstance);
    }

    std::vector<MeshVertex> Vertices;
    std::vector<uint32_t> Indices;
    std::vector<MeshTexture> Textures;
    std::shared_ptr<SimplePrimitive> SimplePrimitivePtr;
    std::shared_ptr<MaterialInstance> NormalMaterialInstance;

    MeshLightProperty m_Prop = {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f},
        1.0f
    };

};

class Model
{
public:
    Model(const std::string& modelPath);
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
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, MeshTexture::MeshTextureType typeName);

protected:
    std::vector<Mesh> m_Meshs;

    glm::vec3 m_Pos = { 0.0f,0.0f,0.0f };
    glm::vec3 m_Rotate = { 0.0f,0.0f,0.0f };

    
};