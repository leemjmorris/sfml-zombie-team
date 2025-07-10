#pragma once
#include "Scene.h"
#include "SceneOverlay.h"

class Player;
class Zombie;
class UserInterface;
class ItemGo;
class TileMap;
class Turret;
class SpriteGo;

class SceneGame : public Scene
{
protected:
	Player* player = nullptr;
	Turret* turret = nullptr;
	UserInterface* userInterface = nullptr;
	SpriteGo* ammoIcon = nullptr;
	ItemGo* item = nullptr;
	ItemGo* item2 = nullptr;
	TileMap* tileMap = nullptr;

	SceneOverlay* sceneOverlay = nullptr;

	std::list<Zombie*> zombieList;
	std::list<Zombie*> zombiePool;

	sf::Sprite cursor;

	int wave = 0;

	int score = 0;
	int currentScore = 0;

	float itemSpawnDistance = 240.f;
	float zombieSpawnDistance = 90.f;

	// LMJ: I will use this when BOSS is added. To pop-up VICTORY UI
	bool hasBoss = false;
	bool bossDefeated = false;

public:
	SceneGame();
	~SceneGame() override = default;

	void Init() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void AddScore(int points) override;
	int GetScore() const { return score; }

	void SpawnZombies(int count);
	void SpawnBossZombies();
	void SpawnItem(ItemGo* item);

	//LMJ: I will use this when BOSS is added.
	//void OnBossDefeated();

	const std::list<Zombie*>& GetZombies() const 
	{
		return zombieList;
	}
};

