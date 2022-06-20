#include "SimplePrimitives.h"
#include "RenderCommand.h"
#include "../Model.h"

SimplePrimitive::SimplePrimitive(uint32_t vertexNum, uint32_t indicesNum)
{
    m_VertexArray = std::make_shared<VertexArray>();
    m_VertexBuffer = std::make_shared<VertexBuffer>(vertexNum * sizeof(MeshVertex));

	m_VertexBuffer->SetLayout({
		{ ShaderDataType::Float3, "aPos"     },
		{ ShaderDataType::Float3, "aNormal"        },
		{ ShaderDataType::Float2, "aTexCoords"     },
		});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
}

SimplePrimitive::SimplePrimitive(const std::vector<MeshVertex>& vertex, const std::vector<uint32_t>& indices)
{
	m_VertexArray = std::make_shared<VertexArray>();
	m_VertexBuffer = std::make_shared<VertexBuffer>(vertex.size() * sizeof(MeshVertex));

	m_VertexBuffer->SetLayout({
		{ ShaderDataType::Float3, "aPos"     },
		{ ShaderDataType::Float3, "aNormal"        },
		{ ShaderDataType::Float2, "aTexCoords"     },
		});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	AddVertex(vertex, indices);
}

SimplePrimitive::~SimplePrimitive()
{
	Release();
}

void SimplePrimitive::Release()
{
	
}

bool SimplePrimitive::Render(std::shared_ptr<Scene> scene, std::shared_ptr<MaterialInstance> materialIns)
{
	materialIns->Bind();
	const auto& viewProj = scene->GetViewProjection();
	materialIns->SetValue("u_ViewProjection", viewProj);

	materialIns->SetValue("light.position", scene->GetLightParam().position);
	materialIns->SetValue("light.ambient", scene->GetLightParam().ambient);
	materialIns->SetValue("light.diffuse", scene->GetLightParam().diffuse);
	materialIns->SetValue("light.specular", scene->GetLightParam().specular);
	RenderCommand::DrawIndexed(m_VertexArray, (uint32_t)m_MeshIndices.size());
	return true;
}

void SimplePrimitive::AddVertex(const std::vector<MeshVertex>& vertice, const std::vector<uint32_t>& indices)
{
	m_MeshIndices.insert(m_MeshIndices.end(), indices.begin(), indices.end());
	m_IndiceBuffer = std::make_shared<IndexBuffer>(m_MeshIndices.data(), m_MeshIndices.size());
	m_VertexArray->SetIndexBuffer(m_IndiceBuffer);
	
	m_MeshVertex.insert(m_MeshVertex.end(), vertice.begin(), vertice.end());
	m_VertexBuffer->SetData(m_MeshVertex.data(), sizeof(MeshVertex) * m_MeshVertex.size());
}