#pragma once
#include "GameObject.h"
#include "HitBox.h"

class Player;
class SceneGame;

class ItemGo :
    public GameObject
{
protected:
	sf::Sprite item;
	std::string texId;

	float spawnInterval = 0.f;
	float despawnInterval = 0.f;
	float spawnTime = 0.f;
	float despawnTime = 0.f;

	bool isSpawn = false;

	HitBox hitBox;
	Player* player;
	SceneGame* sceneGame = nullptr;

public:
	ItemGo(const std::string& name = "");
	virtual ~ItemGo() = default;

	void SetTexId(const std::string& id) { texId = id; }

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
		return item.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		return item.getGlobalBounds();
	}

	void Upgrade();
};

