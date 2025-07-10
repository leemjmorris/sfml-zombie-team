#include "stdafx.h"
#include "SceneOverlay.h"
#include "TextGo.h"
#include "SceneMgr.h"

SceneOverlay::SceneOverlay(const std::string& name)
    : GameObject(name)
{
}

void SceneOverlay::Init()
{
    sortingLayer = SortingLayers::UI;
    sortingOrder = 100; // �ֻ����� ǥ��

    // ��� ����
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    background.setSize(windowSize);
    background.setFillColor(sf::Color(0, 0, 0, 0)); // �ʱ⿡�� ����
    background.setPosition(0, 0);

    // TextGo ��ü�� ����
    CreateTextObjects();
}

void SceneOverlay::Release()
{
    // TextGo ��ü���� ���� ����
    if (titleText)
    {
        delete titleText;
        titleText = nullptr;
    }
    if (currentScoreText)
    {
        delete currentScoreText;
        currentScoreText = nullptr;
    }
    if (highScoreText)
    {
        delete highScoreText;
        highScoreText = nullptr;
    }
    if (instructionText)
    {
        delete instructionText;
        instructionText = nullptr;
    }
    if (retryText)
    {
        delete retryText;
        retryText = nullptr;
    }
    if (exitText)
    {
        delete exitText;
        exitText = nullptr;
    }
    if (returnText)
    {
        delete returnText;
        returnText = nullptr;
    }
}

void SceneOverlay::Reset()
{
    isVisible = false;
    isActive = false;
    fadeTimer = 0.0f;
    inputCooldown = 0.0f;
    isFadingIn = true;

    // ��� �����ϰ� ����
    background.setFillColor(sf::Color(0, 0, 0, 0));
}

void SceneOverlay::CreateTextObjects()
{
    // TextGo ��ü���� Scene�� �߰����� �ʰ� ���� ����
    titleText = new TextGo("fonts/zombiecontrol.ttf", "OverlayTitle");
    currentScoreText = new TextGo("fonts/zombiecontrol.ttf", "OverlayCurrentScore");
    highScoreText = new TextGo("fonts/zombiecontrol.ttf", "OverlayHighScore");
    instructionText = new TextGo("fonts/zombiecontrol.ttf", "OverlayInstruction");
    retryText = new TextGo("fonts/zombiecontrol.ttf", "OverlayRetry");
    exitText = new TextGo("fonts/zombiecontrol.ttf", "OverlayExit");
    returnText = new TextGo("fonts/zombiecontrol.ttf", "OverlayReturn");

    // TextGo���� Init() ���� ȣ��
    titleText->Init();
    currentScoreText->Init();
    highScoreText->Init();
    instructionText->Init();
    retryText->Init();
    exitText->Init();
    returnText->Init();

    // UI ���̾� ����
    titleText->sortingLayer = SortingLayers::UI;
    titleText->sortingOrder = 101;
    currentScoreText->sortingLayer = SortingLayers::UI;
    currentScoreText->sortingOrder = 101;
    highScoreText->sortingLayer = SortingLayers::UI;
    highScoreText->sortingOrder = 101;
    instructionText->sortingLayer = SortingLayers::UI;
    instructionText->sortingOrder = 101;
    retryText->sortingLayer = SortingLayers::UI;
    retryText->sortingOrder = 101;
    exitText->sortingLayer = SortingLayers::UI;
    exitText->sortingOrder = 101;
    returnText->sortingLayer = SortingLayers::UI;
    returnText->sortingOrder = 101;
}

void SceneOverlay::Update(float dt)
{
    if (!isVisible) return;

    UpdateFadeEffect(dt);

    if (isActive)
    {
        inputCooldown += dt;
        HandleInput();
    }
}

