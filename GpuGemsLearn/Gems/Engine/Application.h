#pragma once
#include "Window.h"
#include "LayerStack.h"
#include "WindowEvents.h"

class Application
{
public:
	Application(const std::string& name = "mio App");
	virtual ~Application();

	void OnEvent(Event& e);

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* layer);
	inline Window& GetWindow() { return *m_Window; }

	void Close();

	static Application& Get() { return *s_Instance; }
private:
	virtual void Run();
	bool OnWindowClose(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);

private:
	std::unique_ptr<Window> m_Window;
	bool m_Runing = true;
	bool m_Minimized = false;
	LayerStack m_LayerStack;

	float m_LastFrameTime = 0.0f;

private:
	static Application* s_Instance;
	friend int main();
};
