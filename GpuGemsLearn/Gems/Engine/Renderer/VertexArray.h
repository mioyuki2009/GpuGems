#pragma once
#include "Buffer.h"
#include <memory>

class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
	virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const { return m_IndexBuffers; }

private:
	uint32_t m_RendererID = 0;
	uint32_t m_VertexBufferIndex = 0;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffers;
};