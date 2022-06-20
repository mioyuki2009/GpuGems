#include "RendererAPI.h"
#include "glad/glad.h"


void RendererAPI::Init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

void RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}

void RendererAPI::SetClearColor(const glm::vec4& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void RendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) {
	vertexArray->Bind();
	uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}