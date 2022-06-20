#pragma once
#include "Core.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

#include "Application.h"
#include "GLFW/glfw3.h"

class Input
{
public:
	static bool IsKeyPressed(KeyCode keycode);
	static bool IsMouseButtonPressed(MouseCode button);
	static std::pair<float, float> GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
};

bool Input::IsKeyPressed(KeyCode keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(window, keycode);

	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetMouseButton(window, button);

	return state == GLFW_PRESS;
}

float Input::GetMouseX()
{
	auto [x, y] = GetMousePosition();
	return (float)x;
}

float Input::GetMouseY()
{
	auto [x, y] = GetMousePosition();
	return (float)y;
}

std::pair<float, float> Input::GetMousePosition() {
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos, (float)ypos };

};