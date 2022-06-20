#pragma once

#include "glm/glm.hpp"
#include "VertexArray.h"
#include <memory>
class RendererAPI
{
public:
	virtual void Init();
	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	virtual void SetClearColor(const glm::vec4& color);
	virtual void Clear();

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0);
};
