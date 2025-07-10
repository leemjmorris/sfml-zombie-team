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
	SetWarningMessage();

	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	fireOffset = { 2.f, 1.1f };
	currentAmmo = 6 * ammoUpgradeMount;
	remainAmmo = 30;
	skill.SetPlayer(this);
}

void Player::Release()
{
}

void Player::Reset()
{
	showWarning = false;
	warningTimer = 0.f;
	SetWarningMessage();

	if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
	{
		sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
		tileMap = (TileMap*)sceneGame->FindGameObject("TileMap");
		healItem = (ItemGo*)sceneGame->FindGameObject("HealPack");
		ammoItem = (ItemGo*)sceneGame->FindGameObject("AmmoPack");
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

	for (int i = 0; i < (int) Skill::SkillType::skillCount; i++)
	{
		skillTimer[i] = skill.GetCoolTime((Skill::SkillType)i) + skill.GetUseTime((Skill::SkillType)i);
	}
	skill.Reset();

	shootTimer = 0.f;
	hp = maxHp;

	Upgrade((UpgradeType)SCENE_MGR.GetPlayerUpgradeType());
}

void Player::Update(float dt)
{
	UpdateWarningMessage(dt);

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

	if (currentAmmo > 0)
	{
		showWarning = false;

		if (InputMgr::GetMouseButton(sf::Mouse::Left) && shootTimer > shootInterval)
		{
			shootTimer = 0.f;
			Shoot();
			currentAmmo--;
		}
	}
	else if (remainAmmo == 0)
	{
		showWarning = true;
		warningTimer = 0.f;
		warningMessage = "No Ammo Left!";
		warningText.setString(warningMessage);
		warningText.setFillColor(sf::Color::Red);

		if (InputMgr::GetKeyDown(sf::Keyboard::R))
		{
			currentAmmo = 0;
		}
	}
	else if (currentAmmo == 0)
	{
		showWarning = true;
		warningMessage = "Press R to Reload!";
		warningText.setString(warningMessage);
		warningText.setFillColor(sf::Color::Yellow);

		if (InputMgr::GetKeyDown(sf::Keyboard::R))
		{
			currentAmmo = 6 * ammoUpgradeMount;

			if (remainAmmo >= currentAmmo)
			{
				remainAmmo -= currentAmmo;
			}
			else
			{
				int sum = currentAmmo + remainAmmo;
				if (sum > currentAmmo)
				{
					currentAmmo = 6 * ammoUpgradeMount;
				}
			}
			showWarning = false;
			warningTimer = 0.f;
		}
	}

	for(int i = 0; i < (int)Skill::SkillType::skillCount; i++)
	{
		skillTimer[i] += dt;
		if (skillTimer[i] > skill.GetUseTime((Skill::SkillType) i) && !skill.GetCanUse((Skill::SkillType) i))
		{
			skill.FinishSkill((Skill::SkillType) i);
		}
	}

	Skill::SkillType useType = Skill::SkillType::None;
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		useType = Skill::SkillType::Dash;
		
	}
	else if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
	{
		useType = Skill::SkillType::FastShoot;
	}
	if(useType != Skill::SkillType::None)
	{
		if (skill.GetCanUse(useType) && !skill.IsCoolTime(skillTimer[(int)useType], useType))
		{
			skill.Use(useType);
			skillTimer[(int)useType] = 0.f;
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitBox.Draw(window);
	DrawWarningMessage(window);
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

void Player::SetWarningMessage()
{
	warningFont = &FONT_MGR.Get("fonts/zombiecontrol.ttf");

	warningText.setFont(*warningFont);
	warningText.setCharacterSize(36);
	warningText.setFillColor(sf::Color::Red);
	warningText.setStyle(sf::Text::Bold);

	warningMessage = "Press R to Reload!";
	warningText.setString(warningMessage);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	Utils::SetOrigin(warningText, Origins::TC);
	warningText.setPosition(windowSize.x * 0.5f, windowSize.y * 0.2f);
}

void Player::UpdateWarningMessage(float dt)
{
    if (showWarning)
    {
        warningTimer += dt;
        
        bool isVisible = fmod(warningTimer, 1.0f) < 0.5f;
        sf::Color color = warningText.getFillColor();
        color.a = isVisible ? 255 : 0;
        warningText.setFillColor(color);
    }
}

void Player::DrawWarningMessage(sf::RenderWindow& window)
{
	if (showWarning)
	{
		sf::View currentView = window.getView();
		sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
		sf::View uiView;
		uiView.setSize(windowSize);
		uiView.setCenter(windowSize * 0.5f);
		window.setView(uiView);

		window.draw(warningText);

		window.setView(currentView);
	}
}
