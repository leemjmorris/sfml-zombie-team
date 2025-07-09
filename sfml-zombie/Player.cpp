#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "TileMap.h"
#include "ItemGo.h"

Player::Player(const std::string& name)
	: GameObject(name)
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	body.setPosition(pos);
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	body.setRotation(rot);
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	body.setScale(s);
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	body.setOrigin(o);
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(body, preset);
	}
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	fireOffset = { 2.f, 1.1f }; //LMJ : Bullet Offset to make bullet fire from the gun.
	//SetOrigin(Origins::MC);

	ammoMax = 30;
	currentAmmo = 6 * ammoUpgradeMount;
	remainAmmo = ammoMax - currentAmmo;
}

void Player::Release()
{
}

void Player::Reset()
{
	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
	{
		sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
		tileMap = (TileMap*)sceneGame->FindGameObject("TileMap");
		healItem = (ItemGo*)sceneGame->FindGameObject("AmmoPack");
		ammoItem = (ItemGo*)sceneGame->FindGameObject("HealPack");
	}
	else
	{
		sceneGame = nullptr;
	}

	for (Bullet* bullet : bulletList)
	{
		bullet->SetActive(false);
		bulletPool.push_back(bullet);
	}
	bulletList.clear();
	
	body.setTexture(TEXTURE_MGR.Get(texId), true);
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);

	direction = { 0.f, 0.f };
	look = { 1.0f, 0.f };

	shootTimer = 0.f;
	hp = maxHp;

	// player upgrade at enter scene
	Upgrade((UpgradeType)SCENE_MGR.GetPlayerUpgradeType());
}

void Player::Update(float dt)
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
	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	if (Utils::Magnitude(direction) > 1.f)
	{
		Utils::Normalize(direction);
	}
	SetPosition(position + direction * speed * dt);

	float widthSize = tileMap->GetCellSize().x * tileMap->GetCellCount().x;
	float heightSize = tileMap->GetCellSize().y * tileMap->GetCellCount().y;
	sf::FloatRect mapSize({ -widthSize * 0.5f, -heightSize * 0.5f }, { widthSize, heightSize });
	sf::Vector2f pos = GetPosition();
	pos.x = Utils::Clamp(pos.x, mapSize.left + tileMap->GetCellSize().x, mapSize.left + widthSize - tileMap->GetCellSize().x);
	pos.y = Utils::Clamp(pos.y, mapSize.top + tileMap->GetCellSize().y, mapSize.top + heightSize - tileMap->GetCellSize().y);
	SetPosition(pos);


	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseWorldPos = sceneGame->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseWorldPos - GetPosition());
	SetRotation(Utils::Angle(look));


	hitBox.UpdateTransform(body, GetLocalBounds());


	shootTimer += dt;
	if (InputMgr::GetMouseButton(sf::Mouse::Left) && shootTimer > shootInterval)
	{
		shootTimer = 0.f;
		Shoot();
	}	
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
}

void Player::Shoot()
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
		
	sf::Transform t; //LMJ : Changed bullet->Fire position. added FireOffset.
	t.rotate(GetRotation());
	sf::Vector2f worldFireOffset = t.transformPoint(fireOffset);
	bullet->Fire(position + worldFireOffset * 10.f, look, 1000.f, 50);

	bulletList.push_back(bullet);
	sceneGame->AddGameObject(bullet);
}

void Player::OnDamage(int damage) 
{
	if (!IsAlive())
		return;

	hp = Utils::Clamp(hp - damage, 0, maxHp);

	if (hp == 0)
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
}

void Player::Upgrade(UpgradeType type)
{
	switch (type)
	{
	case Player::UpgradeType::FireRate:
		shootInterval *= 0.2f;
		break;
	case Player::UpgradeType::ClipSize:
		ammoUpgradeMount += 1;
		break;
	case Player::UpgradeType::MaxHP:
		maxHp += 50;
		break;
	case Player::UpgradeType::Speed:
		speed += 5.f;
		break;
	case Player::UpgradeType::HealthPickUp:
		healItem->Upgrade(ItemGo::UpgradeType::Heal);
		break;
	case Player::UpgradeType::AmmoPickUp:
		ammoItem->Upgrade(ItemGo::UpgradeType::Ammo);
		break;
	default:
		break;
	}
}
