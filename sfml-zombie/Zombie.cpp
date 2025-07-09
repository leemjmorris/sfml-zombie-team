#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"

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
	bullet = new Bullet();
	if (bullet != nullptr)
	{
		bullet->BulletSetType(Bullet::BulletType::bossbullet);
	}
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
	{
		sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
		
	}
	else
	{
		sceneGame = nullptr;
	}
	player = (Player*)SCENE_MGR.GetCurrentScene()->FindGameObject("Player");
	body.setTexture(TEXTURE_MGR.Get(texId), true);
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });
	for (Bullet* bullet : bulletList)
	{
		bullet->SetActive(false);
		bulletPool.push_back(bullet);
	}
	bulletList.clear();
	hp = maxHp;
	attackTimer = 0.f;
	bloodTimer = 0.f;
	bloodTimerMax = 1.f;
}

void Zombie::Update(float dt)
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

	bloodTimer += dt;

	if (texId == "graphics/blood.png" && hp > 0)
	{
		if (bloodTimer > bloodTimerMax)
		{
			hp -= 1;
			// ������ ���� ������������
			sf::Color currentColor = body.getColor();
			body.setColor(sf::Color(currentColor.r, currentColor.g, currentColor.b,
				static_cast<sf::Uint8>(currentColor.a * 0.9f)));
			bloodTimer = 0.f;
		}
	}
	else if (texId == "graphics/blood.png" && hp == 0)
	{
		sf::Color currentColor = body.getColor();
		body.setColor(sf::Color(currentColor.r, currentColor.g, currentColor.b,
			255));
		SetActive(false);
		return; // ��Ȱ��ȭ�� ����� �� �̻� ó������ ����
	}

	// ����ִ� ���� �÷��̾ �����ϰ� ����
	if (texId != "graphics/blood.png" && player != nullptr && hp > 0)
	{
		// �÷��̾� ����
		direction = Utils::GetNormal(player->GetPosition() - GetPosition());
		SetRotation(Utils::Angle(direction));
		SetPosition(GetPosition() + direction * speed * dt);

		// HitBox ������Ʈ
		hitBox.UpdateTransform(body, GetLocalBounds());

		// ���� Ÿ�̸� ������Ʈ
		attackTimer += dt;

		// ���� ������ ������, �÷��̾�� �浹���� �� ������
		if (attackTimer >= attackInterval)
		{
			if (Utils::CheckCollision(hitBox.rect, player->GetHitBox().rect))
			{
				// �÷��̾ ������� ���� ������
				if (player->IsAlive())
				{
					attackTimer = 0.f;
					player->OnDamage(static_cast<int>(damage));
				}
			}
		}
	}

	zombieList = sceneGame->GetZombies();

	bool isCollision = false;
	for (auto zombie : zombieList)
	{
		if (zombie->texId != "graphics/blood.png" && zombie != this && this->texId != "graphics/blood.png")
		{
			if (Utils::CheckCollision(hitBox.rect, zombie->GetHitBox().rect))
			{
				float distance1 = Utils::Distance(position, player->GetPosition());
				float distance2 = Utils::Distance(zombie->GetPosition(), player->GetPosition());
				if (distance1 >= distance2)
				{
					speed = 0.f;
				}
				else
				{
					zombie->speed = 0.f;
				}
				isCollision = true;
			}
		}
	}

	if (!isCollision || speed == 0.f)
	{
		SetType(type);
	}

	if (Utils::CheckCollision(hitBox.rect, player->GetHitBox().rect))
	{
		speed = 0.f;
	}
	else
	{
		if (texId == "graphics/bloater.png")
		{
			speed = 50.f;
		}
		if (texId == "graphics/chaser.png")
		{
			speed = 100.f;
		}
		if (texId == "graphics/crawler.png")
		{
			speed = 150.f;
		}
	}

	if (texId == "graphics/boss.png" && hp > 0)
	{
		if (attackTimer >= attackInterval)
		{
			Shoot();
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
		maxHp = 150;
		speed = 50.0f;
		damage = 40.0f;
		attackInterval = 1.0f;
		scoreValue = 30;
		break;
	case Types::Chaser:
		texId = "graphics/chaser.png";
		maxHp = 100;
		speed = 100.0f;
		damage = 25.0f;
		attackInterval = 1.0f;
		scoreValue = 20;
		break;
	case Types::Crawler:
		texId = "graphics/crawler.png";
		maxHp = 50;
		speed = 150.f;
		damage = 20.f;
		attackInterval = 1.f;
		scoreValue = 10;
		break;
	case Types::Boss:
		texId = "graphics/boss.png";
		maxHp = 100000000;
		speed = 100.f;
		damage = 99.f;
		attackInterval = 0.f;
		break;
	case Types::Blood:
		texId = "graphics/blood.png";
		maxHp = 10;
		speed = 0;
		damage = 0.f;
		attackInterval = 1.f;
		scoreValue = 0;
		break;
	}
}

void Zombie::OnDamage(int damage)
{
	hp = Utils::Clamp(hp - damage, 0, maxHp);

	if (hp == 0 && type != Types::Blood)
	{
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
		if (sceneGame)
		{
			sceneGame->AddScore(scoreValue);
		}

		SetType(Types::Blood);
		hp = maxHp;
		body.setTexture(TEXTURE_MGR.Get("graphics/blood.png"), true);
		sortingOrder = -1;
	}
	
	if (GetCurrentType() == "graphics/boss.png")
	{
		scoreValue = 15;
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
		if (sceneGame)
		{
			sceneGame->AddScore(scoreValue);
		}
	}
}

void Zombie::Shoot()
{
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

	sf::Transform t;
	t.rotate(GetRotation());
	sf::Vector2f worldFireOffset = t.transformPoint(fireOffset);
	bullet->Fire(position + worldFireOffset * 10.f, direction, 150.f, 20);

	bulletList.push_back(bullet);
	sceneGame->AddGameObject(bullet);
}