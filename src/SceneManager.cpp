#include "SceneManager.h"

void SceneManager::RegisterScene(SceneData scene)
{
	Locker locker(m_Lock);
	m_Scenes.push_back(scene);
}

void SceneManager::RemoveScene(SceneFramework framework, int sceneId)
{
	m_Scenes.erase(
		std::remove_if(m_Scenes.begin(), m_Scenes.end(), [framework, sceneId](const SceneManager::SceneData& scene) {
			return scene.Framework == framework && scene.SceneId == sceneId;
		})
	);
}

std::vector<SceneManager::SceneData> SceneManager::GetAllScenes() const
{
	Locker locker(m_Lock);
	return m_Scenes;
}
