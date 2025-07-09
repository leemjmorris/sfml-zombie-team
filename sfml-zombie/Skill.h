#pragma once
class Player;

class Skill
{
public:
	enum class SkillType
	{
		None = -1,
		Dash,
		FastShoot,

		skillCount
	};

protected:
	std::string name = "";

	Player* player = nullptr;

	float skillUsingTime[(int)SkillType::skillCount];
	float coolTime[(int)SkillType::skillCount];

	float dashAmount = 600.f;

	bool CanUse[(int)SkillType::skillCount];

	SkillType type;

public:
	Skill(SkillType ty = SkillType::None);
	~Skill() = default;

	void SetPlayer(Player* player) { this->player = player; }

	void SetUseTime(float ut, SkillType ty) { skillUsingTime[(int)ty] = ut; }
	float GetUseTime(SkillType ty) const { return skillUsingTime[(int)ty]; }
	void SetCoolTime(float ct, SkillType ty) { coolTime[(int)ty] = ct; }
	float GetCoolTime(SkillType ty) const { return coolTime[(int)ty]; }
	void SetCanUse(bool b, SkillType ty) { CanUse[(int)ty] = b; }
	bool GetCanUse(SkillType ty) { return CanUse[(int)ty]; }

	void Use(SkillType ty);
	bool IsCoolTime(float t, SkillType ty);

	void Dash();
	void FastShoot(float t);

	void FinishSkill(SkillType ty);
};

