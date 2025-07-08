#include "stdafx.h"
#include "ItemGo.h"
#include "Player.h"

ItemGo::ItemGo(const std::string& name)
	: GameObject(name)
{
}

void ItemGo::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	item.setPosition(pos);
}

void ItemGo::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	item.setRotation(rot);
}

void ItemGo::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	item.setScale(s);
}

void ItemGo::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	item.setOrigin(o);
}

void ItemGo::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(item, preset);
	}
}

void ItemGo::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;

	spawnInterval = 5.f;
	despawnInterval = 5.f;
}

void ItemGo::Release()
{
}

void ItemGo::Reset()
{
	player = (Player*)SCENE_MGR.GetCurrentScene()->FindGameObject("Player");

	item.setTexture(TEXTURE_MGR.Get(texId));
	SetOrigin(Origins::MC);
	SetPosition({ 0.f,0.f });
	SetRotation(0.f);
	SetScale({ 1.f,1.f });

	spawnTime = 0.f;
	despawnTime = 0.f;
	isSpawn = false;
}

void ItemGo::Update(float dt)
{
	if (!isSpawn)
	{
		spawnTime += dt;
		if (spawnTime > spawnInterval)
		{
			isSpawn = true;
			SetPosition(player->GetPosition());
			spawnTime = 0.f;
		}
	}
	else
	{
		despawnTime += dt;
		if (despawnTime > despawnInterval)
		{
			isSpawn = false;
			Reset();
		}
	}
}

void ItemGo::Draw(sf::RenderWindow& window)
{
	if(isSpawn)
	{
		window.draw(item);
		hitBox.Draw(window);
	}
}