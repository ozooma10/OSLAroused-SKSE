#pragma once
#include "PCH.h"

using Lock = std::recursive_mutex;
using Locker = std::lock_guard<Lock>;

class SceneManager
{
public:
	enum class SceneFramework
	{
		kSexLab,
		kOStim
	};

	struct SceneData
	{
		SceneFramework Framework;
		int SceneId;
		std::vector<RE::Actor*> Participants;
	};

	static SceneManager* GetSingleton()
	{
		static SceneManager singleton;
		return &singleton;
	}

	void RegisterScene(SceneData scene);
	void RemoveScene(SceneFramework framework, int sceneId);

	std::vector<SceneData> GetAllScenes() const;

private:
	std::vector<SceneData> m_Scenes;

	mutable Lock m_Lock;
};
