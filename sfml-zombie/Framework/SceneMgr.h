#pragma once
class SceneMgr : public Singleton<SceneMgr>
{
	friend class Singleton<SceneMgr>;

protected:
	SceneMgr() = default;
	~SceneMgr() = default;

	std::unordered_map<SceneIds, Scene*> scenes;

	SceneIds startScene = SceneIds::Title;
	SceneIds currentScene = SceneIds::None;
	SceneIds nextScene = SceneIds::None;

	int playerUpgrade = -1;

public:
	void Init();
	void Release();

	Scene* GetCurrentScene() { return scenes[currentScene]; }
	SceneIds GetCurrentSceneId() const { return currentScene; }
	void ChangeScene(SceneIds id);

	void Update(float dt);
	void Draw(sf::RenderWindow& window);

	int GetPlayerUpgradeType() const { return playerUpgrade; }
	void SetPlayerUpgradeType(int ty){ playerUpgrade = ty; }
};

#define SCENE_MGR (SceneMgr::Instance())

