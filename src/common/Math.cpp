#include "Math.h"

namespace CE
{
	glm::mat4 ToAffineMatrix(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
	{
		const float xx = rotation.x * rotation.x;
		const float xy = rotation.x * rotation.y;
		const float xz = rotation.x * rotation.z;
		const float xw = rotation.x * rotation.w;
		const float yy = rotation.y * rotation.y;
		const float yz = rotation.y * rotation.z;
		const float yw = rotation.y * rotation.w;
		const float zz = rotation.z * rotation.z;
		const float zw = rotation.z * rotation.w;

		return glm::mat4(
			scale.x * (1.f - 2.f * (yy + zz)), scale.x * 2.f * (xy + zw), scale.x * 2.f * (xz - yw), 0.f,
			scale.y * 2.f * (xy - zw), scale.y * (1.f - 2.f * (xx + zz)), scale.y * 2.f * (yz + xw), 0.f,
			scale.z * 2.f * (xz + yw), scale.z * 2.f * (yz - xw), scale.z * (1.f - 2.f * (xx + yy)), 0.f,
			translation.x, translation.y, translation.z, 1.f);
	}

	glm::vec3 LerpTranslation(const glm::vec3& low, const glm::vec3& high, float alpha)
	{
		return Vec3Lerp(low, high, alpha);
	}

	glm::quat LerpRotation(const glm::quat& low, const glm::quat& high, float alpha)
	{
		// shortest path; -high and high are same rotation
		// TODO: This could be done at import.
		const float dot = glm::dot(low, high);
		return glm::normalize(glm::lerp(low, dot < 0.f ? -high : high, alpha));
		//return glm::slerp(low, high, alpha);
	}

	glm::vec3 LerpScale(const glm::vec3& low, const glm::vec3& high, float alpha)
	{
		return Vec3Lerp(low, high, alpha);
	}

	glm::vec3 Vec3Lerp(const glm::vec3& a, const glm::vec3& b, float alpha)
	{
		return glm::vec3(
			(b.x - a.x) * alpha + a.x,
			(b.y - a.y) * alpha + a.y,
			(b.z - a.z) * alpha + a.z);
	}
}
