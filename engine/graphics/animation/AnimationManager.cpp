#include "AnimationManager.h"

#include "Animation.h"
#include "graphics/File3DImporter.h"

#include <string>

namespace CE
{
	void AnimationManager::Initialize(File3DImporter* importer)
	{
		m_importer = importer;
	}

	void AnimationManager::Destroy()
	{
		for (auto it = m_animationsMap.begin(); it != m_animationsMap.end(); ++it)
		{
			delete it->second;
		}
		m_animationsMap.clear();
	}

	Animations* AnimationManager::GetAnimations(const char* szAnimationFile, const Skeleton& skeleton)
	{
		// Try to find the animation.
		auto it = m_animationsMap.find(szAnimationFile);

		// If it exists, return it.
		if (it != m_animationsMap.end())
		{
			return it->second;
		}

		// Otherwise, attempt to load it.
		Animations* animations = new Animations();
		if (!m_importer->ExtractAnimations(szAnimationFile, skeleton, animations))
		{
			delete animations;
			return nullptr;
		}
		m_animationsMap[szAnimationFile] = animations;
		return animations;
	}
}
