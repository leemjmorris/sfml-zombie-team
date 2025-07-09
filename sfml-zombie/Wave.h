#pragma once
#include "Scene.h"
class Wave : public Scene
{
protected:

public:
	Wave();
	~Wave() = default;

	void Init() override;
	void Enter() override;

	void Update(float dt) override;
};

