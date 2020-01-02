#pragma once

class UiItem_Label; 
class UiItem_Image; 

struct Stats
{
	unsigned int HP = 100;
	unsigned int dmg = 20;
	unsigned int dmg2 = 30;
	unsigned int charge = 10;
	unsigned int evadeProb = 50; 
	float blockPercentatge = 50.f; 

	// To Reset
	unsigned int initialHP = HP; 
	unsigned int initialCharge = charge;

	// For AI, to simulate thinking
	float currentTime = 0.f; 
    float thinkTime = 2.f;

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

class Character
{
public: 
	enum class decisionType { RANDOM, PLAYER, REPEAT };
 
	Character() {}; 
	~Character() {};
	Character(bool active, bool enemy, bool AI, UiItem_Image* icon); 

private: 
	void Update(float dt); 

private: 
	Stats stats; 
	strategyMode mode = strategyMode::RANDOM; 
	defenseStrategyMode dMode = defenseStrategyMode::RANDOM; 
	bool actionCompleted = false; 
	bool attackTurn = false; 
	bool active = true; 
	bool enemy = true; 
	bool AI = true;
	UiItem_Image* icon = nullptr; 
	UiItem_Label* hpLabel = nullptr; 
	UiItem_Label* chargeLabel = nullptr;

	friend class j1Gui; 
	friend class j1EntityFactory; 
};