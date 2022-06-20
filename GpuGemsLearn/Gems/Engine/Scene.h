#pragma once
#include "SceneCamera.h"
#include "Timestep.h"
#include "MouseEvents.h"
#include "Window.h"
#include "Light.h"
class Scene
{
public:
	Scene(const Window& window, SceneCamera::ProjectionType cameraType);

	glm::mat4 GetViewProjection();
	glm::vec3 GetCameraPos() { return m_CameraPos; }
	const LightParam& GetLightParam() { return m_Light; }
public:
	void CameraController(Timestep ts);
	bool OnMouseMoved(MouseMovedEvent& e);

protected:
	SceneCamera m_SceneCamera;
	LightParam m_Light = 
	{
		{200.0f, 200.0f, 200.0f},
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f}
	};
	glm::vec3 m_CameraPos = { 0.0f,6.0f,10.0f };
	glm::vec3 m_CameraFront = { 0.0f,0.0f, -1.0f };
	glm::vec3 m_CameraUp = { 0.0f,1.0f,0.0f };

	float yaw = -90.0f;
	float pitch = 0.0f;
};
