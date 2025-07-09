#include "stdafx.h"
#include "Skill.h"
#include "Player.h"

Skill::Skill(SkillType ty)
	:type(ty)
{
}

void Skill::Use()
{
	switch (type)
	{
	case Skill::SkillType::Dash:
		Dash(100.f);
		break;
	default:
		break;
	}
}

void Skill::Dash(float amount)
{
	player->SetSpeed(player->GetSpeed() + amount);
}
