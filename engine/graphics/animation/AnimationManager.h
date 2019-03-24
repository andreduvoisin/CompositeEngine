#ifndef _CE_ANIMATION_MANAGER_H_
#define _CE_ANIMATION_MANAGER_H_

#include "common/Singleton.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace CE
{
	struct Skeleton;
	struct Animation;
	typedef std::vector<Animation> Animations;
	class File3DImporter;

	class AnimationManager : public Singleton<AnimationManager>
	{
	public:
		void Initialize(File3DImporter* importer);
		void Destroy();

		Animations* GetAnimations(const char* szAnimationFile, const Skeleton& skeleton);

	private:
		File3DImporter* m_importer;
		std::unordered_map<std::string, Animations*> m_animationsMap;
	};
}

#endif // _CE_ANIMATION_MANAGER_H_
