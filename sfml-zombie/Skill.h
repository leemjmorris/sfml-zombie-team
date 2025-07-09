#pragma once
class Player;

class Skill
{
public:
	enum class SkillType
	{
		None = -1,
		Dash,
	};

protected:
	std::string name = "";

	Player* player = nullptr;

	float skillUsingTime = 5.f;
	float useCoolTime = 10.f;
	float checkTime = 0.f;

	SkillType type;

public:
	Skill(SkillType ty = SkillType::None);
	~Skill() = default;

	void SetPlayer(Player* player) { this->player = player; }

	void SetUseTime(float ut) { skillUsingTime = ut; }
	float GetUseTime() const { return skillUsingTime; }
	void SetCoolTime(float ct) { useCoolTime = ct; }
	float GetCoolTime() const { return useCoolTime; }

	void Use();
	void Dash(float amount);
};

