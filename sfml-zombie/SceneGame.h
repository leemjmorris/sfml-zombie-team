#pragma once
#include "Scene.h"

class Player;
class Zombie;
class ItemGo;

class SceneGame : public Scene
{
protected:
	Player* player = nullptr;
	ItemGo* item = nullptr;

	std::list<Zombie*> zombieList;
	std::list<Zombie*> zombiePool;

	sf::Sprite cursor;

public:
	SceneGame();
	~SceneGame() override = default;

	void Init() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SpawnZombies(int count);

	const std::list<Zombie*>& GetZombies() const 
	{
		return zombieList;
	}
};

