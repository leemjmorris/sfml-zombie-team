#pragma once
#include "GameObject.h"
class UserInterface :  public GameObject
{
protected:
	sf::Font uiFont;

	sf::Text textScore;
	sf::Text textHighScore;
	sf::Text textCurrentAmmo;
	sf::Text textRemainAmmo;
	sf::Text textZombieCount;
	sf::Text textWaveCount;

public:
	UserInterface();
	~UserInterface() override;

	bool isGameOver = false;
	bool showGameOver = false;

	void Init() override;

	void Release() override;

	void Reset() override;

	void Update(float dt) override;

	void Draw(sf::RenderWindow& window) override;

	void SetScore(int score);
	void SetHighScore(int highScore);
	void SetCurrentAmmo(int currentAmmo);
	void SetRemainAmmo(int remainAmmo);
	void SetWaveCount(int waveCount);
	void SetZombieCount(int zombieCount);
	

};

