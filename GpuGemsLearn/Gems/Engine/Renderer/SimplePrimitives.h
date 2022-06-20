#pragma once
#include "Primitives.h"
#include "Buffer.h"
#include <vector>
#include "../Scene.h"
struct MeshVertex;

class SimplePrimitive : public Primitives
{
public:
	SimplePrimitive(uint32_t vertexNum, uint32_t indicesNum);
	SimplePrimitive(const std::vector<MeshVertex>& vertex, const std::vector<uint32_t>& indices);
	virtual ~SimplePrimitive();
	virtual bool Render(std::shared_ptr<Scene> scene,  std::shared_ptr<MaterialInstance> materialIns);

	void AddVertex(const std::vector<MeshVertex>& vertex, const std::vector<uint32_t>& indices);

	void Release();

private:
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndiceBuffer;

	std::vector<MeshVertex> m_MeshVertex;
	std::vector<uint32_t> m_MeshIndices;

	uint32_t m_VertexNum;
	uint32_t m_IndicesNum;
};
