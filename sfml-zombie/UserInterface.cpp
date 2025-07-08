#include "stdafx.h"
#include "UserInterface.h"

UserInterface::UserInterface()
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::Init()
{
    textScore.setFont(FONT_MGR.Get("fonts/zombiecontrol.ttf"));
    textScore.setCharacterSize(48);
    textScore.setFillColor(sf::Color::Red);
    textScore.setPosition(20.f, 20.f);
	
}

void UserInterface::Release()
{
}

void UserInterface::Reset()
{
}

void UserInterface::Update(float dt)
{
}

void UserInterface::Draw(sf::RenderWindow& window)
{
	
}

void UserInterface::SetScore(int score)
{
	textScore.setString("SCORE:" + std::to_string(score));
}

void UserInterface::SetHighScore(int highScore)
{
}

void UserInterface::SetCurrentAmmo(int currentAmmo)
{
}

void UserInterface::SetRemainAmmo(int remainAmmo)
{
}

void UserInterface::SetWaveCount(int waveCount)
{
}

void UserInterface::SetZombieCount(int zombieCount)
{
}
