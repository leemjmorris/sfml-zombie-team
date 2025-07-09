#pragma once
#include "GameObject.h"
#include <string.h>
#include <iostream>


class Player;
class TextGo;
class Zombie;

class UserInterface : public GameObject
{
protected:
    // UI �ؽ�Ʈ�� (TextGo �����ͷ� ����)
    TextGo* textScore;         // ���� ����
    TextGo* textHighScore;     // �ְ� ����
    TextGo* textAmmo;          // ź�� ���� (���ļ� ǥ��)
    TextGo* textWaveCount;     // ���� Wave
    TextGo* textZombieCount;   // ���� Zombie ��

    // ü�¹� ����
    sf::RectangleShape healthBarBackground;  // ü�¹� ���
    sf::RectangleShape healthBarForeground;  // ü�¹� ����

    // Player ����
    Player* player;

    // �ְ� ���� ����
    int highScore;
    std::string highScoreFilePath;

    // ź�� ���� ����
    int currentAmmo;    // ���� źâ ź��
    int remainAmmo;     // ��ü ź��

    // UI ���� �Լ���
    void CreateTextObjects();   // TextGo ��ü�� ����
    void SetupTextPositions();
    void SetupHealthBar();
    void UpdateHealthBar();
    void UpdateAmmoDisplay();

    // �ְ� ���� ���� ����
    void LoadHighScore();
    void SaveHighScore();
    void UpdateHighScore(int currentScore);
    int remainZombie;

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

    // ������ ���� �Լ���
    void SetScore(int score);
    void SetHighScore(int highScore);
    void SetCurrentAmmo(int currentAmmo);
    void SetRemainAmmo(int remainAmmo);
    void SetWaveCount(int waveCount);

    // TextGo ��ü�� ���� �Լ� (SceneGame���� Scene�� �߰��ϱ� ����)
    TextGo* GetScoreText() const { return textScore; }
    TextGo* GetHighScoreText() const { return textHighScore; }
    TextGo* GetAmmoText() const { return textAmmo; }
    TextGo* GetWaveCountText() const { return textWaveCount; }
    TextGo* GetZombieCountText() const { return textZombieCount; }


    void SetZombieCount(const std::list<Zombie*>& zombieList);
    int GetRemainZombie() { return remainZombie; }
};
