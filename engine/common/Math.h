#ifndef _CE_MATH_H_
#define _CE_MATH_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace CE
{
	glm::mat4 ToAffineMatrix(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale);
	glm::vec3 LerpTranslation(const glm::vec3& low, const glm::vec3& high, float alpha);
	glm::quat LerpRotation(const glm::quat& low, const glm::quat& high, float alpha);
	glm::vec3 LerpScale(const glm::vec3& low, const glm::vec3& high, float alpha);
	glm::vec3 Vec3Lerp(const glm::vec3& a, const glm::vec3& b, float alpha);
}

#endif // _CE_MATH_H_
