#include "OpenGLContext.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "assert.h"

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
:m_WindowHandle(windowHandle)
{

}

OpenGLContext::~OpenGLContext()
{
}

void OpenGLContext::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	assert(status);
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
