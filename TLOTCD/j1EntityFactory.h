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
			RELEASE(c); 
		characters.clear(); 

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

	void SwitchTurn(Character* lastAttacker, Character* lastDefender)
	{
		// Defender now attacks
		lastDefender->attackTurn = true;

		// Attacker now defends
		lastAttacker->attackTurn = false;

		// Reset state
		lastDefender->actionCompleted = lastAttacker->actionCompleted = false; 
	}

	void PlayerHelperDecider(Character* c); 

public:
	RNG* rng = nullptr; 
	std::vector<Character*> characters; 
	std::vector<UiItem_Image*> actionHelpers;
	std::vector<UiItem_Image*> defenseHelpers;
};


#endif  