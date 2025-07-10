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
    sortingOrder = 100; // 최상위에 표시

    // 배경 설정
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    background.setSize(windowSize);
    background.setFillColor(sf::Color(0, 0, 0, 0)); // 초기에는 투명
    background.setPosition(0, 0);

    // TextGo 객체들 생성
    CreateTextObjects();
}

void SceneOverlay::Release()
{
    // TextGo 객체들을 직접 해제
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

    // 배경 투명하게 리셋
    background.setFillColor(sf::Color(0, 0, 0, 0));
}

void SceneOverlay::CreateTextObjects()
{
    // TextGo 객체들을 Scene에 추가하지 않고 직접 관리
    titleText = new TextGo("fonts/zombiecontrol.ttf", "OverlayTitle");
    currentScoreText = new TextGo("fonts/zombiecontrol.ttf", "OverlayCurrentScore");
    highScoreText = new TextGo("fonts/zombiecontrol.ttf", "OverlayHighScore");
    instructionText = new TextGo("fonts/zombiecontrol.ttf", "OverlayInstruction");
    retryText = new TextGo("fonts/zombiecontrol.ttf", "OverlayRetry");
    exitText = new TextGo("fonts/zombiecontrol.ttf", "OverlayExit");
    returnText = new TextGo("fonts/zombiecontrol.ttf", "OverlayReturn");

    // TextGo들의 Init() 직접 호출
    titleText->Init();
    currentScoreText->Init();
    highScoreText->Init();
    instructionText->Init();
    retryText->Init();
    exitText->Init();
    returnText->Init();

    // UI 레이어 설정
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
        // 페이드 인
        float progress = fadeTimer / fadeDuration;
        progress = std::min(progress, 1.0f);

        sf::Uint8 alpha = static_cast<sf::Uint8>(progress * 180); // 최대 180 알파값
        background.setFillColor(sf::Color(0, 0, 0, alpha));

        // 텍스트 알파값도 조정
        sf::Uint8 textAlpha = static_cast<sf::Uint8>(progress * 255);

        // 각 텍스트가 활성화되어 있을 때만 알파값 적용
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
            isActive = true; // 페이드 인 완료 후 입력 활성화
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
            // 게임 재시작
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Game);
        }
        else if (InputMgr::GetKeyDown(sf::Keyboard::Q) || InputMgr::GetKeyDown(sf::Keyboard::Escape))
        {
            // 메인 메뉴로
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Title);
        }
        break;

    case OverlayType::Pause:
        if (InputMgr::GetKeyDown(sf::Keyboard::Escape) || InputMgr::GetKeyDown(sf::Keyboard::P))
        {
            // 게임 재개
            Hide();
        }
        else if (InputMgr::GetKeyDown(sf::Keyboard::Q))
        {
            // 메인 메뉴로
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Title);
        }
        break;

    case OverlayType::Victory:
        if (InputMgr::GetKeyDown(sf::Keyboard::Space) || InputMgr::GetKeyDown(sf::Keyboard::Enter))
        {
            // 다음 웨이브 또는 업그레이드 씬으로
            Hide();
            SCENE_MGR.ChangeScene(SceneIds::Upgrade);
        }
        break;
    }
}

void SceneOverlay::Draw(sf::RenderWindow& window)
{
    if (!isVisible) return;

    // UI 뷰 설정
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::View uiView;
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize * 0.5f);
    window.setView(uiView);

    // 반투명 배경 그리기
    window.draw(background);

    // TextGo들을 직접 그리기
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
    isActive = false; // 페이드 인 중에는 입력 비활성화
    fadeTimer = 0.0f;
    inputCooldown = 0.0f;
    isFadingIn = true;

    // 타입에 따른 UI 설정
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

    // 모든 텍스트 숨기기
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

    // GAME OVER UI 요소들 활성화
    if (titleText) titleText->SetActive(true);
    if (currentScoreText) currentScoreText->SetActive(true);
    if (highScoreText) highScoreText->SetActive(true);
    if (instructionText) instructionText->SetActive(true);
    if (retryText) retryText->SetActive(true);
    if (exitText) exitText->SetActive(true);

    // PAUSE UI 요소들 비활성화
    if (returnText) returnText->SetActive(false);

    // GAME OVER 제목
    titleText->SetCharacterSize(72);
    titleColor = sf::Color(255, 0, 0); // 빨간색 저장
    titleText->SetFillColor(sf::Color(255, 0, 0, 0)); // 초기에는 투명
    titleText->SetString("GAME OVER");
    titleText->SetPosition(sf::Vector2f(center.x, center.y - 150.0f));
    titleText->SetOrigin(Origins::MC);

    // 현재 점수
    currentScoreText->SetCharacterSize(36);
    currentScoreColor = sf::Color(255, 255, 255); // 흰색 저장
    currentScoreText->SetFillColor(sf::Color(255, 255, 255, 0));
    currentScoreText->SetString("SCORE: " + std::to_string(currentScore));
    currentScoreText->SetPosition(sf::Vector2f(center.x, center.y - 80.0f));
    currentScoreText->SetOrigin(Origins::MC);

    // 최고 점수
    highScoreText->SetCharacterSize(32);
    highScoreColor = sf::Color(255, 255, 0); // 노란색 저장
    highScoreText->SetFillColor(sf::Color(255, 255, 0, 0));
    highScoreText->SetString("HIGH SCORE: " + std::to_string(highScore));
    highScoreText->SetPosition(sf::Vector2f(center.x, center.y - 30.0f));
    highScoreText->SetOrigin(Origins::MC);

    // 안내문
    instructionText->SetCharacterSize(24);
    instructionColor = sf::Color(200, 200, 200); // 회색 저장
    instructionText->SetFillColor(sf::Color(200, 200, 200, 0));
    instructionText->SetString("Choose an option:");
    instructionText->SetPosition(sf::Vector2f(center.x, center.y + 40.0f));
    instructionText->SetOrigin(Origins::MC);

    // 재시도 텍스트
    retryText->SetCharacterSize(28);
    retryColor = sf::Color(0, 255, 0); // 초록색 저장
    retryText->SetFillColor(sf::Color(0, 255, 0, 0));
    retryText->SetString("Press R to Retry");
    retryText->SetPosition(sf::Vector2f(center.x, center.y + 90.0f));
    retryText->SetOrigin(Origins::MC);

    // 종료 텍스트
    exitText->SetCharacterSize(28);
    exitColor = sf::Color(255, 0, 0); // 빨간색 저장
    exitText->SetFillColor(sf::Color(255, 0, 0, 0));
    exitText->SetString("Press Q to Exit");
    exitText->SetPosition(sf::Vector2f(center.x, center.y + 130.0f));
    exitText->SetOrigin(Origins::MC);
}

