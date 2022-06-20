#include "Model.h"
#include <iostream>

Model::Model(const std::string& modelPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        assert(0);
        std::cout << importer.GetErrorString() << std::endl;
        return;
    }
    ProcessNode(scene->mRootNode, scene);
}

void Model::SetPos(const glm::vec3& pos)
{
    m_Pos = pos;
    for (auto& mesh : m_Meshs)
    {
        mesh.SetPosAndRotate(m_Pos, m_Rotate);
    }
}

void Model::SetDir(const glm::vec3& dir)
{
    m_Rotate = dir;
    for (auto& mesh : m_Meshs)
    {
        mesh.SetPosAndRotate(m_Pos, m_Rotate);
    }
}

bool Model::Render(std::shared_ptr<Scene> scene)
{
    for (auto& mesh : m_Meshs)
    {
        mesh.Render(scene);
    }

    return true;
}

void Model::SetDiffuseTexture(std::string textureName)
{
    for (auto& mesh : m_Meshs)
    {
        mesh.NormalMaterialInstance->SetValue("texture_diffuse", textureName);
    }
}

#define SETALLMESHVALUE(paramName, value)   \
    for (auto& mesh : m_Meshs)  \
    {   \
        mesh.NormalMaterialInstance->SetValue(paramName, value);    \
    }

void Model::SetValue(const std::string& paramName, int value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::SetValue(const std::string& paramName, const std::vector<int>& value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::SetValue(const std::string& paramName, float value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::SetValue(const std::string& paramName, const glm::vec2& value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::SetValue(const std::string& paramName, const glm::vec3& value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::SetValue(const std::string& paramName, const glm::vec4& value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::SetValue(const std::string& paramName, const glm::mat4& value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::SetValue(const std::string& paramName, const std::string& value)
{
    SETALLMESHVALUE(paramName, value)
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshs.push_back(ProcessMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<MeshVertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<MeshTexture> textures;

    // walk through each of the mesh's vertices
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        MeshVertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            //// tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<MeshTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, MeshTexture::MeshTextureType::Diffuse);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<MeshTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, MeshTexture::MeshTextureType::Specular);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<MeshTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, MeshTexture::MeshTextureType::Normal);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<MeshTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, MeshTexture::MeshTextureType::Height);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

std::vector<MeshTexture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, MeshTexture::MeshTextureType typeName)
{
    std::vector<MeshTexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        textures.push_back(MeshTexture{ typeName , str.C_Str() });
    }
    return textures;
}