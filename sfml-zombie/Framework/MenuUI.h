#pragma once
#include "GameObject.h"
#include "TextGo.h"
class MenuUI : public GameObject
{
protected:
	std::vector<TextGo> texts;
	sf::RectangleShape chooseBar;
	
	Origins chooseBarOriginPreset = Origins::MC;
	sf::Vector2f chooseBarOffset = sf::Vector2f{ 0.f, 15.f };

	std::string fontId;

	sf::Vector2f chooseBarSize;
	std::vector<sf::Vector2f> textPos;
	std::vector<std::string> textMessages;

	sf::Color choosedColor;
	sf::Color notChoosedColor;

	int chooseBarPos = 0;
	int initChooseBarPos = 0;
	int menuCount = 0;

public:
	MenuUI(const std::string& name = "");
	~MenuUI() override = default;

	void SetMenuCount(int count) {menuCount = count;}

	void SetChooseBarSize(const sf::Vector2f& size) { chooseBarSize = size; }
	void AddTextPos(const sf::Vector2f& position);
	std::vector<sf::Vector2f> GetTextPos() const { return textPos; }
	void AddMessages(const std::string& message);
	void AddTextGo(const TextGo& text);
	void SetFontId(std::string Id) { fontId = Id; }

	void SetChooseBarColor(sf::Color c);
	void SetChooseBarThickness(float t);
	void SetChooseBarOutColor(sf::Color c);
	void SetChooseBarOrigin(Origins o);
	void SetChooseBarInitPos(int p) { initChooseBarPos = p; }
	void SetChooseBarOffset(const sf::Vector2f& offset) { chooseBarOffset = offset; }

	void SetChoosedColor(sf::Color c) { choosedColor = c; }
	void SetNotChoosedColor(sf::Color c) { notChoosedColor = c; }

	void SetBarPos(int pos) { chooseBarPos = pos; }
	int GetBarPos() const { return chooseBarPos; }

	int GetMenuCount() const { return menuCount; }


	std::vector<TextGo> GetTexts() const { return texts; }

	// GameObject을(를) 통해 상속됨
	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

