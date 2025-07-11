#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "ItemGo.h"
#include "UserInterface.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "Turret.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	texIds.push_back("graphics/player.png");
	texIds.push_back("graphics/background_sheet.png");
	texIds.push_back("graphics/bloater.png");
	texIds.push_back("graphics/crawler.png");
	texIds.push_back("graphics/chaser.png");
	texIds.push_back("graphics/crosshair.png");
	texIds.push_back("graphics/bullet.png");
	texIds.push_back("graphics/ammo_pickup.png");
	texIds.push_back("graphics/health_pickup.png");
	texIds.push_back("graphics/turret.png");
	texIds.push_back("graphics/blood.png");
	texIds.push_back("graphics/ammo_icon.png");
	texIds.push_back("graphics/boss.png");
	texIds.push_back("graphics/bossbullet.png");

	fontIds.push_back("fonts/zombiecontrol.ttf");
	

	tileMap = (TileMap*)AddGameObject(new TileMap("TileMap"));
	player = (Player*)AddGameObject(new Player("Player"));
	turret = (Turret*)AddGameObject(new Turret("Turret"));

	for (int i = 0; i < 100; ++i)
	{
		Zombie* zombie = (Zombie*)AddGameObject(new Zombie());
		zombie->SetActive(false);
		zombiePool.push_back(zombie);
	}

	
	ammoIcon = (SpriteGo*)AddGameObject(new SpriteGo("AmmoIcon"));
	ammoIcon->sortingLayer = SortingLayers::UI;
	ammoIcon->sortingOrder = 0;
	ammoIcon->SetTextureId("graphics/ammo_icon.png");
	ammoIcon->SetScale({ 1.f, 1.f });

	item = (ItemGo*)AddGameObject(new ItemGo("AmmoPack"));
	item->SetTexId("graphics/ammo_pickup.png");
	item->SetType(ItemGo::UpgradeType::Ammo);
	item2 = (ItemGo*)AddGameObject(new ItemGo("HealPack"));
	item2->SetTexId("graphics/health_pickup.png");
	item2->SetType(ItemGo::UpgradeType::Heal);

	userInterface = (UserInterface*)AddGameObject(new UserInterface());

	sceneOverlay = (SceneOverlay*)AddGameObject(new SceneOverlay("SceneOverlay"));

	Scene::Init();
	wave = 0;
	itemSpawnDistance = 240.f;
	zombieSpawnDistance = 90.f;

}

void SceneGame::Enter()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

	worldView.setSize(windowSize);
	worldView.setCenter({ 0.f, 0.f });

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	Scene::Enter();
	if (wave == 20)
	{
		SpawnBossZombies();
		wave = 0;
		itemSpawnDistance = 240.f;
		zombieSpawnDistance = 90.f;
	}
	else if (wave < 20)
	{
		wave += 5;
		itemSpawnDistance += 30.f;
		zombieSpawnDistance += 140.f;
	}
	SpawnZombies(wave);
	

	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);
}

