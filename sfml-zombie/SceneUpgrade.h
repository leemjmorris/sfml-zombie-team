#pragma once
#include "Scene.h"

class MenuUI;
class Player;

class SceneUpgrade : public Scene
{
protected:
	MenuUI* menuUi = nullptr;
	Player* player = nullptr;

public:
	SceneUpgrade();
	~SceneUpgrade() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
};

