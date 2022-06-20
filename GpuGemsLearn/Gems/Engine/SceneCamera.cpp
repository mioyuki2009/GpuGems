#include "SceneCamera.h"
#include "glm/gtc/matrix_transform.hpp"

SceneCamera::SceneCamera()
{
	RecalculateProjection();
}

void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip) {
	m_ProjectionType = ProjectionType::Perspective;
	m_PerspectiveFov = verticalFov;
	m_PerspectiveNear = nearClip;
	m_PerspectiveFar = farClip;
	RecalculateProjection();
}

void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
{
	m_ProjectionType = ProjectionType::Orthographic;
	m_OrthographicSize = size;
	m_OrthographicNear = nearClip;
	m_OrthographicFar = farClip;
	RecalculateProjection();
}

void SceneCamera::RecalculateProjection() {

	if (m_ProjectionType == ProjectionType::Perspective)
	{
		m_Projection = glm::perspective(m_PerspectiveFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
	}
	else if (m_ProjectionType == ProjectionType::Orthographic)
	{
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = -orthoLeft;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = -orthoBottom;
		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
			m_OrthographicNear, m_OrthographicFar);
	}

}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
	m_AspectRatio = float(width) / float(height);
	RecalculateProjection();
}