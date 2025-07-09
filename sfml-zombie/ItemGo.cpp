#include "stdafx.h"
#include "ItemGo.h"
#include "Player.h"
#include "SceneGame.h"

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
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

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
			sceneGame->SpawnItem(this);
			hitBox.UpdateTransform(item, GetLocalBounds());
			if (Utils::CheckCollision(hitBox.rect, player->GetHitBox().rect))
			{
				Upgrade(type);
			}
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

void ItemGo::Upgrade(UpgradeType ty)
{
	switch (ty)
	{
	case ItemGo::UpgradeType::Heal:
		break;
	case ItemGo::UpgradeType::Ammo:
		break;
	default:
		break;
	}
}
