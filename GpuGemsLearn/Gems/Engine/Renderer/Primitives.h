#pragma once
#include "memory"
#include "VertexArray.h"
#include "MaterialInstance.h"
class Scene;
class Primitives
{
public:
	virtual bool Render(std::shared_ptr<Scene> scene, std::shared_ptr<MaterialInstance> materialIns) = 0;
protected:
	std::shared_ptr<VertexArray> m_VertexArray;
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
};