void SceneOverlay::UpdateFadeEffect(float dt)
{
    fadeTimer += dt;

    if (isFadingIn)
    {
        // ���̵� ��
        float progress = fadeTimer / fadeDuration;
        progress = std::min(progress, 1.0f);

        sf::Uint8 alpha = static_cast<sf::Uint8>(progress * 180); // �ִ� 180 ���İ�
        background.setFillColor(sf::Color(0, 0, 0, alpha));

        // �ؽ�Ʈ ���İ��� ����
        sf::Uint8 textAlpha = static_cast<sf::Uint8>(progress * 255);

        // �� �ؽ�Ʈ�� Ȱ��ȭ�Ǿ� ���� ���� ���İ� ����
        if (titleText && titleText->GetActive())
        {
            sf::Color color = titleColor;
            color.a = textAlpha;
            titleText->SetFillColor(color);
        }
        if (currentScoreText && currentScoreText->GetActive())
        {
            sf::Color color = currentScoreColor;
            color.a = textAlpha;
            currentScoreText->SetFillColor(color);
        }
        if (highScoreText && highScoreText->GetActive())
        {
            sf::Color color = highScoreColor;
            color.a = textAlpha;
            highScoreText->SetFillColor(color);
        }
        if (instructionText && instructionText->GetActive())
        {
            sf::Color color = instructionColor;
            color.a = textAlpha;
            instructionText->SetFillColor(color);
        }
        if (retryText && retryText->GetActive())
        {
            sf::Color color = retryColor;
            color.a = textAlpha;
            retryText->SetFillColor(color);
        }
        if (exitText && exitText->GetActive())
        {
            sf::Color color = exitColor;
            color.a = textAlpha;
            exitText->SetFillColor(color);
        }
        if (returnText && returnText->GetActive())
        {
            sf::Color color = returnColor;
            color.a = textAlpha;
            returnText->SetFillColor(color);
        }

        if (progress >= 1.0f)
        {
            isActive = true; // ���̵� �� �Ϸ� �� �Է� Ȱ��ȭ
        }
    }
}

void SceneOverlay::HandleInput()
{
    if (inputCooldown < inputDelay) return;

    switch (currentType)
    {
    case OverlayType::GameOver:
        if (InputMgr::GetKeyDown(sf::Keyboard::R))
        {
            // ���� �����
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Game);
        }
        else if (InputMgr::GetKeyDown(sf::Keyboard::Q) || InputMgr::GetKeyDown(sf::Keyboard::Escape))
        {
            // ���� �޴���
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Title);
        }
        break;

    case OverlayType::Pause:
        if (InputMgr::GetKeyDown(sf::Keyboard::Escape) || InputMgr::GetKeyDown(sf::Keyboard::P))
        {
            // ���� �簳
            Hide();
        }
        else if (InputMgr::GetKeyDown(sf::Keyboard::Q))
        {
            // ���� �޴���
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Title);
        }
        break;

    case OverlayType::Victory:
        if (InputMgr::GetKeyDown(sf::Keyboard::Space) || InputMgr::GetKeyDown(sf::Keyboard::Enter))
        {
            // ���� ���̺� �Ǵ� ���׷��̵� ������
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Upgrade);
        }
        break;
    }
}

void SceneOverlay::Draw(sf::RenderWindow& window)
{
    if (!isVisible) return;

    // UI �� ����
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::View uiView;
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize * 0.5f);
    window.setView(uiView);

    // ������ ��� �׸���
    window.draw(background);

    // TextGo���� ���� �׸���
    if (titleText && titleText->GetActive()) titleText->Draw(window);
    if (currentScoreText && currentScoreText->GetActive()) currentScoreText->Draw(window);
    if (highScoreText && highScoreText->GetActive()) highScoreText->Draw(window);
    if (instructionText && instructionText->GetActive()) instructionText->Draw(window);
    if (retryText && retryText->GetActive()) retryText->Draw(window);
    if (exitText && exitText->GetActive()) exitText->Draw(window);
    if (returnText && returnText->GetActive()) returnText->Draw(window);
}

void SceneOverlay::Show(OverlayType type)
{
    currentType = type;
    isVisible = true;
    isActive = false; // ���̵� �� �߿��� �Է� ��Ȱ��ȭ
    fadeTimer = 0.0f;
    inputCooldown = 0.0f;
    isFadingIn = true;

    // Ÿ�Կ� ���� UI ����
    switch (type)
    {
    case OverlayType::GameOver:
        SetupGameOverUI();
        break;
    case OverlayType::Pause:
        SetupPauseUI();
        break;
    case OverlayType::Victory:
        SetupVictoryUI();
        break;
    }
}

