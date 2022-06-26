#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <vector>
#include <string>
#include <memory>
#include "Renderer/SimplePrimitives.h"

struct MeshVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
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

class Mesh
{
    friend class Model;
public:

    Mesh(const std::string& shderPath, const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MeshTexture>& textures)
    {
        Vertices = vertices;
        Indices = indices;
        Textures = textures;

        SimplePrimitivePtr = std::make_shared<SimplePrimitive>(vertices, indices);
        NormalMaterialInstance = std::make_shared<MaterialInstance>(shderPath);

        NormalMaterialInstance->SetValue("u_Model", glm::mat4(1.0f));
    }

    void SetPosAndRotate(const glm::vec3& pos, const glm::vec3& rotate)
    {
        glm::qua<float> q = glm::qua<float>(glm::radians(rotate));
        glm::mat4 model = glm::mat4_cast(q) * glm::translate(glm::mat4(1.0f), pos);
        NormalMaterialInstance->SetValue("u_Model", model);
    }

    virtual void Render(std::shared_ptr<Scene> scene) = 0;

protected:

    std::shared_ptr<SimplePrimitive> SimplePrimitivePtr;
    std::shared_ptr<MaterialInstance> NormalMaterialInstance;

    std::vector<MeshVertex> Vertices;
    std::vector<uint32_t> Indices;
    std::vector<MeshTexture> Textures;
};