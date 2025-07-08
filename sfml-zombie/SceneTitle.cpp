#include "stdafx.h"
#include "SceneTitle.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "SceneMgr.h"
#include "InputMgr.h"

SceneTitle::SceneTitle() : Scene(SceneIds::Title),
	fontInit(false),
	isShowTitleMessage(true),
	isShowIntroMessage(true),
	fadeAlpha(255.0f),
	fadeIn(false),
	fadeOut(false),
	fadeDuration(1.0f),
	fadeTimer(0.0f),
	blinkTimer(0.0f),
	blinkTime(1.0f),
	Visible(true)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	LoadResources();
	InitializeUI();
	Scene::Init();
}

void SceneTitle::Enter()
{
	Scene::Enter();
	SetupViews();
	SetupBackground();
	SetupFadeEffect();
	Reset();
}

void SceneTitle::Exit()
{
}

void SceneTitle::Reset()
{
	InitializeFonts();
	ResetVisibilityFlags();
	SetupTextContent();
	PositionUIElements();
}

void SceneTitle::Update(float dt)
{
	UpdateFadeEffect(dt);
	UpdateBlinkEffect(dt);
	HandleInput();
	Scene::Update(dt);
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	window.setView(uiView);
	window.draw(backgroundSprite);

	window.draw(textTitle);
	if (Visible)
	{
		window.draw(textIntro);
	}

	Scene::Draw(window);

	window.setView(uiView);
	window.draw(fadeRect);
}

void SceneTitle::LoadResources()
{
	texIds.push_back("graphics/background.png");
	fontIds.push_back("fonts/zombiecontrol.ttf");
	fontId = "fonts/zombiecontrol.ttf";
}

void SceneTitle::InitializeUI()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f windowCenter = windowSize * 0.5f;

	SetupTitleText(windowCenter);
	SetupIntroText(windowCenter);
}

void SceneTitle::SetupTitleText(const sf::Vector2f& windowCenter)
{
	textTitle.setCharacterSize(100);
	textTitle.setFillColor(sf::Color::Red);
	textTitle.setPosition(windowCenter.x, windowCenter.y - 100.f);
}

void SceneTitle::SetupIntroText(const sf::Vector2f& windowCenter)
{
	textIntro.setCharacterSize(50);
	textIntro.setFillColor(sf::Color::White);
	textIntro.setPosition(windowCenter.x, windowCenter.y + 100.f);
}

void SceneTitle::SetupViews()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
}

void SceneTitle::SetupBackground()
{
	backgroundSprite.setTexture(TEXTURE_MGR.Get("graphics/background.png"));
	backgroundSprite.setOrigin(0.f, 0.f);
	backgroundSprite.setPosition(0.f, 0.f);
	backgroundSprite.setScale(1.f, 1.f);
}

void SceneTitle::SetupFadeEffect()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

	fadeRect.setSize(windowSize);
	fadeRect.setPosition(0.f, 0.f);
	fadeRect.setFillColor(sf::Color(0, 0, 0, 255));

	fadeAlpha = 255.0f;
	fadeIn = true;
	fadeOut = false;
	fadeTimer = 0.0f;
}

void SceneTitle::InitializeFonts()
{
	if (!fontInit)
	{
		textTitle.setFont(FONT_MGR.Get(fontId));
		textIntro.setFont(FONT_MGR.Get(fontId));
		fontInit = true;
	}
}

void SceneTitle::ResetVisibilityFlags()
{
	isShowTitleMessage = true;
	isShowIntroMessage = true;
}

void SceneTitle::SetupTextContent()
{
	SetTitleMessage("ZOMBIE ARENA");
	SetIntroMessage("< Press Any Key to Begin >");
}

void SceneTitle::PositionUIElements()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	sf::Vector2f windowCenter = windowSize * 0.5f;

	textTitle.setPosition(windowCenter.x, windowCenter.y - 100.f);
	textIntro.setPosition(windowCenter.x, windowCenter.y + 100.f);
}

void SceneTitle::SetTitleMessage(const std::string& msg)
{
	textTitle.setString(msg);
	CenterTextOrigin(textTitle);
}

void SceneTitle::SetIntroMessage(const std::string& msg)
{
	textIntro.setString(msg);
	CenterTextOrigin(textIntro);
}

void SceneTitle::CenterTextOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width * 0.5f,
		bounds.top + bounds.height * 0.5f);
}

void SceneTitle::UpdateFadeEffect(float dt)
{
	if (fadeIn)
	{
		ProcessFadeIn(dt);
	}
	else if (fadeOut)
	{
		ProcessFadeOut(dt);
	}
}

void SceneTitle::ProcessFadeIn(float dt)
{
	fadeTimer += dt;
	
	float progress = fadeTimer / fadeDuration; //진행률 계산 (0.0 ~ 1.0)
	if (progress > 1.0f) progress = 1.0f;
	
	fadeAlpha = 255.0f * (1.0f - progress); //255에서 0으로 감소

	if (progress >= 1.0f)
	{
		fadeAlpha = 0.0f;
		fadeIn = false;
		fadeTimer = 0.0f;
		std::cout << "Fade in complete!" << std::endl; //디버깅용
	}

	UpdateFadeColor();
}

void SceneTitle::ProcessFadeOut(float dt)
{
	fadeTimer += dt;
	
	float progress = fadeTimer / fadeDuration; //진행률 계산 (0.0 ~ 1.0)
	if (progress > 1.0f) progress = 1.0f;
	
	fadeAlpha = 255.0f * progress; //0에서 255로 증가

	std::cout << "Fade out - fadeAlpha: " << fadeAlpha << std::endl; //디버깅용

	if (progress >= 1.0f)
	{
		fadeAlpha = 255.0f;
		fadeTimer = 0.0f;
		std::cout << "Attempting to change scene to Game!" << std::endl; //디버깅용
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	UpdateFadeColor();
}

void SceneTitle::UpdateFadeColor()
{	
	int alphaInt = static_cast<int>(fadeAlpha + 0.5f); //float을 int로 변환할 때 반올림
	fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alphaInt)));
}

void SceneTitle::UpdateBlinkEffect(float dt)
{
	blinkTimer += dt;
	if (blinkTimer >= blinkTime)
	{
		blinkTimer = 0.f;
		Visible = !Visible;
	}
}

void SceneTitle::HandleInput()
{
	if (!fadeIn && !fadeOut && InputMgr::AnyKeyDown())
	{
		std::cout << "Key pressed! Starting fade out..." << std::endl; //디버깅용
		fadeOut = true;
		fadeTimer = 0.0f; //타이머 리셋 추가
	}
}