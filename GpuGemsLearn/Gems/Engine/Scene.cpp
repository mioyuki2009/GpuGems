#include "Scene.h"
#include "Input.h"
#include "glm/ext/matrix_transform.hpp"

constexpr float CameraTranslationSpeed = 5.0f;

Scene::Scene(const Window& window, SceneCamera::ProjectionType cameraType)
{
	m_SceneCamera.SetProjectionType(cameraType);
	m_SceneCamera.SetViewportSize(window.GetWidth(), window.GetHeight());
}

glm::mat4 Scene::GetViewProjection()
{
	return m_SceneCamera.GetProjection()* glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
}

void Scene::CameraController(Timestep ts)
{
	if (Input::IsKeyPressed(Key::A)) {
		m_CameraPos -= CameraTranslationSpeed * ts * glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
	}
	else if (Input::IsKeyPressed(Key::D)) {
		m_CameraPos += CameraTranslationSpeed * ts * glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
	}
	if (Input::IsKeyPressed(Key::W)) {
		m_CameraPos += CameraTranslationSpeed * ts * m_CameraFront;
	}
	else if (Input::IsKeyPressed(Key::S)) {
		m_CameraPos -= CameraTranslationSpeed * ts * m_CameraFront;
	}
	if (Input::IsKeyPressed(Key::Q)) {
		m_CameraPos += CameraTranslationSpeed * ts * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (Input::IsKeyPressed(Key::E)) {
		m_CameraPos -= CameraTranslationSpeed * ts * glm::vec3(0.0f, 1.0f, 0.0f);;
	}
}

bool Scene::OnMouseMoved(MouseMovedEvent& e)
{
	yaw += e.GetX() * CameraTranslationSpeed * 0.01f;
	pitch += e.GetY() * CameraTranslationSpeed * 0.01f;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_CameraFront = glm::normalize(front);

	return false;
}