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
    // UI 텍스트들 (TextGo 포인터로 관리)
    TextGo* textScore;         // 현재 점수
    TextGo* textHighScore;     // 최고 점수
    TextGo* textAmmo;          // 탄약 정보 (합쳐서 표시)
    TextGo* textWaveCount;     // 현재 Wave
    TextGo* textZombieCount;   // 남은 Zombie 수

    // 체력바 관련
    sf::RectangleShape healthBarBackground;  // 체력바 배경
    sf::RectangleShape healthBarForeground;  // 체력바 전경

    // Player 참조
    Player* player;

    // 최고 점수 관리
    int highScore;
    std::string highScoreFilePath;

    // 탄약 정보 저장
    int currentAmmo;    // 현재 탄창 탄약
    int remainAmmo;     // 전체 탄약

    // UI 설정 함수들
    void CreateTextObjects();   // TextGo 객체들 생성
    void SetupTextPositions();
    void SetupHealthBar();
    void UpdateHealthBar();
    void UpdateAmmoDisplay();

    // 최고 점수 파일 관리
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

    // 데이터 설정 함수들
    void SetScore(int score);
    void SetHighScore(int highScore);
    void SetCurrentAmmo(int currentAmmo);
    void SetRemainAmmo(int remainAmmo);
    void SetWaveCount(int waveCount);
    void SetZombieCount(const std::list<Zombie*>& zombieList);

    // TextGo 객체들 접근 함수 (SceneGame에서 Scene에 추가하기 위해)
    TextGo* GetScoreText() const { return textScore; }
    TextGo* GetHighScoreText() const { return textHighScore; }
    TextGo* GetAmmoText() const { return textAmmo; }
    TextGo* GetWaveCountText() const { return textWaveCount; }
    TextGo* GetZombieCountText() const { return textZombieCount; }

    int GetRemainZombie() { return remainZombie; }
};
