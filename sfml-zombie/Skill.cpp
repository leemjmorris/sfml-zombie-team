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
	skillUsingTime[(int)SkillType::FastShoot] = 1.5f;
	coolTime[(int)SkillType::FastShoot] = 5.f;
}

void Skill::Reset()
{
	for (int i = 0; i < (int)SkillType::skillCount; i++)
	{
		CanUse[i] = true;
	}
}

void Skill::Use(SkillType ty)
{
	switch (ty)
	{
	case Skill::SkillType::Dash:
		Dash();
		break;
	case Skill::SkillType::FastShoot:
		FastShoot();
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

void Skill::FastShoot()
{
	if (CanUse[(int)SkillType::FastShoot])
	{
		player->SetShootInterval(player->GetShootInterval() / fastShootAmount);
		CanUse[(int)SkillType::FastShoot] = false;
	}
}

void Skill::FinishSkill(SkillType ty)
{
	switch (ty)
	{
	case Skill::SkillType::Dash:
		player->SetSpeed(player->GetSpeed() - dashAmount);
		break;
	case Skill::SkillType::FastShoot:
		player->SetShootInterval(player->GetShootInterval() * fastShootAmount);
		break;
	default:
		break;
	}
	CanUse[(int)ty] = true;
}
