#pragma once
#include <string>
#include "Events.h"
#include "../Engine/Renderer/OpenGLContext.h"
#include <functional>
#include <memory>

struct GLFWwindow;
struct WindowsProps
{
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    WindowsProps(const std::string& title = "mios Engine",
        uint32_t width = 1600,
        uint32_t height = 900
    ) :Title(title), Width(width), Height(height) {};
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

public:
    Window(const WindowsProps& props);
    virtual ~Window();

    void OnUpdate();

    inline uint32_t GetWidth() const { return m_Data.Width; }
    inline uint32_t GetHeight() const { return m_Data.Height; }

    inline void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
    void SetVSync(bool enabled);
    bool IsVSync() const;

    inline void* GetNativeWindow() const { return m_Windows; }

private:
    virtual void Init(const WindowsProps& props);
    virtual void Shutdown();

private:
    GLFWwindow* m_Windows;
    std::unique_ptr<OpenGLContext> m_Context;
    struct WindowData
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        bool VSync;

        bool MouseLButtonDown;
        bool FirstData;
        double InitPosX;
        double InitPosY;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};
