#pragma once
#include "../../Engine/Renderer/Material.h"

#include "../../Engine/Layer.h"
#include "../../Engine/Renderer/RenderCommand.h"

#include "../../Engine/MouseEvents.h"

#include "../../Engine/Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../../Engine/Scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

class MainLayer : public Layer 
{
public:
	MainLayer() {};
	virtual ~MainLayer() = default;

protected:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnEvent(Event& event) override;

	bool OnMouseMoved(MouseMovedEvent& e);

private:
	std::shared_ptr<Scene> m_Scene;
	std::shared_ptr<Model> m_TestModel1;
	std::shared_ptr<Model> m_TestModel2;
	std::shared_ptr<Model> m_TestModel3;
};

bool MainLayer::OnMouseMoved(MouseMovedEvent& e)
{
	return m_Scene->OnMouseMoved(e);
}

void MainLayer::OnUpdate(Timestep ts) 
{
	m_Scene->CameraController(ts);

	RenderCommand::Clear();

	m_TestModel1->Render(m_Scene);
	m_TestModel2->Render(m_Scene);
	//m_TestModel3->Render(m_Scene);
}

void MainLayer::OnAttach()
{
	RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	const auto& window = Application::Get().GetWindow();
	m_Scene = std::make_shared<Scene>(window, SceneCamera::ProjectionType::Perspective);

	m_TestModel1 = std::make_shared<Model>("assets/Gems1/Natural_Effects/Water_Simulation/sand.obj");
	m_TestModel2 = std::make_shared<Model>("assets/Gems1/Natural_Effects/Water_Simulation/pillars.obj");
	//m_TestModel3 = std::make_shared<Model>("assets/Gems1/Natural_Effects/Water_Simulation/WaterMesh.x");

	m_TestModel1->SetDiffuseTexture("assets/texture/Sandy.tga");
	m_TestModel2->SetDiffuseTexture("assets/texture/white.tga");

	//m_TestModel1->SetPos(glm::vec3(1.0f, 0.0f, 1.0f));

	//m_TestModel3->SetDiffuseTexture("assets/texture/b.tga");
}

void MainLayer::OnDetach()
{

}

void MainLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseMovedEvent>(std::bind(&MainLayer::OnMouseMoved, this, std::placeholders::_1));
}