void SceneOverlay::Hide()
{
    isVisible = false;
    isActive = false;

    // ��� �ؽ�Ʈ �����
    if (titleText) titleText->SetActive(false);
    if (currentScoreText) currentScoreText->SetActive(false);
    if (highScoreText) highScoreText->SetActive(false);
    if (instructionText) instructionText->SetActive(false);
    if (retryText) retryText->SetActive(false);
    if (exitText) exitText->SetActive(false);
    if (returnText) returnText->SetActive(false);
}

void SceneOverlay::SetupGameOverUI()
{
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2f center = windowSize * 0.5f;

    // GAME OVER UI ��ҵ� Ȱ��ȭ
    if (titleText) titleText->SetActive(true);
    if (currentScoreText) currentScoreText->SetActive(true);
    if (highScoreText) highScoreText->SetActive(true);
    if (instructionText) instructionText->SetActive(true);
    if (retryText) retryText->SetActive(true);
    if (exitText) exitText->SetActive(true);

    // PAUSE UI ��ҵ� ��Ȱ��ȭ
    if (returnText) returnText->SetActive(false);

    // GAME OVER ����
    titleText->SetCharacterSize(72);
    titleColor = sf::Color(255, 0, 0); // ������ ����
    titleText->SetFillColor(sf::Color(255, 0, 0, 0)); // �ʱ⿡�� ����
    titleText->SetString("GAME OVER");
    titleText->SetPosition(sf::Vector2f(center.x, center.y - 150.0f));
    titleText->SetOrigin(Origins::MC);

    // ���� ����
    currentScoreText->SetCharacterSize(36);
    currentScoreColor = sf::Color(255, 255, 255); // ��� ����
    currentScoreText->SetFillColor(sf::Color(255, 255, 255, 0));
    currentScoreText->SetString("SCORE: " + std::to_string(currentScore));
    currentScoreText->SetPosition(sf::Vector2f(center.x, center.y - 80.0f));
    currentScoreText->SetOrigin(Origins::MC);

    // �ְ� ����
    highScoreText->SetCharacterSize(32);
    highScoreColor = sf::Color(255, 255, 0); // ����� ����
    highScoreText->SetFillColor(sf::Color(255, 255, 0, 0));
    highScoreText->SetString("HIGH SCORE: " + std::to_string(highScore));
    highScoreText->SetPosition(sf::Vector2f(center.x, center.y - 30.0f));
    highScoreText->SetOrigin(Origins::MC);

    // �ȳ���
    instructionText->SetCharacterSize(24);
    instructionColor = sf::Color(200, 200, 200); // ȸ�� ����
    instructionText->SetFillColor(sf::Color(200, 200, 200, 0));
    instructionText->SetString("Choose an option:");
    instructionText->SetPosition(sf::Vector2f(center.x, center.y + 40.0f));
    instructionText->SetOrigin(Origins::MC);

    // ��õ� �ؽ�Ʈ
    retryText->SetCharacterSize(28);
    retryColor = sf::Color(0, 255, 0); // �ʷϻ� ����
    retryText->SetFillColor(sf::Color(0, 255, 0, 0));
    retryText->SetString("Press R to Retry");
    retryText->SetPosition(sf::Vector2f(center.x, center.y + 90.0f));
    retryText->SetOrigin(Origins::MC);

    // ���� �ؽ�Ʈ
    exitText->SetCharacterSize(28);
    exitColor = sf::Color(255, 0, 0); // ������ ����
    exitText->SetFillColor(sf::Color(255, 0, 0, 0));
    exitText->SetString("Press Q to Exit");
    exitText->SetPosition(sf::Vector2f(center.x, center.y + 130.0f));
    exitText->SetOrigin(Origins::MC);
}

