#include "SceneManager.h"

void SceneManager::RegisterScene(SceneData scene)
{
	Locker locker(m_Lock);
	m_Scenes.push_back(scene);
}

void SceneManager::RemoveScene(SceneFramework framework, int sceneId)
{
	Locker locker(m_Lock);
	auto scenesToRemove = std::remove_if(m_Scenes.begin(), m_Scenes.end(), [framework, sceneId](const SceneManager::SceneData& scene) {
		return scene.Framework == framework && scene.SceneId == sceneId;
	});
	if (scenesToRemove != m_Scenes.end()) {
		m_Scenes.erase(
			scenesToRemove
		);
	}
}

void SceneManager::ClearScenes()
{
	Locker locker(m_Lock);
	m_Scenes.clear();
}

std::vector<SceneManager::SceneData> SceneManager::GetAllScenes() const
{
	Locker locker(m_Lock);
	return m_Scenes;
}
