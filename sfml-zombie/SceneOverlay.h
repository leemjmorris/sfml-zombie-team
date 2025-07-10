#pragma once
#include "GameObject.h"

class TextGo;

class SceneOverlay : public GameObject
{
public:
    enum class OverlayType
    {
        GameOver,
        Pause,
        Victory
    };

protected:
    // �������� ���
    sf::RectangleShape background;

    // GAME OVER UI ��ҵ�
    TextGo* titleText = nullptr;
    TextGo* currentScoreText = nullptr;
    TextGo* highScoreText = nullptr;
    TextGo* instructionText = nullptr;
    TextGo* retryText = nullptr;
    TextGo* exitText = nullptr;

    // Color diff
    sf::Color titleColor = sf::Color::Red;
    sf::Color currentScoreColor = sf::Color::White;
    sf::Color highScoreColor = sf::Color::Yellow;
    sf::Color instructionColor = sf::Color(200, 200, 200);
    sf::Color retryColor = sf::Color::Green;
    sf::Color exitColor = sf::Color::Red;
    sf::Color returnColor = sf::Color::Green;

    // PAUSE UI ��ҵ� (�Ϻ� ����)
    TextGo* returnText = nullptr;

    // �������� ����
    OverlayType currentType = OverlayType::GameOver;
    bool isVisible = false;
    bool isActive = false;

    // ������
    int currentScore = 0;
    int highScore = 0;
    int currentWave = 1;

    // �Է� ó��
    float inputCooldown = 0.0f;
    const float inputDelay = 0.3f;

    // �ִϸ��̼�
    float fadeTimer = 0.0f;
    float fadeDuration = 0.5f;
    bool isFadingIn = true;

public:
    SceneOverlay(const std::string& name = "SceneOverlay");
    virtual ~SceneOverlay() = default;

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    // �������� ����
    void Show(OverlayType type);
    void Hide();
    bool IsVisible() const { return isVisible; }
    bool IsActive() const { return isActive; }

    // ������ ����
    void SetScore(int score) { currentScore = score; }
    void SetHighScore(int score) { highScore = score; }
    void SetWave(int wave) { currentWave = wave; }

private:
    void CreateTextObjects();
    void SetupGameOverUI();
    void SetupPauseUI();
    void SetupVictoryUI();
    void UpdateFadeEffect(float dt);
    void HandleInput();
};