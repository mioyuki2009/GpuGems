#pragma once
#include "../../Engine/Renderer/Material.h"

#include "../../Engine/Layer.h"
#include "../../Engine/Renderer/RenderCommand.h"

#include "../../Engine/MouseEvents.h"

#include "../../Engine/Model.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
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
	virtual void OnImGuiRender() override;
	bool OnMouseMoved(MouseMovedEvent& e);

	bool m_BlockEvents = false;

private:
	std::shared_ptr<Scene> m_Scene;
	std::shared_ptr<Model> m_SandModel;
	std::shared_ptr<Model> m_PillModel;
	std::shared_ptr<Model> m_WaterModel;

	glm::vec4 m_WaterColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);

	struct WaveParam
	{
		glm::vec2 direction = { 0.5f,0.5f };
		float frequency = 1.0f;
		float phase = 1.0f;
		float amplitude = 2.0f;
		float k = 1.0f;
	};
	WaveParam m_WaveParam[3] = 
	{
		{ {1.0,2.0}, 0.2f, 2.0f, 0.4f, 2.0f},
		{ {1.0,-1.0}, 0.5f, 2.0f, 0.3f, 2.0f},
		{ {1.0,2.0}, 0.2f, 0.5f, 0.5f, 3.0f}
	};
	
	Timestep m_TotalTime = 0;
};

static bool imguiWindow = true;

