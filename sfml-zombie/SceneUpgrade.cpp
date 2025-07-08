#include "stdafx.h"
#include "SceneUpgrade.h"
#include "SpriteGo.h"
#include "MenuUI.h"
#include "Player.h"

SceneUpgrade::SceneUpgrade()
	: Scene(SceneIds::Upgrade)
{
}

void SceneUpgrade::Init()
{
	texIds.push_back("graphics/background.png");
	fontIds.push_back("fonts/zombiecontrol.ttf");

	sf::FloatRect bounds = FRAMEWORK.GetWindowBounds();

	AddGameObject(new SpriteGo("graphics/background.png"));

	menuUi = (MenuUI*)AddGameObject(new MenuUI("menuUI"));
	menuUi->SetFontId("fonts/zombiecontrol.ttf");

	menuUi->AddMessages("Increased Rate of Fire");
	menuUi->AddMessages("Increased Clip Size(Next Reload)");
	menuUi->AddMessages("Increased Max Health");
	menuUi->AddMessages("Increased Run Speed");
	menuUi->AddMessages("More and Better Health Pickups");
	menuUi->AddMessages("More and Better Ammo Pickups");

	for (int i = 0; i < menuUi->GetMenuCount(); i++)
	{
		menuUi->AddTextPos({bounds.width * 0.5f, (i+1) * 100.f});
		TextGo text;
		text.SetCharacterSize(40);
		text.SetFillColor(sf::Color::White);
		text.SetOrigin(Origins::MC);
		text.SetPosition(menuUi->GetTextPos()[i]);
		menuUi->AddTextGo(text);
	}

	menuUi->SetChooseBarColor(sf::Color(0, 0, 0, 0));
	menuUi->SetChooseBarOutColor(sf::Color::Yellow);
	menuUi->SetChooseBarThickness(5.f);
	menuUi->SetChooseBarSize({ 650.f, 80.f });
	menuUi->SetChoosedColor(sf::Color::Yellow);
	menuUi->SetNotChoosedColor(sf::Color::White);
	menuUi->SetChooseBarOffset(sf::Vector2f{ 0.f, 15.f });

	Scene::Init();
}

void SceneUpgrade::Enter()
{
	sf::Vector2f windows = FRAMEWORK.GetWindowSizeF();

	uiView.setCenter(windows * 0.5f);
	uiView.setSize(windows);

	Scene::Enter();
}

void SceneUpgrade::Update(float dt)
{
	Scene::Update(dt);

	int choosedPos = -1;
	bool isChoosed = false;
	std::vector<TextGo> texts = menuUi->GetTexts();

	for (int i = 0; i < menuUi->GetMenuCount(); i++)
	{
		if (Utils::PointInTransformBounds(texts[i].GetText(), texts[i].GetLocalBounds(), (sf::Vector2f)InputMgr::GetMousePosition()))
		{
			std::cout << i << std::endl;
			menuUi->SetBarPos(i);
			if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
			{
				choosedPos = i;
				isChoosed = true;
			}
		}
	}
	
	if (isChoosed)
	{
		player.Upgrade((Player::UpgradeType)choosedPos);
	}
}