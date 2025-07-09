#include "stdafx.h"
#include "Turret.h"
#include "Zombie.h"
#include "SceneGame.h"

Turret::Turret(const std::string& name)
	: GameObject(name)
{
}

void Turret::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Turret::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Turret::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Turret::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Turret::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Turret::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;
	SetOrigin(Origins::MC);

	attackRange = 200.f;
}

void Turret::Release()
{
}

void Turret::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(texId));
	SetRotation(0.f);
	SetPosition({0.f,0.f});
	SetScale({0.3f,0.3f});

	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
	{
		sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
	}
	else
	{
		sceneGame = nullptr;
	}
}

void Turret::Update(float dt)
{
	auto zombies = sceneGame->GetZombies();
	for (auto zombie : zombies)
	{
		sf::Vector2f zombiePos = zombie->GetPosition();
	}
}

void Turret::Draw(sf::RenderWindow& window)
{
	if(isSpawn)
	{
		window.draw(body);
	}
}

void Turret::Spawn(const sf::Vector2f& pos)
{
	if(spawnCount > 0)
	{
		SetPosition(pos);
		isSpawn = true;
		spawnCount--;
	}
}