void SceneOverlay::SetupPauseUI()
{
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2f center = windowSize * 0.5f;

    // PAUSE UI 요소들 활성화
    if (titleText) titleText->SetActive(true);
    if (instructionText) instructionText->SetActive(true);
    if (returnText) returnText->SetActive(true);
    if (exitText) exitText->SetActive(true);

    // GAME OVER UI 요소들 비활성화
    if (currentScoreText) currentScoreText->SetActive(false);
    if (highScoreText) highScoreText->SetActive(false);
    if (retryText) retryText->SetActive(false);

    // PAUSED 제목
    titleText->SetCharacterSize(64);
    titleColor = sf::Color(255, 255, 0); // 노란색 저장
    titleText->SetFillColor(sf::Color(255, 255, 0, 0));
    titleText->SetString("PAUSED");
    titleText->SetPosition(sf::Vector2f(center.x, center.y - 100.0f));
    titleText->SetOrigin(Origins::MC);

    // 안내문
    instructionText->SetCharacterSize(24);
    instructionColor = sf::Color(200, 200, 200); // 회색 저장
    instructionText->SetFillColor(sf::Color(200, 200, 200, 0));
    instructionText->SetString("Game is paused");
    instructionText->SetPosition(sf::Vector2f(center.x, center.y - 20.0f));
    instructionText->SetOrigin(Origins::MC);

    // 돌아가기 텍스트
    returnText->SetCharacterSize(28);
    returnColor = sf::Color(0, 255, 0); // 초록색 저장
    returnText->SetFillColor(sf::Color(0, 255, 0, 0));
    returnText->SetString("Press ESC to Return");
    returnText->SetPosition(sf::Vector2f(center.x, center.y + 50.0f));
    returnText->SetOrigin(Origins::MC);

    // 종료 텍스트
    exitText->SetCharacterSize(28);
    exitColor = sf::Color(255, 0, 0); // 빨간색 저장
    exitText->SetFillColor(sf::Color(255, 0, 0, 0));
    exitText->SetString("Press Q to Exit");
    exitText->SetPosition(sf::Vector2f(center.x, center.y + 100.0f));
    exitText->SetOrigin(Origins::MC);
}

void SceneOverlay::SetupVictoryUI()
{
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2f center = windowSize * 0.5f;

    // Victory에서 사용하는 UI 요소들만 활성화
    if (titleText) titleText->SetActive(true);
    if (currentScoreText) currentScoreText->SetActive(true);
    if (instructionText) instructionText->SetActive(true);

    // 사용하지 않는 요소들 비활성화
    if (highScoreText) highScoreText->SetActive(false);
    if (retryText) retryText->SetActive(false);
    if (exitText) exitText->SetActive(false);
    if (returnText) returnText->SetActive(false);

    // WAVE CLEAR 제목 (큰 글씨, 초록색)
    titleText->SetCharacterSize(64);
    titleColor = sf::Color(0, 255, 0); // 초록색 저장
    titleText->SetFillColor(sf::Color(0, 255, 0, 0)); // 초록색, 초기에는 투명
    titleText->SetString("WAVE " + std::to_string(currentWave) + " CLEAR!");
    titleText->SetPosition(sf::Vector2f(center.x, center.y - 100.0f));
    titleText->SetOrigin(Origins::MC);

    // 현재 점수 (중간 글씨, 흰색)
    currentScoreText->SetCharacterSize(36);
    currentScoreColor = sf::Color(255, 255, 255); // 흰색 저장
    currentScoreText->SetFillColor(sf::Color(255, 255, 255, 0)); // 흰색, 초기에는 투명
    currentScoreText->SetString("Score: " + std::to_string(currentScore));
    currentScoreText->SetPosition(sf::Vector2f(center.x, center.y - 20.0f));
    currentScoreText->SetOrigin(Origins::MC);

    // 계속하기 안내 (작은 글씨, 노란색)
    instructionText->SetCharacterSize(28);
    instructionColor = sf::Color(255, 255, 0); // 노란색 저장
    instructionText->SetFillColor(sf::Color(255, 255, 0, 0)); // 노란색, 초기에는 투명
    instructionText->SetString("Press SPACE to Continue");
    instructionText->SetPosition(sf::Vector2f(center.x, center.y + 80.0f));
    instructionText->SetOrigin(Origins::MC);
}