#include "stdafx.h"
#include "Bullet.h"
#include "SceneGame.h"
#include "Zombie.h"

Bullet::Bullet(const std::string& name)
	: GameObject(name)
{
}

void Bullet::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Bullet::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Bullet::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Bullet::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Bullet::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Bullet::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Bullet::Release()
{
}

void Bullet::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(texId), true);
	SetOrigin(Origins::ML);

	SetPosition({ 0.f, 0.f }); 
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });

	direction = { 0.f, 0.f };
	speed = 0;
	damage = 0;
}

void Bullet::Update(float dt)
{
	SetPosition(position + direction * speed * dt);
	hitBox.UpdateTransform(body, GetLocalBounds());

	const auto& list = sceneGame->GetZombies();
	for (const auto zombie : list)
	{
		if (Utils::CheckCollision(hitBox.rect, zombie->GetHitBox().rect))
		{
			if (zombie->GetCurrentType() != "graphics/blood.png")
			{
				SetActive(false);
				zombie->OnDamage(damage);
			}
			else
			{
				SetActive(true);
			}
			break;
		}
	}
}

void Bullet::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void Bullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float s, int d)
{
	SetPosition(pos);
	direction = dir;
	speed = s;
	damage = d;

	SetRotation(Utils::Angle(direction));
}
