#pragma once
#include "Scene.h"
class SceneGameOver :  public Scene
{
protected:

public:
	SceneGameOver();
	~SceneGameOver() = default;

	void Init() override {};
	void Enter() override {};

	void Update(float dt) override {};
};

