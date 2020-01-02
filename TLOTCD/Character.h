#pragma once

#include <string>

class UiItem_Label; 
class UiItem_Image; 

struct Stats
{
	unsigned int HP = 100;
	unsigned int dmg = 20;
	unsigned int dmg2 = 30;
	unsigned int dmgCharge = 2; 
	unsigned int dmg2Charge = 3; 
	unsigned int charge = 10;
	unsigned int evadeProb = 50; 
	float blockPercentatge = 0.5f; 

	// To Reset
	unsigned int initialHP = HP; 
	unsigned int initialCharge = charge;

	// For AI, to simulate thinking
	float currentTime = 0.f; 
    float thinkTime = 2.f;
	bool lastEvaded = false; 

	void Reset()
	{
		HP = initialHP; 
		charge = initialCharge; 
		currentTime = 0.f; 
	}

};


enum class strategyMode
{
	RANDOM,
	WEAK_ATTACK,
	STRONG_ATTACK,
	CHARGE,
	HP
};

enum class defenseStrategyMode
{
	RANDOM,
	BLOCK,
	EVADE,
};

enum class currenDefense
{
	BLOCK,
	EVADE
};

class Character
{
public: 
	enum class decisionType { RANDOM, PLAYER, REPEAT };
 
	Character() {}; 
	~Character() {};
	Character(bool active, bool enemy, bool AI, UiItem_Image* icon); 

private: 
	void Update(float dt); 

	// Generic
	void Attack(unsigned int damage, unsigned int charge, bool updateLabel = true);
	void ReCharge(bool updateLabel = true);
	void Heal(bool updateLabel = true);
	void Evade();

	// For Each Case
	void AI_WeakAttack();
	void AI_StrongAttack();
	void AI_Recharge();
	void AI_Heal();

	// For AI
	void UpdateLabels(); 

private: 
	Stats stats; 
	strategyMode mode = strategyMode::RANDOM; 
	defenseStrategyMode dMode = defenseStrategyMode::RANDOM; 
	currenDefense currentDefense = currenDefense::BLOCK; 
	bool actionCompleted = false; 
	bool attackTurn = false; 
	bool active = true; 
	bool enemy = true; 
	bool AI = true;
	std::string lastPassiveAction = "empty"; 
	UiItem_Image* icon = nullptr; 
	UiItem_Label* hpLabel = nullptr; 
	UiItem_Label* chargeLabel = nullptr;

	friend class j1Gui; 
	friend class j1EntityFactory; 
};