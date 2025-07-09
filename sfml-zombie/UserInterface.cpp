#include "stdafx.h"
#include "UserInterface.h"
#include "Player.h"
#include "Zombie.h"
#include "SceneMgr.h"
#include "TextGo.h"
#include <fstream>
#include <iostream>

UserInterface::UserInterface()
    : GameObject("UserInterface")
    , textScore(nullptr)
    , textHighScore(nullptr)
    , textAmmo(nullptr)
    , textWaveCount(nullptr)
    , textZombieCount(nullptr)
    , player(nullptr)
    , highScore(0)
    , highScoreFilePath("highscore.txt")
    , currentAmmo(0)
    , remainAmmo(0)
{
}

UserInterface::~UserInterface()
{
}

void UserInterface::Init()
{
    sortingLayer = SortingLayers::UI;
    sortingOrder = 10;

    CreateTextObjects();
    SetupTextPositions();
    SetupHealthBar();

    LoadHighScore();
}

void UserInterface::Release()
{
    SaveHighScore();

    textScore = nullptr;
    textAmmo = nullptr;
    textWaveCount = nullptr;
    textZombieCount = nullptr;
}

void UserInterface::Reset()
{
    // Player 참조 가져오기
    if (SCENE_MGR.GetCurrentSceneId() == SceneIds::Game)
    {
        player = (Player*)SCENE_MGR.GetCurrentScene()->FindGameObject("Player");
    }
    else
    {
        player = nullptr;
    }

    textScore->Reset();
    textHighScore->Reset();
    textAmmo->Reset();
    textWaveCount->Reset();
    textZombieCount->Reset();

    // 위치 재설정
    SetupTextPositions();
    SetupHealthBar();

    // 초기 텍스트 설정
    SetScore(0);
    SetHighScore(highScore);
    SetCurrentAmmo(0);
    SetRemainAmmo(0);
    SetWaveCount(1);
}

void UserInterface::Update(float dt)
{
    // Game 씬에서만 업데이트
    if (SCENE_MGR.GetCurrentSceneId() != SceneIds::Game)
        return;

    UpdateHealthBar();

    // TextGo 객체들도 업데이트
    if (textScore) textScore->Update(dt);
    if (textHighScore) textHighScore->Update(dt);
    if (textAmmo) textAmmo->Update(dt);
    if (textWaveCount) textWaveCount->Update(dt);
    if (textZombieCount) textZombieCount->Update(dt);
}

void UserInterface::Draw(sf::RenderWindow& window)
{
    // Game 씬에서만 그리기
    if (SCENE_MGR.GetCurrentSceneId() != SceneIds::Game)
        return;

    // UI View 설정
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::View view = FRAMEWORK.GetWindow().getView();
    //sf::View uiView;
    /*uiView.setSize(windowSize);
    uiView.setCenter(windowSize * 0.5f);*/
    window.setView(view);

    // 모든 TextGo 직접 그리기
    if (textScore) textScore->Draw(window);
    if (textHighScore) textHighScore->Draw(window);
    if (textAmmo) textAmmo->Draw(window);
    if (textWaveCount) textWaveCount->Draw(window);
    if (textZombieCount) textZombieCount->Draw(window);

    // 체력바만 직접 그리기 (TextGo들은 Scene에서 자동으로 그려짐)
    window.draw(healthBarBackground);
    window.draw(healthBarForeground);
}

void UserInterface::CreateTextObjects()
{

    textScore = new TextGo("fonts/zombiecontrol.ttf", "UI_Score");    
    textHighScore = new TextGo("fonts/zombiecontrol.ttf", "UI_HighScore");    
    textAmmo = new TextGo("fonts/zombiecontrol.ttf", "UI_Ammo");    
    textWaveCount = new TextGo("fonts/zombiecontrol.ttf", "UI_Wave");
    textZombieCount = new TextGo("fonts/zombiecontrol.ttf", "UI_Zombie");

    // 텍스트 기본 설정
    textScore->SetCharacterSize(36);
    textScore->SetFillColor(sf::Color::Yellow);
    textScore->SetString("SCORE: 0");

    textHighScore->SetCharacterSize(36);
    textHighScore->SetFillColor(sf::Color::Cyan);
    textHighScore->SetString("HIGH: 0");

    textAmmo->SetCharacterSize(32);
    textAmmo->SetFillColor(sf::Color::White);
    textAmmo->SetString("0/0");

    textWaveCount->SetCharacterSize(32);
    textWaveCount->SetFillColor(sf::Color::Green);
    textWaveCount->SetString("WAVE: 1");

    textZombieCount->SetCharacterSize(32);
    textZombieCount->SetFillColor(sf::Color::Red);
    textZombieCount->SetString("ZOMBIES: 0");

    // 모든 TextGo를 UI 레이어로 설정
    textScore->sortingLayer = SortingLayers::UI;
    textScore->sortingOrder = 10;

    textHighScore->sortingLayer = SortingLayers::UI;
    textHighScore->sortingOrder = 10;

    textAmmo->sortingLayer = SortingLayers::UI;
    textAmmo->sortingOrder = 10;

    textWaveCount->sortingLayer = SortingLayers::UI;
    textWaveCount->sortingOrder = 10;

    textZombieCount->sortingLayer = SortingLayers::UI;
    textZombieCount->sortingOrder = 10;
}