void SceneOverlay::SetupPauseUI()
{
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2f center = windowSize * 0.5f;

    // PAUSE UI ��ҵ� Ȱ��ȭ
    if (titleText) titleText->SetActive(true);
    if (instructionText) instructionText->SetActive(true);
    if (returnText) returnText->SetActive(true);
    if (exitText) exitText->SetActive(true);

    // GAME OVER UI ��ҵ� ��Ȱ��ȭ
    if (currentScoreText) currentScoreText->SetActive(false);
    if (highScoreText) highScoreText->SetActive(false);
    if (retryText) retryText->SetActive(false);

    // PAUSED ����
    titleText->SetCharacterSize(64);
    titleColor = sf::Color(255, 255, 0); // ����� ����
    titleText->SetFillColor(sf::Color(255, 255, 0, 0));
    titleText->SetString("PAUSED");
    titleText->SetPosition(sf::Vector2f(center.x, center.y - 100.0f));
    titleText->SetOrigin(Origins::MC);

    // �ȳ���
    instructionText->SetCharacterSize(24);
    instructionColor = sf::Color(200, 200, 200); // ȸ�� ����
    instructionText->SetFillColor(sf::Color(200, 200, 200, 0));
    instructionText->SetString("Game is paused");
    instructionText->SetPosition(sf::Vector2f(center.x, center.y - 20.0f));
    instructionText->SetOrigin(Origins::MC);

    // ���ư��� �ؽ�Ʈ
    returnText->SetCharacterSize(28);
    returnColor = sf::Color(0, 255, 0); // �ʷϻ� ����
    returnText->SetFillColor(sf::Color(0, 255, 0, 0));
    returnText->SetString("Press ESC to Return");
    returnText->SetPosition(sf::Vector2f(center.x, center.y + 50.0f));
    returnText->SetOrigin(Origins::MC);

    // ���� �ؽ�Ʈ
    exitText->SetCharacterSize(28);
    exitColor = sf::Color(255, 0, 0); // ������ ����
    exitText->SetFillColor(sf::Color(255, 0, 0, 0));
    exitText->SetString("Press Q to Exit");
    exitText->SetPosition(sf::Vector2f(center.x, center.y + 100.0f));
    exitText->SetOrigin(Origins::MC);
}

void SceneOverlay::SetupVictoryUI()
{
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2f center = windowSize * 0.5f;

    // Victory���� ����ϴ� UI ��ҵ鸸 Ȱ��ȭ
    if (titleText) titleText->SetActive(true);
    if (currentScoreText) currentScoreText->SetActive(true);
    if (instructionText) instructionText->SetActive(true);

    // ������� �ʴ� ��ҵ� ��Ȱ��ȭ
    if (highScoreText) highScoreText->SetActive(false);
    if (retryText) retryText->SetActive(false);
    if (exitText) exitText->SetActive(false);
    if (returnText) returnText->SetActive(false);

    // WAVE CLEAR ���� (ū �۾�, �ʷϻ�)
    titleText->SetCharacterSize(64);
    titleColor = sf::Color(0, 255, 0); // �ʷϻ� ����
    titleText->SetFillColor(sf::Color(0, 255, 0, 0)); // �ʷϻ�, �ʱ⿡�� ����
    titleText->SetString("WAVE " + std::to_string(currentWave) + " CLEAR!");
    titleText->SetPosition(sf::Vector2f(center.x, center.y - 100.0f));
    titleText->SetOrigin(Origins::MC);

    // ���� ���� (�߰� �۾�, ���)
    currentScoreText->SetCharacterSize(36);
    currentScoreColor = sf::Color(255, 255, 255); // ��� ����
    currentScoreText->SetFillColor(sf::Color(255, 255, 255, 0)); // ���, �ʱ⿡�� ����
    currentScoreText->SetString("Score: " + std::to_string(currentScore));
    currentScoreText->SetPosition(sf::Vector2f(center.x, center.y - 20.0f));
    currentScoreText->SetOrigin(Origins::MC);

    // ����ϱ� �ȳ� (���� �۾�, �����)
    instructionText->SetCharacterSize(28);
    instructionColor = sf::Color(255, 255, 0); // ����� ����
    instructionText->SetFillColor(sf::Color(255, 255, 0, 0)); // �����, �ʱ⿡�� ����
    instructionText->SetString("Press SPACE to Continue");
    instructionText->SetPosition(sf::Vector2f(center.x, center.y + 80.0f));
    instructionText->SetOrigin(Origins::MC);
}