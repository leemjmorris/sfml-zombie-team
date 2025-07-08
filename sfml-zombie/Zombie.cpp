#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"

Zombie::Zombie(const std::string& name)
	: GameObject(name)
{
}

void Zombie::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Zombie::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Zombie::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Zombie::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Zombie::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Zombie::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	SetType(type);
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	player = (Player*)SCENE_MGR.GetCurrentScene()->FindGameObject("Player");

	body.setTexture(TEXTURE_MGR.Get(texId), true);
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });

	hp = maxHp;
	attackTimer = 0.f;
}

void Zombie::Update(float dt)
{
	direction = Utils::GetNormal(player->GetPosition() - GetPosition());

	if (direction.x > 0.f) //LMJ: changed rotation to scale.
	{
		SetScale({1.f, 1.f});
	}
	else
	{
		SetScale({ -1.f, 1.f });
	}

	SetPosition(GetPosition() + direction * speed * dt);

	hitBox.UpdateTransform(body, GetLocalBounds());

	attackTimer += dt;
	if (attackTimer > attackInterval)
	{
		if (Utils::CheckCollision(hitBox.rect, player->GetHitBox().rect))
		{
			attackTimer = 0.f;
			player->OnDamage(damage);
		}
	}
}

void Zombie::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void Zombie::SetType(Types type)
{
	this->type = type;
	switch (this->type)
	{
	case Types::Bloater:
		texId = "graphics/bloater.png";
		maxHp = 200;
		speed = 50.0f;                    // float으로 명시적 지정
		damage = 100.0f;                  // float으로 명시적 지정
		attackInterval = 1.0f;            // float으로 명시적 지정
		scoreValue = 50;
		break;
	case Types::Chaser:
		texId = "graphics/chaser.png";
		maxHp = 100;
		speed = 100.0f;
		damage = 100.0f;
		attackInterval = 1.0f;
		scoreValue = 30;
		break;
	case Types::Crawler:
		texId = "graphics/crawler.png";
		maxHp = 50;
		speed = 200.0f;
		damage = 100.0f;
		attackInterval = 1.0f;
		scoreValue = 20;
		break;
	}
}

void Zombie::OnDamage(int damage)
{
	hp = Utils::Clamp(hp - damage, 0, maxHp);
	if (hp == 0)
	{
		// Scene을 통해 점수 추가
		Scene* currentScene = SCENE_MGR.GetCurrentScene();
		if (currentScene)
		{
			currentScene->AddScore(scoreValue);
		}

		SetActive(false);
	}
}

