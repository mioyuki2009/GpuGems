#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

std::unique_ptr<Renderer::SceneData> Renderer::s_SceneData = std::make_unique<Renderer::SceneData>();
	
void Renderer::Init() {
	RenderCommand::Init();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height){
	RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::Shutdown()
{
}