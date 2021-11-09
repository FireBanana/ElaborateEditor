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
	void Rotate(double x, double y, double deltaTime);

private:
	glm::mat4 m_PerspectiveMatrix;
	glm::mat4 m_ViewMatrix;
	
	glm::vec3 m_CameraPosition;

	float m_AspectRatio;
};