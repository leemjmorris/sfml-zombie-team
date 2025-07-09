#pragma once
#include "GameObject.h"
#include <string.h>
#include <iostream>


class Player;
class TextGo;
class Zombie;
class SceneGame;

class UserInterface : public GameObject
{
protected:
    TextGo* textScore;
    TextGo* textHighScore;
    TextGo* textAmmo;
    TextGo* textWaveCount;
    TextGo* textZombieCount;

    sf::Sprite ammoImage;
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;

    std::string texId;

    Player* player;

    int highScore;
    std::string highScoreFilePath;

    int currentAmmo;
    int remainAmmo;

    void CreateTextObjects();
    void SetupTextPositions();
    void SetupAmmoImage();
    void SetupHealthBar();
    void UpdateHealthBar();
    void UpdateAmmoDisplay();

    void LoadHighScore();
    void SaveHighScore();
    void UpdateHighScore(int currentScore);
    int remainZombie;

public:
    UserInterface(const std::string& name = "");
    ~UserInterface() override;

    bool isGameOver = false;
    bool showGameOver = false;

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    void SetTexId(const std::string& id) { texId = id; }
    void SetPosition(const sf::Vector2f& pos) override;
    void SetRotation(float rot) override;
    void SetScale(const sf::Vector2f& s) override;
    void SetOrigin(const sf::Vector2f& o) override;
    void SetOrigin(Origins preset) override;

    void SetScore(int score);
    void SetHighScore(int highScore);
    void SetCurrentAmmo(int currentAmmo);
    void SetRemainAmmo(int remainAmmo);
    void SetWaveCount(int waveCount);

    TextGo* GetScoreText() const { return textScore; }
    TextGo* GetHighScoreText() const { return textHighScore; }
    TextGo* GetAmmoText() const { return textAmmo; }
    TextGo* GetWaveCountText() const { return textWaveCount; }
    TextGo* GetZombieCountText() const { return textZombieCount; }


    void SetZombieCount(const std::list<Zombie*>& zombieList);
    int GetRemainZombie() { return remainZombie; }
};
