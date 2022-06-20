#include "Application.h"
#include "Renderer/Renderer.h"
#include "GLFW/glfw3.h"
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
{
	s_Instance = this;

	m_Runing = true;

	m_Window = std::make_unique<Window>(WindowsProps(name));
	m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

	Renderer::Init();

}

Application::~Application() {
	Renderer::Shutdown();
}

void Application::PushLayer(Layer* layer) {
	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}

void Application::Close() {
	m_Runing = false;
}

void Application::OnEvent(Event& e) {
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
		(*--it)->OnEvent(e);
		if (e.Handled)
			break;
	}
}

void Application::Run() {
	while (m_Runing) 
	{
		float time = (float)glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized) {

			{
				for (const auto& layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			{
				for (const auto& layer : m_LayerStack)
					layer->OnImGuiRender();
			}
		}

		m_Window->OnUpdate();
	}
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
	m_Runing = false;
	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
	if (e.GetWidth() == 0 || e.GetHeight() == 0) {
		m_Minimized = true;
		return false;
	}
	m_Minimized = false;
	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
	return false;
}
