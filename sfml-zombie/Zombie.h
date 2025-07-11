#pragma once
#include "GameObject.h"
#include "HitBox.h"

class Player;
class SceneGame;
class Bullet;

class Zombie : public GameObject
{
public:
	enum class Types
	{
		Bloater,
		Chaser,
		Crawler,
		Boss,
		Blood,
	};

	static const int TotalTypes = 3;
	
protected:
	int hp = 0;
	int scoreValue = 0; //LMJ : Made For UI Score Count.

	Types type = Types::Bloater;

	sf::Sprite body;
	std::string texId;

	sf::Vector2f direction;
	sf::Vector2f fireOffset;

	int maxHp = 0;
	float speed = 0.f;
	float damage = 0.f;
	float attackInterval = 0.f;

	float attackTimer = 0.f;
	float bloodTimer = 0.f;
	float bloodTimerMax = 1.f;
	
	int bossScore = 0;
	int accumulatedDamage = 0;

	std::list<Bullet*> bulletList;
	std::list<Bullet*> bulletPool;

	Player* player = nullptr;
	SceneGame* sceneGame = nullptr;
	Bullet* bullet = nullptr;
	std::list<Zombie*> zombieList;

	HitBox hitBox;
	
	sf::Color OriginColor;

public:
	Zombie(const std::string& name = "");
	virtual ~Zombie() = default;

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

	void SetType(Types type);
	sf::String GetCurrentType()
	{
		return texId;
	}

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

	void OnDamage(int damage);

	Types GetType() const { return type; }

	void Shoot();
};