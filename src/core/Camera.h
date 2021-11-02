#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();

	inline const glm::mat4& GetPerspective() { return m_PerspectiveMatrix; }
	inline const glm::mat4& GetView() { return m_ViewMatrix; }

	void UpdateAspectRatio(float r);
private:
	glm::mat4 m_PerspectiveMatrix;
	glm::mat4 m_ViewMatrix;

	float m_AspectRatio;
};