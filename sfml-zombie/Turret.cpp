#include "stdafx.h"
#include "Turret.h"
#include "SceneGame.h"
#include "Bullet.h"

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

	attackRange = 200.f;
}

void Turret::Release()
{
}

void Turret::Reset()
{
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
	{
		sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
	}
	else
	{
		sceneGame = nullptr;
	}

	body.setTexture(TEXTURE_MGR.Get(texId),true);
	SetOrigin(Origins::MC);
	SetRotation(0.f);
	SetPosition({0.f,0.f});
	SetScale({0.3f,0.3f});
	
	for (Bullet* bullet : bulletList)
	{
		bullet->SetActive(false);
		bulletPool.push_back(bullet);
	}
	bulletList.clear();
}

void Turret::Update(float dt)
{
	auto it = bulletList.begin();
	while (it != bulletList.end())
	{
		if (!(*it)->GetActive())
		{
			bulletPool.push_back(*it);
			it = bulletList.erase(it);
		}
		else
		{
			++it;
		}
	}

	if(isSpawn)
	{
		Zombie* zombie = FindClosestZombie();
		if(zombie != nullptr)
		{
			float distance = Utils::Distance(position, zombie->GetPosition());
			shootTime += dt;
			if (distance < attackRange && shootTime > shootInterval)
			{
				shootTime = 0.f;
				Bullet* bullet = nullptr;
				if (bulletPool.empty())
				{
					bullet = new Bullet();
					bullet->Init();
				}
				else
				{
					bullet = bulletPool.front();
					bulletPool.pop_front();
					bullet->SetActive(true);
				}

				bullet->Reset();

				look = Utils::GetNormal(zombie->GetPosition() - position);
				SetRotation(Utils::Angle(look));
				bullet->Fire(position + look * 20.f, look, 100.f, 50.f);

				bulletList.push_back(bullet);
				sceneGame->AddGameObject(bullet);
			}
		}
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

Zombie* Turret::FindClosestZombie()
{
	auto zombies = sceneGame->GetZombies();
	if(zombies.size() > 0)
	{
		float minDistance = (zombies.front()->GetCurrentType() !="graphics/blood.png") ? Utils::Distance(position, zombies.front()->GetPosition()) : 10000.f;
		Zombie* closeZombie = (zombies.front()->GetCurrentType() != "graphics/blood.png") ? zombies.front() : nullptr;
		for (auto zombie : zombies)
		{
			if (zombie->GetCurrentType() != "graphics/blood.png")
			{
				sf::Vector2f zombiePos = zombie->GetPosition();
				float distance = Utils::Distance(position, zombiePos);
				if (distance < minDistance)
				{
					minDistance = distance;
					closeZombie = zombie;
				}
			}
		}
		return closeZombie;
	}
	else
	{
		return nullptr;
	}

}