void UserInterface::SetupTextPositions()
{
    if (!textScore || !textHighScore || !textAmmo || !textWaveCount || !textZombieCount)
        return;

    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

    // 좌측 상단: 현재 점수 (10, 10)
    textScore->SetPosition(sf::Vector2f(10.0f, 10.0f));

    // 우측 상단: 최고 점수 (임시 위치, SetHighScore에서 정확히 조정)
    textHighScore->SetPosition(sf::Vector2f(windowSize.x - 200.0f, 10.0f));

    // 좌측 하단: 탄약 정보
    textAmmo->SetPosition(sf::Vector2f(10.0f, windowSize.y - 80.0f));

    // 우측 하단: Wave 정보 (임시 위치, SetWaveCount에서 정확히 조정)
    textWaveCount->SetPosition(sf::Vector2f(windowSize.x - 150.0f, windowSize.y - 80.0f));

    // Wave 옆: Zombie 수 (임시 위치, SetZombieCount에서 정확히 조정)
    textZombieCount->SetPosition(sf::Vector2f(windowSize.x - 300.0f, windowSize.y - 40.0f));
}

void UserInterface::SetupHealthBar()
{
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

    // 체력바 크기 및 위치 (탄약 정보 바로 위)
    sf::Vector2f healthBarSize(200.0f, 20.0f);
    sf::Vector2f healthBarPos(10.0f, windowSize.y - 110.0f);

    // 배경 (검은색)
    healthBarBackground.setSize(healthBarSize);
    healthBarBackground.setPosition(healthBarPos);
    healthBarBackground.setFillColor(sf::Color::Black);
    healthBarBackground.setOutlineThickness(2.0f);
    healthBarBackground.setOutlineColor(sf::Color::White);

    // 전경 (초록색)
    healthBarForeground.setSize(healthBarSize);
    healthBarForeground.setPosition(healthBarPos);
    healthBarForeground.setFillColor(sf::Color::Green);
}

void UserInterface::UpdateHealthBar()
{
    if (!player)
        return;

    // Player 클래스의 GetHp()와 GetMaxHp() 메서드 사용
    float currentHp = static_cast<float>(player->GetHp());
    float maxHp = static_cast<float>(player->GetMaxHp());

    if (maxHp <= 0.0f) return;

    // 체력 비율 계산
    float healthRatio = currentHp / maxHp;
    healthRatio = std::max(0.0f, std::min(1.0f, healthRatio));

    // 체력바 크기 조정
    sf::Vector2f fullSize = healthBarBackground.getSize();
    sf::Vector2f currentSize(fullSize.x * healthRatio, fullSize.y);
    healthBarForeground.setSize(currentSize);

    // 체력에 따른 색상 변경
    if (healthRatio > 0.6f)
        healthBarForeground.setFillColor(sf::Color::Green);
    else if (healthRatio > 0.3f)
        healthBarForeground.setFillColor(sf::Color::Yellow);
    else
        healthBarForeground.setFillColor(sf::Color::Red);
}

void UserInterface::LoadHighScore()
{
    std::ifstream file(highScoreFilePath);
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
    else
    {
        highScore = 0;
        SaveHighScore();
    }
}

void UserInterface::SaveHighScore()
{
    std::ofstream file(highScoreFilePath);
    if (file.is_open())
    {
        file << highScore;
        file.close();
    }
}

void UserInterface::UpdateHighScore(int currentScore)
{
    if (currentScore > highScore)
    {
        highScore = currentScore;
        SaveHighScore();
        SetHighScore(highScore);
    }
}

void UserInterface::SetScore(int score)
{
    if (!textScore) return;

    textScore->SetString("SCORE: " + std::to_string(score));

    // 새로운 점수가 최고 점수를 넘는지 확인
    UpdateHighScore(score);
}

void UserInterface::SetHighScore(int highScore)
{
    if (!textHighScore) return;

    textHighScore->SetString("HIGH: " + std::to_string(highScore));

    // 우측 정렬을 위해 위치 재조정
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::FloatRect bounds = textHighScore->GetLocalBounds();
    textHighScore->SetPosition(sf::Vector2f(windowSize.x - bounds.width - 10.0f, 10.0f));
    textHighScore->SetOrigin(Origins::TL);
}

void UserInterface::SetCurrentAmmo(int currentAmmo)
{
    this->currentAmmo = currentAmmo;
    UpdateAmmoDisplay();
}

void UserInterface::SetRemainAmmo(int remainAmmo)
{
    this->remainAmmo = remainAmmo;
    UpdateAmmoDisplay();
}

void UserInterface::UpdateAmmoDisplay()
{
    if (!textAmmo) return;

    // "6/24" 형태로 표시
    textAmmo->SetString(std::to_string(currentAmmo) + "/" + std::to_string(remainAmmo));
}

void UserInterface::SetWaveCount(int waveCount)
{
    if (!textWaveCount) return;

    textWaveCount->SetString("WAVE: " + std::to_string(waveCount));

    // 우측 정렬을 위해 위치 재조정
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::FloatRect bounds = textWaveCount->GetLocalBounds();
    textWaveCount->SetPosition(sf::Vector2f(windowSize.x - bounds.width - 30.0f, windowSize.y - 80.0f));
    textWaveCount->SetOrigin(Origins::TL);
}


void UserInterface::SetZombieCount(const std::list<Zombie*>& zombieList)
{
    remainZombie = 0;
    for (auto zombie : zombieList)
    {
        if (zombie->GetCurrentType() != "graphics/blood.png")
        {
            remainZombie++;
        }
    }
    if (!textZombieCount) return;

    textZombieCount->SetString("ZOMBIES: " + std::to_string(remainZombie));

    // Wave 텍스트 옆에 위치 조정
    if (textWaveCount)
    {
        sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
        sf::FloatRect waveBounds = textWaveCount->GetLocalBounds();
        sf::FloatRect zombieBounds = textZombieCount->GetLocalBounds();
        textZombieCount->SetPosition(sf::Vector2f(windowSize.x - waveBounds.width - zombieBounds.width + 80.0f, windowSize.y - 40.0f));
        textZombieCount->SetOrigin(Origins::TL);
    }
}