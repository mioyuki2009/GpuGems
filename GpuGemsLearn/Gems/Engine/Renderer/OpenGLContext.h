#pragma once

struct GLFWwindow;

class OpenGLContext
{
public:
	OpenGLContext(GLFWwindow* windowHandle);
	virtual ~OpenGLContext();
	virtual void Init();
	virtual void SwapBuffers();
private:
	GLFWwindow* m_WindowHandle;
};