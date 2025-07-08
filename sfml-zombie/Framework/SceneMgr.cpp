#include "stdafx.h"
#include "SceneMgr.h"
#include "SceneDev1.h"
#include "SceneDev2.h"
#include "SceneGame.h"
#include "SceneUpgrade.h"

void SceneMgr::Init()
{
	scenes.insert({ SceneIds::Game, new SceneGame() });
	scenes.insert({ SceneIds::Upgrade, new SceneUpgrade() });
	scenes.insert({ SceneIds::Dev1, new SceneDev1() });
	scenes.insert({ SceneIds::Dev2, new SceneDev2() });

	for (auto& pair : scenes)
	{
		pair.second->Init();
	}

	currentScene = startScene;
	scenes[currentScene]->Enter();
}

void SceneMgr::Release()
{
	for (auto scene : scenes)
	{
		if (scene.second->Id == currentScene)
		{
			scene.second->Exit();
		}
		scene.second->Release();
		delete scene.second;
	}
	scenes.clear();
}

void SceneMgr::ChangeScene(SceneIds id)
{
	nextScene = id;
}

void SceneMgr::Update(float dt)
{
	if (nextScene != SceneIds::None)
	{
		scenes[currentScene]->Exit();
		currentScene = nextScene;
		nextScene = SceneIds::None;
		scenes[currentScene]->Enter();
	}

	scenes[currentScene]->Update(dt);
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	scenes[currentScene]->Draw(window);
}
