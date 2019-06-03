#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace CE
{
	// TODO: Add this to a constants file.
	glm::vec3 WORLD_UP(0, 1, 0);

	Camera::Camera(const glm::vec3& location, const glm::vec3& forward, const glm::vec3& up)
		: location(location)
		, forward(glm::normalize(forward))
		, up(glm::normalize(up))
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

	void Camera::MoveUp(float delta)
	{
		location += up * delta;
	}

	void Camera::Swivel(int x, int y, float delta)
	{
		SwivelHorizontal(x, delta);
		SwivelVertical(y, delta);
	}

	glm::mat4 Camera::CreateViewMatrix()
	{
		glm::vec3 at = location + forward;
		return glm::lookAt(location, at, up);
	}

	// Horizontal rotation is unlimited around the up axis.
	void Camera::SwivelHorizontal(int x, float delta)
	{
		float rotationX = static_cast<float>(x) * delta;
		glm::quat quatX = glm::angleAxis(rotationX, WORLD_UP);
		glm::quat conjX = glm::conjugate(quatX);
		forward = conjX * forward * quatX;
		up = conjX * up * quatX;
	}

	// Vertical rotation is clamped to the up vector always having the
	// magnitude of its component in the direction of WORLD_UP being >= 0.
	//
	// This implementation breaks if rotationY is so great that it flips
	// the camera 180 degrees. Realistically, that should never happen.
	void Camera::SwivelVertical(int y, float delta)
	{
		float rotationY = static_cast<float>(y) * delta;
		glm::vec3 left = CreateLeftVector();
		glm::quat quatY = glm::angleAxis(rotationY, left);
		glm::quat conjY = glm::conjugate(quatY);

		glm::vec3 transformedUp = conjY * up * quatY;

		if (glm::dot(transformedUp, WORLD_UP) >= 0)
		{
			// If we're within our clamp post-transformation, perform the transform.
			forward = conjY * forward * quatY;
			up = transformedUp;
		}
		else
		{
			// Otherwise, clamp to our maximum values.
			float direction = glm::dot(forward, WORLD_UP) > 0.f ? 1.f : -1.f;
			forward = WORLD_UP * direction;
			up = glm::normalize(glm::cross(forward, left));
		}
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
