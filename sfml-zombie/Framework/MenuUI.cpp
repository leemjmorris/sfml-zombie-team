#include "stdafx.h"
#include "MenuUI.h"

MenuUI::MenuUI(const std::string& name)
	:GameObject(name)
{
}

void MenuUI::AddTextPos(const sf::Vector2f& position)
{
	textPos.push_back(position);
}

void MenuUI::AddMessages(const std::string& message)
{
	textMessages.push_back(message);
	menuCount++;
}

void MenuUI::AddTextGo(const TextGo& text)
{
	texts.push_back(text);
}

void MenuUI::SetChooseBarColor(sf::Color c)
{
	chooseBar.setFillColor(c);
}

void MenuUI::SetChooseBarThickness(float t)
{
	chooseBar.setOutlineThickness(t);
}

void MenuUI::SetChooseBarOutColor(sf::Color c)
{
	chooseBar.setOutlineColor(c);
}

void MenuUI::SetChooseBarOrigin(Origins o)
{
	chooseBarOriginPreset = o;
	Utils::SetOrigin(chooseBar, o);
}

void MenuUI::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;

	chooseBarPos = initChooseBarPos;
	chooseBar.setSize(chooseBarSize);
	Utils::SetOrigin(chooseBar, chooseBarOriginPreset);
}

void MenuUI::Release()
{
}

void MenuUI::Reset()
{
	for (int i = 0; i < menuCount; i++)
	{
		texts[i].GetText().setFont(FONT_MGR.Get(fontId));
		texts[i].SetString(textMessages[i]);
	}

	chooseBarPos = initChooseBarPos;
	chooseBar.setPosition(textPos[chooseBarPos] + chooseBarOffset);
}

void MenuUI::Update(float dt)
{
	chooseBar.setPosition(textPos[chooseBarPos] + chooseBarOffset);
	for (int i = 0; i < menuCount; i++)
	{
		if (i == chooseBarPos)
		{
			texts[i].SetFillColor(choosedColor);
		}
		else
		{
			texts[i].SetFillColor(notChoosedColor);
		}
	}
}

void MenuUI::Draw(sf::RenderWindow& window)
{
	if (active)
	{
		for (int i = 0; i < menuCount; i++)
		{
			texts[i].Draw(window);
		}
		window.draw(chooseBar);
	}
}
