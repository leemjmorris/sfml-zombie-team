#pragma once
#include "Scene.h"

class MenuUI;
class Player;
class SpriteGo;

class SceneUpgrade : public Scene
{
protected:
	MenuUI* menuUi = nullptr;
	Player* player = nullptr;
	SpriteGo* Background = nullptr;

public:
	SceneUpgrade();
	~SceneUpgrade() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
};

