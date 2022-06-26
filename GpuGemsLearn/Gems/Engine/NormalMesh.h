#pragma once
#include "Mesh.h"

struct MeshLightProperty {
    glm::vec3 ambient{ 0.0f,0.0f,0.0f };
    glm::vec3 diffuse{ 1.0f,1.0f,1.0f };
    glm::vec3 specular{ 0.0f,0.0f,0.0f };
    float shininess = 0;
};


class NormalMesh : public Mesh
{
public:
    NormalMesh(const std::string& shderPath, const std::vector<MeshVertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<MeshTexture>& textures)
        :Mesh(shderPath, vertices, indices, textures)
    {

    }
    void Render(std::shared_ptr<Scene> scene) override
    {
        NormalMaterialInstance->SetValue("material.ambient", m_Prop.ambient);
        NormalMaterialInstance->SetValue("material.diffuse", m_Prop.diffuse);
        NormalMaterialInstance->SetValue("material.specular", m_Prop.specular);
        NormalMaterialInstance->SetValue("material.shininess", m_Prop.shininess);

        SimplePrimitivePtr->Render(scene, NormalMaterialInstance);
    }

    MeshLightProperty m_Prop = {
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 0.0f},
        1.0f
    };

};
