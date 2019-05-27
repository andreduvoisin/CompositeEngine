#ifndef _CE_CAMERA_H_
#define _CE_CAMERA_H_

#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec3.hpp"

namespace CE
{
	class Camera
	{
	public:
		Camera(const glm::vec3& location, const glm::vec3& forward, const glm::vec3& up);

		void MoveForward(float delta);
		void MoveBackward(float delta);
		void MoveLeft(float delta);
		void MoveRight(float delta);

		glm::mat4 CreateViewMatrix();

	private:
		glm::vec3 CreateLeftVector();

		glm::vec3 location;
		glm::vec3 forward;
		glm::vec3 up;
	};
}

#endif // _CE_CAMERA_H_
