#include "stdafx.h"
#include "Skill.h"
#include "Player.h"

Skill::Skill(SkillType ty)
	:type(ty)
{
	for (int i = 0; i < (int)SkillType::skillCount; i++)
	{
		CanUse[i] = true;
	}
	skillUsingTime[(int)SkillType::Dash] = 0.1f;
	coolTime[(int)SkillType::Dash] = 10.f;
	skillUsingTime[(int)SkillType::FastShoot] = 0.5f;
	coolTime[(int)SkillType::FastShoot] = 20.f;
}

void Skill::Use(SkillType ty)
{
	switch (ty)
	{
	case Skill::SkillType::Dash:
		Dash();
		break;
	case Skill::SkillType::FastShoot:
		Dash();
		break;
	default:
		break;
	}
}

bool Skill::IsCoolTime(float t, SkillType ty)
{
	return t < skillUsingTime[(int)ty] + coolTime[(int)ty];
}

void Skill::Dash()
{
	if (CanUse[(int)SkillType::Dash])
	{
		player->SetSpeed(player->GetSpeed() + dashAmount);
		CanUse[(int)SkillType::Dash] = false;
	}
}

void Skill::FastShoot(float t)
{
	if (CanUse[(int)SkillType::FastShoot])
	{
		CanUse[(int)SkillType::FastShoot] = false;
	}
}

void Skill::FinishSkill(SkillType ty)
{
	switch (ty)
	{
	case Skill::SkillType::Dash:
		player->SetSpeed(player->GetSpeed() - dashAmount);
		CanUse[(int)ty] = true;
		break;
	case Skill::SkillType::FastShoot:
		break;
	default:
		break;
	}
}
