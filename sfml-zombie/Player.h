#pragma once
#include "GameObject.h"
#include "HitBox.h"
#include "Skill.h"

class SceneGame;
class Bullet;
class TileMap;
class ItemGo;

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

	sf::Text warningText;
	sf::Font* warningFont = nullptr;
	std::string warningMessage = "";
	float warningTimer = 0.f;  //blinker timer for the warning message.
	bool showWarning = false;
	bool isVisible = false;

	sf::Sprite body;
	std::string texId = "graphics/player.png";

	sf::Vector2f direction;
	sf::Vector2f look;
	sf::Vector2f fireOffset;

	float speed = 250.f;

	SceneGame* sceneGame = nullptr;
	TileMap* tileMap = nullptr;
	ItemGo* healItem = nullptr;
	ItemGo* ammoItem = nullptr;

	HitBox hitBox;

	Skill skill;
	float skillTimer[(int)Skill::SkillType::skillCount];

	std::list<Bullet*> bulletList;
	std::list<Bullet*> bulletPool;

	float shootInterval = 1.f;
	float shootTimer = 0.f;

	int hp = 0;
	int maxHp = 100;

	// Ammo
	int ammoUpgradeMount = 1;
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

	void Upgrade(UpgradeType type);

	int GetHp() const { return hp; }
	int GetMaxHp() const { return maxHp; }

	void AddHp(int hp) { this->hp = Utils::Clamp(this->hp + hp, 0, this->maxHp); }
	void AddAmmo(int ammo) { this->remainAmmo += ammo; }
	int GetCurrentAmmo() { return currentAmmo; }
	void SetCurrentAmmo(int ammo) { currentAmmo = ammo; }
	int GetRemainAmmo() { return remainAmmo; }

	void SetWarningMessage();
	void UpdateWarningMessage(float dt);
	void DrawWarningMessage(sf::RenderWindow& window);

	void SetSpeed(float sp) { speed = sp; }
	float GetSpeed() const { return speed; }
	void SetShootInterval(float si) { shootInterval = si; }
	float GetShootInterval() const { return shootInterval; }
};

