#include "Camera.h"

#include "glm/gtc/matrix_transform.inl"

namespace CE
{
	Camera::Camera(const glm::vec3& location, const glm::vec3& forward)
		: location(location)
		, forward(forward)
	{

	}

	void Camera::MoveForward(float delta)
	{
		location += forward * delta;
	}

	void Camera::MoveBackward(float delta)
	{
		location += -forward * delta;
	}

	void Camera::MoveLeft(float delta)
	{
		glm::vec3 left = CreateLeftVector();
		location += left * delta;
	}

	void Camera::MoveRight(float delta)
	{
		glm::vec3 left = CreateLeftVector();
		location += -left * delta;
	}

	glm::mat4 Camera::CreateViewMatrix()
	{
		glm::vec3 at = location + forward;

		glm::vec3 left = CreateLeftVector();
		glm::vec3 up = glm::cross(forward, left);

		return glm::lookAt(location, at, up);
	}

	glm::vec3 Camera::CreateLeftVector()
	{
		return glm::normalize(glm::cross(location, forward));
	}
}