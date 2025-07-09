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
	hp = maxHp;
	attackTimer = 0.f;
	bloodTimer = 0.f;
	bloodTimerMax = 1.f;
}

void Zombie::Update(float dt)
{
	bloodTimer += dt;

	if (texId == "graphics/blood.png" && hp > 0)
	{
		if (bloodTimer > bloodTimerMax)
		{
			hp -= 1;
			// 혈흔이 점점 투명해지도록
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
		return; // 비활성화된 좀비는 더 이상 처리하지 않음
	}

	// 살아있는 좀비만 플레이어를 추적하고 공격
	if (texId != "graphics/blood.png" && player != nullptr && hp > 0)
	{
		// 플레이어 추적
		direction = Utils::GetNormal(player->GetPosition() - GetPosition());
		SetRotation(Utils::Angle(direction));
		SetPosition(GetPosition() + direction * speed * dt);

		// HitBox 업데이트
		hitBox.UpdateTransform(body, GetLocalBounds());

		// 공격 타이머 업데이트
		attackTimer += dt;

		// 공격 간격이 지났고, 플레이어와 충돌했을 때 데미지
		if (attackTimer >= attackInterval)
		{
			if (Utils::CheckCollision(hitBox.rect, player->GetHitBox().rect))
			{
				// 플레이어가 살아있을 때만 데미지
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
		speed = 50.0f;                    // float으로 명시적 지정
		damage = 40.0f;                  // float으로 명시적 지정
		attackInterval = 1.0f;            // float으로 명시적 지정
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
		speed = 0.f;
		damage = 100.f;
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
		scoreValue += 15;
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
		if (sceneGame)
		{
			sceneGame->AddScore(scoreValue);
		}
	}
}