void MainLayer::OnImGuiRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();
	
	if (imguiWindow)
	{
		ImGui::Begin("Debug", &imguiWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		
		ImGui::ColorEdit4("Color", glm::value_ptr(m_WaterColor));

		if (ImGui::Button("Close"))
			imguiWindow = false;

		ImGui::PushID("Wave1");
		ImGui::Text("Wave1");
		ImGui::DragFloat2("Dir", glm::value_ptr(m_WaveParam[0].direction));
		ImGui::DragFloat("Frequency", &(m_WaveParam[0].frequency));
		ImGui::DragFloat("Phase", &(m_WaveParam[0].phase));
		ImGui::DragFloat("Amplitude", &(m_WaveParam[0].amplitude));
		ImGui::DragFloat("k", &(m_WaveParam[0].k));
		ImGui::PopID();

		ImGui::PushID("Wave2");
		ImGui::Text("Wave2");
		ImGui::DragFloat2("Dir", glm::value_ptr(m_WaveParam[1].direction));
		ImGui::DragFloat("Frequency", &(m_WaveParam[1].frequency));
		ImGui::DragFloat("Phase", &(m_WaveParam[1].phase));
		ImGui::DragFloat("Amplitude", &(m_WaveParam[1].amplitude));
		ImGui::DragFloat("k", &(m_WaveParam[1].k));
		ImGui::PopID();

		ImGui::PushID("Wave3");
		ImGui::Text("Wave3");
		ImGui::DragFloat2("Dir", glm::value_ptr(m_WaveParam[2].direction));
		ImGui::DragFloat("Frequency", &(m_WaveParam[2].frequency));
		ImGui::DragFloat("Phase", &(m_WaveParam[2].phase));
		ImGui::DragFloat("Amplitude", &(m_WaveParam[2].amplitude));
		ImGui::DragFloat("k", &(m_WaveParam[2].k));
		ImGui::PopID();

		bool m_ViewportFocused = ImGui::IsWindowFocused();
		bool m_ViewportHovered = ImGui::IsWindowHovered();
		m_BlockEvents = m_ViewportFocused && m_ViewportHovered;

		ImGui::End();
	}

	

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool MainLayer::OnMouseMoved(MouseMovedEvent& e)
{
	return m_Scene->OnMouseMoved(e);
}

void MainLayer::OnUpdate(Timestep ts) 
{
	m_TotalTime += ts;
	m_Scene->CameraController(ts);

	RenderCommand::Clear();
	
	m_WaterModel->SetValue("waterColor", m_WaterColor);
	m_WaterModel->SetValue("u_Time", (float)m_TotalTime.GetSeconds());
	m_WaterModel->SetValue("wave1.direction", m_WaveParam[0].direction);
	m_WaterModel->SetValue("wave1.frequency", m_WaveParam[0].frequency);
	m_WaterModel->SetValue("wave1.phase", m_WaveParam[0].phase);
	m_WaterModel->SetValue("wave1.amplitude", m_WaveParam[0].amplitude);
	m_WaterModel->SetValue("wave1.k", m_WaveParam[0].k);

	m_WaterModel->SetValue("wave2.direction", m_WaveParam[1].direction);
	m_WaterModel->SetValue("wave2.frequency", m_WaveParam[1].frequency);
	m_WaterModel->SetValue("wave2.phase", m_WaveParam[1].phase);
	m_WaterModel->SetValue("wave2.amplitude", m_WaveParam[1].amplitude);
	m_WaterModel->SetValue("wave2.k", m_WaveParam[1].k);


	m_WaterModel->SetValue("wave3.direction", m_WaveParam[2].direction);
	m_WaterModel->SetValue("wave3.frequency", m_WaveParam[2].frequency);
	m_WaterModel->SetValue("wave3.phase", m_WaveParam[2].phase);
	m_WaterModel->SetValue("wave3.amplitude", m_WaveParam[2].amplitude);
	m_WaterModel->SetValue("wave3.k", m_WaveParam[2].k);

	m_SandModel->Render(m_Scene);
	m_PillModel->Render(m_Scene);
	m_WaterModel->Render(m_Scene);
}

void MainLayer::OnAttach()
{
	RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	const auto& window = Application::Get().GetWindow();
	m_Scene = std::make_shared<Scene>(window, SceneCamera::ProjectionType::Perspective);

	m_SandModel = std::make_shared<Model>("assets/Gems1/Natural_Effects/Water_Simulation/sand.obj");
	m_PillModel = std::make_shared<Model>("assets/Gems1/Natural_Effects/Water_Simulation/pillars.obj");
	m_WaterModel = std::make_shared<Model>("assets/Gems1/Natural_Effects/Water_Simulation/water.obj", Model_Type::Normal_Model, "assets/shaders/Water.glsl");

	m_SandModel->SetDiffuseTexture("assets/texture/Sandy1.tga");
	m_PillModel->SetDiffuseTexture("assets/texture/white.tga");
	m_WaterModel->SetValue("waterColor", m_WaterColor);


	m_WaterModel->SetValue("wave1.direction", m_WaveParam[0].direction);
	m_WaterModel->SetValue("wave1.frequency", m_WaveParam[0].frequency);
	m_WaterModel->SetValue("wave1.phase", m_WaveParam[0].phase);
	m_WaterModel->SetValue("wave1.amplitude", m_WaveParam[0].amplitude);
	m_WaterModel->SetValue("wave1.k", m_WaveParam[0].k);

	m_WaterModel->SetValue("wave2.direction", m_WaveParam[1].direction);
	m_WaterModel->SetValue("wave2.frequency", m_WaveParam[1].frequency);
	m_WaterModel->SetValue("wave2.phase", m_WaveParam[1].phase);
	m_WaterModel->SetValue("wave2.amplitude", m_WaveParam[1].amplitude);
	m_WaterModel->SetValue("wave2.k", m_WaveParam[1].k);


	m_WaterModel->SetValue("wave3.direction", m_WaveParam[2].direction);
	m_WaterModel->SetValue("wave3.frequency", m_WaveParam[2].frequency);
	m_WaterModel->SetValue("wave3.phase", m_WaveParam[2].phase);
	m_WaterModel->SetValue("wave3.amplitude", m_WaveParam[2].amplitude);
	m_WaterModel->SetValue("wave3.k", m_WaveParam[2].k);


	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window.GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 150");
		
	}

}

void MainLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void MainLayer::OnEvent(Event& event)
{

	if (!m_BlockEvents) 
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&MainLayer::OnMouseMoved, this, std::placeholders::_1));
	}

}
