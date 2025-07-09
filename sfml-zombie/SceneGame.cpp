#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "ItemGo.h"
#include "UserInterface.h"
#include "TextGo.h"

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
	fontIds.push_back("fonts/zombiecontrol.ttf");
	texIds.push_back("graphics/blood.png");

	tileMap = (TileMap*)AddGameObject(new TileMap("TileMap"));
	player = (Player*)AddGameObject(new Player("Player"));

	for (int i = 0; i < 100; ++i)
	{
		Zombie* zombie = (Zombie*)AddGameObject(new Zombie());
		zombie->SetActive(false);
		zombiePool.push_back(zombie);
	}

	item = (ItemGo*)AddGameObject(new ItemGo("AmmoPack"));
	item->SetTexId("graphics/ammo_pickup.png");
	item->SetType(ItemGo::UpgradeType::Ammo);
	item2 = (ItemGo*)AddGameObject(new ItemGo("HealPack"));
	item2->SetTexId("graphics/health_pickup.png");
	item2->SetType(ItemGo::UpgradeType::Heal);

	userInterface = (UserInterface*)AddGameObject(new UserInterface());

	Scene::Init();
	wave = 0;
}

void SceneGame::Enter()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

	worldView.setSize(windowSize);
	worldView.setCenter({0.f, 0.f});

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	itemSpawnDistance = 200.f;

	Scene::Enter();
	if (wave == 20)
	{
		wave = 0;
	}
	else if (wave < 20)
	{
		wave += 5;
	}
	SpawnZombies(wave);

	score = 0; //LMJ : Updated for the UI making
	if (userInterface)
	{
		userInterface->SetScore(0);
	}
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

	Scene::Update(dt);

	// UI ������Ʈ
	if (userInterface)
	{
		userInterface->SetScore(score);
		userInterface->SetZombieCount(static_cast<int>(zombieList.size()));
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

	if (InputMgr::GetKeyDown(sf::Keyboard::Return))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}

	if (userInterface)
	{
		userInterface->SetScore(score);
		userInterface->SetZombieCount(static_cast<int>(zombieList.size()));
	}

	//userInterface->SetScore(score);
	////userInterface->SetWaveCount(currentWave); LMJ: Will Add this when Wave is merged together.
	//userInterface->SetZombieCount(zombieList.size());
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	window.setView(uiView);
	userInterface->SetScore(score);

	window.setView(uiView);
	window.draw(cursor);

	if (userInterface)
	{
		userInterface->Draw(window);
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
		zombie->SetPosition(Utils::RandomInUnitCircle() * 500.f);
		zombieList.push_back(zombie);
	}
}

void SceneGame::SpawnItem(ItemGo* item)
{
	float widthSize = tileMap->GetCellSize().x * tileMap->GetCellCount().x;
	float heightSize = tileMap->GetCellSize().y * tileMap->GetCellCount().y;
	sf::FloatRect mapSize( {-widthSize * 0.5f, -heightSize * 0.5f}, { widthSize, heightSize });
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

	// UserInterface�� ���� ������Ʈ
	if (userInterface)
	{
		userInterface->SetScore(score);
	}

	std::cout << "Score updated: " << score << std::endl;
}

