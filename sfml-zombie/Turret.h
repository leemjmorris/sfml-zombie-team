#pragma once
#include "GameObject.h"
#include "Zombie.h"

class Bullet;
class SceneGame;

class Turret :
    public GameObject
{
protected:
	sf::Sprite body;
	std::string texId = "graphics/turret.png";

	sf::Vector2f look;

	float shootInterval = 1.f;
	float shootTime = 0.f;

	std::list<Bullet*> bulletList;
	std::list<Bullet*> bulletPool;
	SceneGame* sceneGame = nullptr;

	float attackRange = 200.f;
	int spawnCount = 1;

	bool isSpawn = false;

public:
	Turret(const std::string& name = "");
	virtual ~Turret() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Spawn(const sf::Vector2f& pos);
	Zombie* FindClosestZombie();
};

