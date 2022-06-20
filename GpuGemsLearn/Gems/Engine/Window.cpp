#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Events.h"
#include "WindowEvents.h"
#include "MouseEvents.h"
#include "KeyEvents.h"

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error_code, const char* description) 
{
	printf("%d, %s", error_code, description);
}

Window::Window(const WindowsProps& props)
{
	Init(props);
}

Window::~Window()
{
	Shutdown();
}

void Window::OnUpdate()
{
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void Window::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_Data.VSync = enabled;
}

bool Window::IsVSync() const
{
    return m_Data.VSync;
}

void Window::Init(const WindowsProps& props)
{
    m_Data.Title = props.Title;
    m_Data.Height = props.Height;
    m_Data.Width = props.Width;

    if (s_GLFWWindowCount == 0) {
        int success = glfwInit();
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    {
        m_Windows = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;
    }

    m_Context = std::make_unique<OpenGLContext>(m_Windows);
    m_Context->Init();

    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    glfwSetWindowUserPointer(m_Windows, &m_Data);

    SetVSync(true);

    glfwSetWindowSizeCallback(m_Windows, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;
        WindowResizeEvent event(width, height);
        data.EventCallback(event);

        });

    glfwSetWindowCloseCallback(m_Windows, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
        });

    glfwSetKeyCallback(m_Windows, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action)
        {
        case GLFW_PRESS:
        {
            KeyPressedEvent event(key, mods, 0);
            data.EventCallback(event);
        }
        break;
        case GLFW_RELEASE:
        {
            KeyReleasedEvent event(key, mods);
            data.EventCallback(event);
        }
        break;
        case GLFW_REPEAT:
        {
            KeyPressedEvent event(key, mods, 1);
            data.EventCallback(event);
        }
        break;
        default:
            break;
        }
        });

    glfwSetCharCallback(m_Windows, [](GLFWwindow* window, unsigned int codepoint) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        KeyTypedEvent event(codepoint, 0);
        data.EventCallback(event);
        });


    glfwSetMouseButtonCallback(m_Windows, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action)
        {
        case GLFW_PRESS:
        {
            if (button == GLFW_MOUSE_BUTTON_1)
            {
                data.MouseLButtonDown = true;
                data.FirstData = false;
            }

            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
        }
        break;
        case GLFW_RELEASE:
        {
            if (button == GLFW_MOUSE_BUTTON_1)
            {
                data.MouseLButtonDown = false;
                data.FirstData = false;
            }

            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
        }
        break;
        default:
            break;
        }

        });

    glfwSetScrollCallback(m_Windows, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event((float)xoffset, (float)yoffset);
        data.EventCallback(event);
        });

    glfwSetCursorPosCallback(m_Windows, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        if (data.MouseLButtonDown) 
        {
            if (data.FirstData)
            {
                MouseMovedEvent event((float)(xpos - data.InitPosX), (float)(data.InitPosY - ypos));
                data.InitPosX = xpos;
                data.InitPosY = ypos;
                data.EventCallback(event);
            }
            else
            {
                data.FirstData = true;
                data.InitPosX = xpos;
                data.InitPosY = ypos;
            }
        }
        });
}

void Window::Shutdown()
{
    glfwDestroyWindow(m_Windows);
    --s_GLFWWindowCount;

    if (s_GLFWWindowCount == 0)
    {
        glfwTerminate();
    }
}
