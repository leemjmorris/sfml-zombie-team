#pragma once
#include "GameObject.h"
#include "HitBox.h"

class SceneGame;
class Player;

class Bullet : public GameObject
{
public:
	enum class BulletType
	{
		bullet,
		bossbullet,
	};

protected:
	BulletType type = BulletType::bullet;
	sf::Sprite body;
	std::string texId;
	sf::Vector2f direction;
	float speed = 0.f;
	int damage = 0;

	HitBox hitBox;
	Player* player;

	SceneGame* sceneGame = nullptr;



public:
	Bullet(const std::string& name = "");
	virtual ~Bullet() = default;

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

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float s, int d);
	void BulletSetType(BulletType type);

	std::string BulletGetType()
	{
		return texId;
	}
};

