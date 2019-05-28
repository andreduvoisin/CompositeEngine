#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>

namespace CE
{
	Camera::Camera(const glm::vec3& location, const glm::vec3& forward, const glm::vec3& up)
		: location(location)
		, forward(glm::normalize(forward))
		, up(glm::normalize(up))
		, swivelRotationY(0.f)
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
		glm::vec3 right = CreateRightVector();
		location += right * delta;
	}

	void Camera::Swivel(int x, int y, float delta)
	{
		glm::vec3 worldUp(0, 1, 0);

		// Horizontal rotation is unlimited around the up axis.
		float rotationX = static_cast<float>(x) * delta;
		glm::quat quatX = glm::angleAxis(rotationX, worldUp);
		glm::quat conjX = glm::conjugate(quatX);
		forward = conjX * forward * quatX;
		up = conjX * up * quatX;

		// Vertical rotation is clamped to 90 degrees around the left axis.
		float rotationY = static_cast<float>(y) * delta;
		rotationY = std::clamp(
			rotationY,
			-glm::half_pi<float>() - swivelRotationY,
			glm::half_pi<float>() - swivelRotationY);
		swivelRotationY += rotationY;

		glm::vec3 left = CreateLeftVector();
		glm::quat quatY = glm::angleAxis(rotationY, left);
		glm::quat conjY = glm::conjugate(quatY);
		forward = conjY * forward * quatY;
		up = conjY * up * quatY;
	}

	glm::mat4 Camera::CreateViewMatrix()
	{
		glm::vec3 at = location + forward;
		return glm::lookAt(location, at, up);
	}

	glm::vec3 Camera::CreateRightVector()
	{
		return glm::normalize(glm::cross(forward, up));
	}

	glm::vec3 Camera::CreateLeftVector()
	{
		return -CreateRightVector();
	}
}
