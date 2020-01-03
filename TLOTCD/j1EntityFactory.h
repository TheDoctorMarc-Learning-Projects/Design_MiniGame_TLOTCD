#ifndef __J1ENTITYFACTORY_H__
#define __J1ENTITYFACTORY_H__

#include "j1Module.h"
 
#include "SDL/include/SDL.h"
#include <vector>
#include <list>
#include "Color.h"
#include <map>
#include <array>
#include "p2Point.h"
#include "Character.h"
#include <random>
#include "j1Fonts.h"

#define RECHARGE 3

class RNG; 
class j1EntityFactory : public j1Module
{
public:
	j1EntityFactory();
	~j1EntityFactory();

	bool Awake(pugi::xml_node& node);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp() { 
		
		for (auto& c : characters)
		{
			c->CleanUp(); 
			RELEASE(c);
		}
		
		characters.clear(); 

		for (auto& c : actionHelpers)
			RELEASE(c);
		actionHelpers.clear(); 

		for (auto& c : defenseHelpers)
			RELEASE(c);
		defenseHelpers.clear();

		for (auto& c : AIHelpers)
			RELEASE(c);
		AIHelpers.clear();

		for (auto& c : AIHelpersEmemy)
			RELEASE(c);
		AIHelpersEmemy.clear(); 

		for (auto& c : allyCharacters)
			RELEASE(c);
		allyCharacters.clear(); 

		for (auto& c : enemyCharacters)
			RELEASE(c);
		enemyCharacters.clear();

		RELEASE(dmgLabel); 
		RELEASE(rng); 

		return true; 
	};


	bool hasIntersectionRectAndLine(const SDL_Rect* rect, std::array<int, 4> line) const  // line is passed like this: {x1, y1, x2, y2}
	{ 
		return SDL_IntersectRectAndLine(rect, &line[0], &line[1], &line[2], &line[3]);
	}

	bool isPointInsideRect(const iPoint* p, const SDL_Rect* r) const {
		const SDL_Point P = { p->x, p->y }; 
		return SDL_PointInRect(&P, r);
	}

	void CheckBothCompleted()
	{
		uint success = 0; 
		Character* lastAttacker = nullptr; 
		Character* lastDefender = nullptr;

		for (auto& c : characters)
		{
			if (c->active && c->actionCompleted)
			{
				if (c->attackTurn)
					lastAttacker = c;
				else
					lastDefender = c; 

				success++;
			}
				
			if (success == 2)
				break; 
		}

		if (success == 2)
		{
			SwitchTurn(lastAttacker, lastDefender);
		}
		

	}

	void SwitchTurn(Character* lastAttacker, Character* lastDefender); 

	void ToggleAIModes(bool ally, bool enemy, Character* allyC, Character* enemyC);

	void ToggleUIVisibility(bool ally, bool enemy, Character* allyC, Character* enemyC);

	void ToggleBattleMode(); 

	void Death(Character* dead);

	void ResetAIHelperColors(bool ally, bool enemy); 

public:
	bool AIvsAI = false; 
	float popUpCurrentTime = 0.f; 
	float popUpTime = 2.f; 
	float popUpWinCurrentTime = 0.f; 
	float popUpWinTime = 2.f; 
	unsigned int currentAttack = 0; 
	UiItem_Label* dmgLabel = nullptr; 
	RNG* rng = nullptr; 
	std::vector<Character*> characters; 
	std::vector<Character*> allyCharacters; 
	std::vector<Character*> enemyCharacters;
	std::vector<UiItem_Image*> actionHelpers;
	std::vector<UiItem_Image*> defenseHelpers;
	std::vector<UiItem_Label*> AIHelpers; // attack and defence labels
	std::vector<UiItem_Label*> AIHelpersEmemy; // attack and defence labels
	UiItem_Image* topRight = nullptr; 
	UiItem_Image* bottomLeft = nullptr;
	UiItem_Label* allyName = nullptr; 
	UiItem_Label* enemyName = nullptr;
	UiItem_Label* winner = nullptr; 
};


#endif  