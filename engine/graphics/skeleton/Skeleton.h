#ifndef _CE_SKELETON_H_
#define _CE_SKELETON_H_

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace CE
{
	struct Joint
	{
		glm::mat4 inverseBindPose;
		std::string name;
		short parentIndex;
	};

	struct Skeleton
	{
		std::vector<Joint> joints;
	};
}

#endif // _CE_SKELETON_H_