void SceneGame::Exit()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);

	for (Zombie* zombie : zombieList)
	{
		zombie->SetActive(false);
		zombiePool.push_back(zombie);
	}
	zombieList.clear();

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	cursor.setPosition(ScreenToUi(InputMgr::GetMousePosition()));

	// �������� ���� ó�� (�׻� ���� üũ)
	//if (sceneOverlay)
	//{
	//	// 1. GameOver üũ - �÷��̾� HP�� 0�� ��
	//	if (player && player->GetHp() <= 0 && !sceneOverlay->IsVisible())
	//	{
	//		sceneOverlay->SetScore(score);
	//		if (userInterface)
	//		{
	//			sceneOverlay->SetHighScore(userInterface->GetHighScore());
	//		}
	//		sceneOverlay->Show(SceneOverlay::OverlayType::GameOver);
	//	}

	//	// 2. Pause üũ - ESC Ű�� ������ �� (�������̰� ������ ���� ����)
	//	if (InputMgr::GetKeyDown(sf::Keyboard::Escape) && !sceneOverlay->IsVisible())
	//	{
	//		sceneOverlay->Show(SceneOverlay::OverlayType::Pause);
	//	}

	//	// SceneOverlay ������Ʈ (�׻� ������Ʈ)
	//	sceneOverlay->Update(dt);
	//}

	// �������̰� ǥ�õ��� ���� ���� ���� ������Ʈ
	/*if (!sceneOverlay || !sceneOverlay->IsVisible())
	{*/
	Scene::Update(dt);

	if (userInterface)
	{
		userInterface->SetScore(score);
		userInterface->SetZombieCount(zombieList);

		// ����ִ� ���� 0�� �Ǹ� Victory üũ
		int aliveCount = 0;
		for (auto* zombie : zombieList)
		{
			if (zombie && zombie->GetActive() && zombie->GetType() != Zombie::Types::Blood)
			{
				aliveCount++;
			}
		}

		if (aliveCount == 0)
		{
			SCENE_MGR.ChangeScene(SceneIds::Upgrade);
		}
	}

	auto it = zombieList.begin();
	while (it != zombieList.end())
	{
		if (!(*it)->GetActive())
		{
			zombiePool.push_back(*it);
			it = zombieList.erase(it);
		}
		else
		{
			++it;
		}
	}

	worldView.setCenter(player->GetPosition());

	// ���� ����� (����׿�)
	if (InputMgr::GetKeyDown(sf::Keyboard::Return))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	//}
	if (InputMgr::GetKeyDown(sf::Keyboard::T))
	{
		turret->Spawn(player->GetPosition());
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	window.setView(uiView);
	if (userInterface)
	{
		userInterface->SetScore(score);
		userInterface->Draw(window);
	}

	window.setView(uiView);
	window.draw(cursor);

	if (sceneOverlay)
	{
		sceneOverlay->Draw(window);
	}
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = nullptr;
		if (zombiePool.empty())
		{
			zombie = (Zombie*)AddGameObject(new Zombie());
			zombie->Init();
		}
		else
		{
			zombie = zombiePool.front();
			zombiePool.pop_front();
			zombie->SetActive(true);
		}
		zombie->SetType((Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes));
		zombie->Reset();
		float widthSize = tileMap->GetCellSize().x * tileMap->GetCellCount().x;
		float heightSize = tileMap->GetCellSize().y * tileMap->GetCellCount().y;
		sf::FloatRect mapSize({ -widthSize * 0.5f, -heightSize * 0.5f }, { widthSize, heightSize });
		sf::Vector2f spawnPos = player->GetPosition();
		do
		{
			sf::Vector2f unitPos = Utils::RandomOnUnitCircle();
			spawnPos = unitPos * zombieSpawnDistance + player->GetPosition();
		} while (spawnPos.x < mapSize.left + tileMap->GetCellSize().x ||
			spawnPos.x > mapSize.left + mapSize.width - tileMap->GetCellSize().x ||
			spawnPos.y < mapSize.top + tileMap->GetCellSize().y ||
			spawnPos.y > mapSize.top + mapSize.height - tileMap->GetCellSize().y);
		zombie->SetPosition(spawnPos);
		zombieList.push_back(zombie);
	}
}

void SceneGame::SpawnBossZombies()
{
	Zombie* zombie = nullptr;
	if (zombiePool.empty())
	{
		zombie = (Zombie*)AddGameObject(new Zombie());
		zombie->Init();
	}
	else
	{
		zombie = zombiePool.front();
		zombiePool.pop_front();
		zombie->SetActive(true);
	}
	zombie->SetType(Zombie::Types::Boss);
	zombie->Reset();

	float widthSize = tileMap->GetCellSize().x * tileMap->GetCellCount().x;
	float heightSize = tileMap->GetCellSize().y * tileMap->GetCellCount().y;
	sf::FloatRect mapSize({ -widthSize * 0.5f, -heightSize * 0.5f }, { widthSize, heightSize });
	sf::Vector2f spawnPos = player->GetPosition();
	do
	{
		sf::Vector2f unitPos = Utils::RandomOnUnitCircle();
		spawnPos = unitPos * zombieSpawnDistance + player->GetPosition();
	} while (spawnPos.x < mapSize.left + tileMap->GetCellSize().x ||
		spawnPos.x > mapSize.left + mapSize.width - tileMap->GetCellSize().x ||
		spawnPos.y < mapSize.top + tileMap->GetCellSize().y ||
		spawnPos.y > mapSize.top + mapSize.height - tileMap->GetCellSize().y);
	zombie->SetPosition(spawnPos);
	zombieList.push_back(zombie);
}

void SceneGame::SpawnItem(ItemGo* item)
{
	float widthSize = tileMap->GetCellSize().x * tileMap->GetCellCount().x;
	float heightSize = tileMap->GetCellSize().y * tileMap->GetCellCount().y;
	sf::FloatRect mapSize({ -widthSize * 0.5f, -heightSize * 0.5f }, { widthSize, heightSize });
	sf::Vector2f spawnPos = player->GetPosition();
	do
	{
		sf::Vector2f unitPos = Utils::RandomOnUnitCircle();
		spawnPos = unitPos * itemSpawnDistance + player->GetPosition();
	} while (spawnPos.x < mapSize.left + tileMap->GetCellSize().x ||
		spawnPos.x > mapSize.left + mapSize.width - tileMap->GetCellSize().x ||
		spawnPos.y < mapSize.top + tileMap->GetCellSize().y ||
		spawnPos.y > mapSize.top + mapSize.height - tileMap->GetCellSize().y);
	item->SetPosition(spawnPos);
}

void SceneGame::AddScore(int points)
{
	score += points;

	if (userInterface)
	{
		userInterface->SetScore(score);
	}

	std::cout << "Score updated: " << score << std::endl;
}

