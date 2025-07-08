#pragma once
#include "GameObject.h"
#include "HitBox.h"

class SceneGame;
class Bullet;

class Player : public GameObject
{
public:
	// player Upgrade type enum class
	enum class UpgradeType
	{
		Not = -1,
		FireRate,
		ClipSize,
		MaxHP,
		Speed,
		HealthPickUp,
		AmmoPickUp,
	};

protected:
	sf::Sprite body;
	std::string texId = "graphics/player.png";

	sf::Vector2f direction;
	sf::Vector2f look;

	float speed = 500.f;

	SceneGame* sceneGame = nullptr;

	HitBox hitBox;

	std::list<Bullet*> bulletList;
	std::list<Bullet*> bulletPool;

	float shootInterval = 2.f;
	float shootTimer = 0.f;

	int hp = 0;
	int maxHp = 100;

	// Ammo
	int ammoUpgradeMount = 1;
	int ammoMax = 0;
	int currentAmmo = 0;
	int remainAmmo = 0;

public:
	bool IsAlive() const { return hp > 0; }

	Player(const std::string& name = "");
	~Player() override = default;

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

	sf::FloatRect GetLocalBounds() const override
	{
		return body.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		return body.getGlobalBounds();
	}

	const HitBox& GetHitBox() const
	{
		return hitBox;
	}

	void Shoot();
	void OnDamage(int damage);

	// player upgrade function
	void Upgrade(UpgradeType type);
};

